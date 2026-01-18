#define _GNU_SOURCE
#include "./perf.h"
#include "./agent.h"
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <linux/perf_event.h> 
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <errno.h>

#define NO_PERF_PTR -1l
#define LBR_BRANCH_NR 32
#define RING_BUFFER_PAGES 16

struct perf_event_attr* attr = NULL;
static int perf_flag = false;
static int perf_fd = 0;
static int enable_ret;
static int start_ret;
static void* rbuf;
static uint64_t next_offset=0;

static void init_perf_event_attr(void);
static int start_perf(unsigned long nr, unsigned long arg0, unsigned long arg1,
	      unsigned long arg2);
static int end_perf(void);
static int perf_event_open(struct perf_event_attr* attr, pid_t pid,
		    int cpu, int group_fd, unsigned long flags);
static void write_shared_mem(struct perf_branch_entry* lbrs);
static void perf_prologue(int nr, ...);
static void perf_epilogue(void);
static void perf_setup(void);
static void sample_handler(int sig_num,siginfo_t *sig_info,void *context);

struct perf_sample {
	struct perf_event_header header;
	struct perf_branch_entry lbrs[LBR_BRANCH_NR];
};

static void init_perf_event_attr(void)
{
	attr = (struct perf_event_attr*)malloc(sizeof(struct perf_event_attr));
	if (!attr) {
		printf("malloc failed, no perf mode\n");
		attr = (struct perf_event_attr*)NO_PERF_PTR;
		return;
	}
	memset(attr, 0, sizeof(struct perf_event_attr));
	attr->type = PERF_TYPE_HARDWARE;
	attr->size = sizeof(struct perf_event_attr);
	attr->config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
	attr->exclude_user = 1;
	attr->disabled = 1;
	attr->sample_period = 3200;
	attr->freq = 0;
	attr->sample_type = PERF_SAMPLE_BRANCH_STACK;
	attr->branch_sample_type = PERF_SAMPLE_BRANCH_KERNEL | PERF_SAMPLE_BRANCH_COND;
	attr->wakeup_events=1;
}

static int start_perf(unsigned long nr, unsigned long arg0, unsigned long arg1,
	      unsigned long arg2)
{
	int ret;
	asm volatile(
	    "vmcall"
	    : "=a"(ret)
	    : "a"(100), "b"(nr), "c"(arg0), "d"(arg1), "S"(arg2));
	return ret;
}

static int end_perf(void)
{
	int ret;
	asm volatile(
	    "vmcall"
	    : "=a"(ret)
	    : "a"(101));
	return ret;
}

static int perf_event_open(struct perf_event_attr* attr, pid_t pid,
		    int cpu, int group_fd, unsigned long flags)
{
	return syscall(SYS_perf_event_open, attr, pid, cpu, group_fd, flags);
}

static void write_shared_mem(struct perf_branch_entry* lbrs) {
	int i = 0;
	for (i = 0; i < LBR_NUM; i++){
		shared_memory.start_addr[shared_memory.head + i] = (int64_t)(lbrs[i].from);
	}
	shared_memory.start_addr[i] = DATA_END;
	shared_memory.head += LBR_NUM;

	return;
}

static void perf_prologue(int nr, ...)
{
	va_list vl;
	unsigned long arg0, arg1, arg2;

	printf("[perf] prologue start\n");
	// perf mmap.
	rbuf = mmap(0, (1 + RING_BUFFER_PAGES) * PAGE_SIZE, PROT_WRITE, MAP_SHARED, perf_fd, 0);
	fcntl(perf_fd,F_SETFL,O_RDWR|O_NONBLOCK|O_ASYNC);
    fcntl(perf_fd,F_SETSIG,SIGIO);
    fcntl(perf_fd,F_SETOWN,getpid());
	struct sigaction sig;
	memset(&sig,0,sizeof(struct sigaction));
	sig.sa_sigaction=sample_handler;
	sig.sa_flags = SA_SIGINFO;
	if (sigaction(SIGIO, &sig, 0) < 0)
		return;
	
	perf_flag = 1;
	ioctl(perf_fd, PERF_EVENT_IOC_RESET, 0);
	/* Dangerous NMI part */
	if (nr == 16) {
		// ioctl, pass arguments to hypervisor
		va_start(vl, nr);
		arg0 = va_arg(vl, unsigned long);
		arg1 = va_arg(vl, unsigned long);
		arg2 = va_arg(vl, unsigned long);
		va_end(vl);
		start_ret = start_perf(nr, arg0, arg1, arg2);
	} else {
		// otherwise simply pass syscall nr
		start_ret = start_perf(nr, 0, 0, 0);
	}
	enable_ret = ioctl(perf_fd, PERF_EVENT_IOC_ENABLE, 0);
}

static void perf_epilogue(void)
{
	int end_ret;

	if (!perf_flag)
		return;
	ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
	end_ret = end_perf();
	perf_flag = 0;
	printf("[perf] epilogue done, start_ret: %d, enable_ret: %d,"
	      " end_ret: %d\n",
	      start_ret, enable_ret, end_ret);
	munmap(rbuf,(1 + RING_BUFFER_PAGES) * PAGE_SIZE);
}

static void perf_setup(void)
{
	static int setup = 0;

	// Make sure perf stuffs should be cleared at exit
	if (!setup) {
		setup = 1;
		// signal handler?
		atexit(perf_epilogue);
	}

	if (!attr)
		init_perf_event_attr();
	if ((long)attr == NO_PERF_PTR)
		perf_fd = -1;

	perf_fd = perf_event_open(attr, 0, -1, -1, 0);
	if (perf_fd < 0) {
		printf("perf_event_open failed, errno = %d\n", errno);
	} else {
		printf("[perf] perf_event_open ok\n");
	}
}

static void sample_handler(int sig_num,siginfo_t *sig_info,void *context)
{
    uint64_t offset = 4096 + next_offset;
    struct perf_sample* sample=(void*)((uint8_t*)rbuf+offset);
	static int sig_counter = 0;
	sig_counter += 1;
    if(shared_memory != NULL && sample->header.type==PERF_RECORD_SAMPLE) {
		printf("Writing shared memory, sig_counter: %d\n", sig_counter);
		write_shared_mem(sample->lbrs);
	}
		
    struct perf_event_mmap_page* rinfo = rbuf;
    next_offset = rinfo->data_head%(RING_BUFFER_PAGES * PAGE_SIZE);
}

long perf_open(const char* pathname, int flags)
{
	long ret;

	printf("[perf] perf_open\n");
	perf_setup();
	if (perf_fd < 0) {
		return open(pathname, flags);
	}

	perf_prologue(2);
	ret = open(pathname, flags);
	perf_epilogue();

	close(perf_fd);
	return ret;
}

long perf_ioctl(int fd, int cmd, ...)
{
	long ret;
	void* arg;
	va_list vl;

	printf("[perf] perf_ioctl\n");
	va_start(vl, cmd);
	arg = va_arg(vl, void*);
	va_end(vl);

	perf_setup();
	if (perf_fd < 0) {
		return ioctl(fd, cmd, arg);
	}

	perf_prologue(16, fd, cmd, arg);
	ret = ioctl(fd, cmd, arg);
	perf_epilogue();

	close(perf_fd);
	return ret;
}
# include "./perf.h"
# include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <linux/perf_event.h>
#include <errno.h>
#include <linux/hw_breakpoint.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h> 
#include <string.h>

struct perf_event_attr* attr = NULL;
static int lbr_flag = false;
static int perf_fd = 0;
static int enable_ret;
static int start_ret;
static void init_lbr_perf_event_attr(void)
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
}

int start_nmi_pass(void) {
    int ret;
    asm("vmcall" : "=a"(ret) : "a"(100));
    return ret;
}

int end_nmi_pass(void) {
    int ret;
    asm("vmcall" : "=a"(ret) : "a"(101));
    return ret;
}

static int start_lbr(unsigned long nr, unsigned long arg0, unsigned long arg1,
	      unsigned long arg2)
{
	int ret;
	asm volatile(
	    "vmcall"
	    : "=a"(ret)
	    : "a"(100), "b"(nr), "c"(arg0), "d"(arg1), "S"(arg2));
	return ret;
}

static int end_lbr(void)
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

void lbr_prologue(int nr, ...)
{
	va_list vl;
	unsigned long arg0, arg1, arg2;

	printf("[perf] prologue start\n");
	lbr_flag = 1;
	ioctl(perf_fd, PERF_EVENT_IOC_RESET, 0);
	/* Dangerous NMI part */
	if (nr == 16) {
		// ioctl, pass arguments to hypervisor
		va_start(vl, nr);
		arg0 = va_arg(vl, unsigned long);
		arg1 = va_arg(vl, unsigned long);
		arg2 = va_arg(vl, unsigned long);
		va_end(vl);
		start_ret = start_lbr(nr, arg0, arg1, arg2);
	} else {
		// otherwise simply pass syscall nr
		start_ret = start_lbr(nr, 0, 0, 0);
	}
	enable_ret = ioctl(perf_fd, PERF_EVENT_IOC_ENABLE, 0);
}

void lbr_epilogue(void)
{
	int end_ret;

	if (!lbr_flag)
		return;
	ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
	// ioctl(perf_fd, PERF_EVENT_IOC_REFRESH, 1);
	end_ret = end_lbr();
	/* We're out! */
	lbr_flag = 0;
	printf("[perf] epilogue done, start_ret: %d, enable_ret: %d,"
	      " end_ret: %d\n",
	      start_ret, enable_ret, end_ret);
}

void lbr_setup(void)
{
	static int setup = 0;

	// Make sure perf stuffs should be cleared at exit
	if (!setup) {
		setup = 1;
		// signal handler?
		atexit(lbr_epilogue);
	}

	if (!attr)
		init_lbr_perf_event_attr();
	if ((long)attr == NO_PERF_PTR)
		perf_fd = -1;

	perf_fd = perf_event_open(attr, 0, -1, -1, 0);
	if (perf_fd < 0) {
		printf("perf_event_open failed, errno = %d\n", errno);
	} else {
		printf("[perf] perf_event_open ok\n");
	}
}

long perf_open(const char* pathname, int flags)
{
	long ret;

	printf("[perf] perf_open\n");
	lbr_setup();
	if (perf_fd < 0) {
		return open(pathname, flags);
	}

	lbr_prologue(2);
	ret = open(pathname, flags);
	lbr_epilogue();

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

	lbr_setup();
	if (perf_fd < 0) {
		return ioctl(fd, cmd, arg);
	}

	lbr_prologue(16, fd, cmd, arg);
	ret = ioctl(fd, cmd, arg);
	lbr_epilogue();

	close(perf_fd);
	return ret;
}

int main(){
    printf("hello world!\n");
}
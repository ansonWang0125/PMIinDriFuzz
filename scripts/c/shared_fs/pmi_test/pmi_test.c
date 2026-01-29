#include "../agent_lib/agent_uapi.h"
#include <stdio.h>
#include <fcntl.h> 
#include <linux/random.h>
#include <stdio.h>
#include <sys/syscall.h>

void test_perf_syscall(){
    map_shared_mem();
    int fd = perf_syscall(SYS_open, (long)"/dev/random", O_RDONLY, 0, 0, 0, 0);
    if (fd < 0) printf("syscall error\n");
    int ret = perf_ioctl(fd, RNDGETENTCNT);
    if (ret) printf("ioctl error\n");
    umap_shared_mem();
}

void test_perf_ioctl(){
    map_shared_mem();
    int fd = perf_open("/dev/random", O_RDONLY);
    int ret = perf_ioctl(fd, RNDGETENTCNT);
    if (ret) printf("ioctl error\n");
    umap_shared_mem();
}

int main(){
    // Refer to drivers/char/random.c random_ioctl
    test_perf_syscall();
    return 0;
}
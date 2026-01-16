#include "../perf.h"
#include <stdio.h>
#include <fcntl.h> 
#include <linux/random.h>

int main(){
    // Refer to drivers/char/random.c random_ioctl
    int fd = perf_open("/dev/random", O_RDONLY);
    int ret = perf_ioctl(fd, RNDGETENTCNT);
    return 0;
}
#include "../perf.h"
#include "../shmem.h"
#include <stdio.h>
#include <fcntl.h> 
#include <linux/random.h>
#include <stdio.h>

int main(){
    // Refer to drivers/char/random.c random_ioctl
    map_shared_mem();
    int fd = perf_open("/dev/random", O_RDONLY);
    int ret = perf_ioctl(fd, RNDGETENTCNT);
    if (ret) printf("ioctl error\n");
    umap_shared_mem();
    return 0;
}
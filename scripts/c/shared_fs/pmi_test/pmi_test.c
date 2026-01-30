#include "../agent_lib/agent_uapi.h"
#include <stdio.h>
#include <fcntl.h> 
#include <linux/random.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <linux/i2c-dev.h>

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

// refer to: https://www.kernel.org/doc/Documentation/i2c/dev-interface
void test_bochs_driver(){
    map_shared_mem();
    int fd = perf_open("/dev/i2c-0", O_RDONLY);
    int addr = 0x3ff; /* The I2C address */
    __u8 reg = 0x10; /* Device register to access */
    __s32 res;
    char buf[10];
    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        printf("i2c slave failed\n");
        exit(1);
    }
    buf[0] = reg;
    buf[1] = 0x43;
    buf[2] = 0x65;
    if (write(fd, buf, 3) != 3) {
        /* ERROR HANDLING: i2c transaction failed */
    }

    /* Using I2C Read, equivalent of i2c_smbus_read_byte(file) */
    if (read(fd, buf, 1) != 1) {
        /* ERROR HANDLING: i2c transaction failed */
    } else {
        /* buf[0] contains the read byte */
    }
    umap_shared_mem();
}

int main(){
    // Refer to drivers/char/random.c random_ioctl
    test_bochs_driver();
    return 0;
}
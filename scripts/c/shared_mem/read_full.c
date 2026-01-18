#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#define PERF_MEM_SIZE 1024*1024
#define FLAG_NUM 8
#define DATA_END 0xffffffffffffffff
#define LBR_NUM 32

int main(){
    uint64_t reset_flag, pattern, head, tail, overflow_counter, test, from = 0x1, to = 0x1;
    int lbr_num = 0;
    int fd = open("/dev/shm/perf_record", O_RDONLY);
    if (!fd) {
        perror("fopen");
        return 1;
    }
    read(fd, &reset_flag, sizeof(uint64_t));
    read(fd, &pattern, sizeof(uint64_t));
    read(fd, &head, sizeof(uint64_t));
    read(fd, &tail, sizeof(uint64_t));
    read(fd, &test, sizeof(uint64_t));
    read(fd, &overflow_counter, sizeof(uint64_t));
    read(fd, &overflow_counter, sizeof(uint64_t));
    while(from != DATA_END && (from != 0 || to != 0)){
        read(fd, &from, sizeof(uint64_t));
        read(fd, &to, sizeof(uint64_t));
        printf("Sample %d, LBR %dth entry: from 0x%lx, to 0x%lx\n", lbr_num / LBR_NUM, lbr_num % LBR_NUM, from, to);
        lbr_num++;
    }
}
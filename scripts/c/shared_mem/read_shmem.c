#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

#define PERF_MEM_SIZE 1024*1024
#define FLAG_NUM 8
#define DATA_END 0xffffffffffffffff
#define LBR_NUM 32

struct shared_memory_page {
    int64_t data_head;
    int64_t data_tail;
    int64_t data_offset;
} ;

void* shared_memory = NULL;
struct shared_memory_page* meta_data_page;

int64_t read_shared_mem(){
    // TODO: Consider the data tail.
    char ip_c[64];
    int64_t ip;
    strcpy(ip_c, (char*)((int64_t)shared_memory + meta_data_page->data_tail));
    sscanf(ip_c, "%ld", &ip);
    meta_data_page->data_tail += sizeof(int64_t);
    if (meta_data_page->data_tail > PERF_MEM_SIZE)
        meta_data_page->data_tail = meta_data_page->data_offset;
    return ip;
}

int main(){
    uint64_t reset_flag, pattern, head, tail, overflow_counter, test, from = 0x1, to = 0x1;
    int lbr_num = 0;
    int fd = shm_open("/perf_record", O_RDWR, S_IRUSR | S_IRGRP | S_IROTH);
    if (!fd) {
        perror("fopen");
        return 1;
    }
    shared_memory = mmap(0, PERF_MEM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_memory == MAP_FAILED) {
        printf("mmap pci failed!!\n");
        exit(0);
    }
    meta_data_page = (struct shared_memory_page*)shared_memory;
    meta_data_page->data_tail = meta_data_page->data_offset;
    while(meta_data_page->data_tail < meta_data_page->data_head){
        from = read_shared_mem();
        to = read_shared_mem();
        printf("Sample %d, LBR %dth entry: from 0x%lx, to 0x%lx\n", lbr_num / LBR_NUM, lbr_num % LBR_NUM, from, to);
        lbr_num++;
    }
    close(fd);
}
#include "./agent.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../shared_mem/ivshmem_driver.h"

#define MSG_LEN sizeof(char) * 20

void* shared_memory = NULL;
static struct shared_memory_page* meta_data_page;

void map_shared_mem(){
    struct uio_info_t* info = malloc(sizeof(struct uio_info_t));
    memset(info, 0, sizeof(struct uio_info_t));
    // Assume we only have one uio device.
    info->uio_num = 0;
    shared_memory = uio_mmap(info);
    if (shared_memory == NULL) {
        printf("mmap pci failed!!\n");
        exit(0);
    }
    if (info->maps[1].size < PERF_MEM_SIZE) {
        printf("mapped size lower than expected\n");
        return;
    }
    // Fill struct at init part of shared_memory
    meta_data_page = (struct shared_memory_page*)shared_memory;
    meta_data_page->data_head = sizeof(struct shared_memory_page);
    meta_data_page->data_tail = 0;
    meta_data_page->data_offset = sizeof(struct shared_memory_page);
}

void umap_shared_mem(){
    munmap(shared_memory, PERF_MEM_SIZE);
}

void write_shared_mem(int64_t ip){
    // TODO: Consider the data tail.
    char ip_c[64];
    sprintf(ip_c, "%lld", ip);
    // printf("shm_p: %p, offset_d: %lld, target_c %s, ip_d: %d, ip_c: %s, ip_x: %lx\n", shared_memory, meta_data_page->data_head, (char*)((int64_t)shared_memory + meta_data_page->data_head), ip, ip_c, ip);
    strcpy((char*)((int64_t)shared_memory + meta_data_page->data_head), ip_c);
    meta_data_page->data_head += sizeof(int64_t);
    if (meta_data_page->data_head > PERF_MEM_SIZE)
        meta_data_page->data_head = meta_data_page->data_offset;
}
#include "./agent.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_LEN sizeof(char) * 20

void* shared_memory = NULL;

void map_shared_mem(){
    int fd = open("/sys/devices/pci0000:00/0000:00:04.0/resource2", O_RDWR);
    shared_memory = mmap(0, PERF_MEM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_memory == MAP_FAILED) {
        printf("mmap pci failed!!\n");
        exit(0);
    }
    memset(shared_memory, 0, PERF_MEM_SIZE);
    
}

void umap_shared_mem(){
    munmap(shared_memory, PERF_MEM_SIZE);
}
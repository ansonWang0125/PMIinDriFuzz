#include "./perf.h"
#include "./agent.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* shared_memory;

static void map_shared_mem(){
    shared_memory = mmap((void*)0xfe000000, PERF_MEM_SIZE, PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        printf("mmap pci failed!!\n");
        exit(0);
    }
}

int main(){
    map_shared_mem();
    strcpy((char *)shared_memory, "HELLO WORLD!");
    printf("First line of shared memory is: %s\n", (char *)shared_memory);
    munmap(shared_memory, PERF_MEM_SIZE);
    map_shared_mem();
    printf("First line of shared memory is: %s\n", (char *)shared_memory);
    munmap(shared_memory, PERF_MEM_SIZE);
}
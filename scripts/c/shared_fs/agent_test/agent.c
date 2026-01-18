#include "./agent.h"
#include "./shmem.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_LEN sizeof(char) * 20

int main(){
    void* write_offset = NULL;
    map_shared_mem();
    write_offset = shared_memory.start_addr + PERF_MEM_SIZE - MSG_LEN;
    strcpy((char *)write_offset, "HELLO WORLD!");
    printf("Writing, first line of shared memory is: %s\n", (char *)write_offset);
    munmap(shared_memory.start_addr, PERF_MEM_SIZE);
    map_shared_mem();
    write_offset = shared_memory.start_addr + PERF_MEM_SIZE - MSG_LEN;
    printf("After write, first line of shared memory is: %s\n", (char *)write_offset);
    munmap(shared_memory.start_addr, PERF_MEM_SIZE);
}
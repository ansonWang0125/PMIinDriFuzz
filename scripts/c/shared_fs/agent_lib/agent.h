#include <stdint.h>
#define PERF_MEM_SIZE 1024*1024
#define PAGE_SIZE 4096
#define LBR_NUM 32

struct shared_memory_page {
    int64_t data_head; // Write by producer to tell the write position.
    int64_t data_tail; // Write by consumer to tell the read position. 
    int64_t data_offset; // Base address of data section.
} ;

extern void* shared_memory;
extern void map_shared_mem();
extern void umap_shared_mem();
extern void write_shared_mem(int64_t ip);
#include <stdint.h>
#define PERF_MEM_SIZE 1024*1024
#define PAGE_SIZE 4096
#define LBR_NUM 32
#define DATA_END 0xffffffffffffffff

struct shared_memory_page {
    int64_t data_head;
    int64_t data_tail;
    int64_t data_offset;
};

extern void* shared_memory;
extern void map_shared_mem();
extern void umap_shared_mem();
extern void write_page_shared_mem();
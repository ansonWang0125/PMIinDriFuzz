#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define WRITE_REGION_SIZE (1024 * 1024)
#define EXPECTED_PATTERN 20250818

int main() {
    uint64_t reset_flag, pattern, head, tail, overflow_counter, test, last_element;
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
    lseek(fd, WRITE_REGION_SIZE - 8, SEEK_SET);
    read(fd, &last_element, sizeof(uint64_t));
    
    printf("reset flag: 0x%lx, pattern: 0x%lx\n", reset_flag, pattern);
    printf("head: 0x%lx, tail: 0x%lx\n", head, tail);
    printf("test: %ld, overflow counter: 0x%lx\n", test, overflow_counter);
    printf("last element: 0x%lx\n", last_element);
    return 0;
}


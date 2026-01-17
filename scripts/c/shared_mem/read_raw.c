#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define PERF_MEM_SIZE 1024*1024
#define MSG_LEN sizeof(char) * 20

int main(){
    char* msg = malloc(MSG_LEN);
    int fd = open("/dev/shm/perf_data", O_RDONLY);
    read(fd, msg, MSG_LEN);
    printf("Shared memory msg is: %s\n", msg);
}
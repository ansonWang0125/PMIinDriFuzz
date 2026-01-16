#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main() {
    int fd = open("/dev/shm/perf_record", O_WRONLY | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    uint64_t val = 0x1;

    ssize_t written = write(fd, &val, sizeof(val));
    if (written != sizeof(val)) {
        perror("write");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}


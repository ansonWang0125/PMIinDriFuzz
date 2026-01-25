#include <stdint.h>

long perf_open(const char* pathname, int flags);
long perf_ioctl(int fd, int cmd, ...);
// intptr_t perf_syscall(int, intptr_t, intptr_t, intptr_t, intptr_t, intptr_t, intptr_t);
long perf_syscall(int, long, long, long, long, long, long);
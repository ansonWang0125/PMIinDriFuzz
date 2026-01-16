#define NO_PERF_PTR -1l

int start_nmi_pass(void);

int end_nmi_pass(void);

void lbr_prologue(int nr, ...);

void lbr_epilogue(void);

void lbr_setup(void);

long perf_open(const char* pathname, int flags);

long perf_ioctl(int fd, int cmd, ...);
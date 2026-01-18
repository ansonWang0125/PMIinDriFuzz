CC := gcc
CFLAGS := -Wall
TARGETS := perf.o shmem.o agent

all: $(TARGETS)

perf.o: perf.c perf.h
	$(CC) -c $<

shmem.o: shmem.c shmem.h
	$(CC) -c $<

agent.o: agent.c agent.h
	$(CC) -c $<

agent: agent.o shmem.o perf.o
	$(CC) -o $@ $^

clean:
	rm $(TARGETS)
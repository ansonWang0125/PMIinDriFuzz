CC := gcc
CFLAGS := -Wall
CLEANS := perf.o shmem.o libagent.a

all: libagent.a

libagent.a: shmem.o perf.o
	ar r libagent.a shmem.o perf.o

perf.o: perf.c perf.h
	$(CC) -c $<

shmem.o: shmem.c shmem.h
	$(CC) -c $<

clean:
	rm $(CLEANS)
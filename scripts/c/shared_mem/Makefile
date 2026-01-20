CC := gcc
CFLAGS := -Wall
TARGET := read_shmem

all: $(TARGET)

read_shmem: read_shmem.c
	$(CC) -o $@ $< -lrt

clean:
	rm $(TARGET)
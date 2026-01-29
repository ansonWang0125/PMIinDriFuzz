CC := gcc
CFLAGS := -Wall

all: libivshmem.a

libivshmem.a: ivshmem_driver.o
	ar r libivshmem.a ivshmem_driver.o

ivshmem_driver.o: ivshmem_driver.c ivshmem_driver.h
	$(CC) -c $<

clean:
	rm ivshmem_driver.o libivshmem.a
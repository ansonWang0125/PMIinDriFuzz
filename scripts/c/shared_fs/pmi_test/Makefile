CC := gcc
CFLAGS := -Wall

all: pmi_test

pmi_test: pmi_test.c ../agent_lib/libagent.a ../shared_mem/libivshmem.a
	$(CC) -o pmi_test pmi_test.c -L../agent_lib -l agent -L../shared_mem -l ivshmem

clean:
	rm pmi_test
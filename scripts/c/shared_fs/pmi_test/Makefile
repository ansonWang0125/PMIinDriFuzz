CC := gcc
CFLAGS := -Wall

all: pmi_test

pmi_test: pmi_test.c ../agent_lib/libagent.a
	$(CC) -o pmi_test pmi_test.c -L/storage/PMIinDriFuzz/scripts/c/shared_fs/agent_lib -l agent

clean:
	rm pmi_test
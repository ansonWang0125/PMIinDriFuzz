CC := gcc
CFLAGS := -Wall

all: perf.o agent

perf.o: perf.c agent.h
	$(CC) -c $<

agent: agent.c agent.h
	$(CC) -o $@ $^

clean:
	rm perf.o agent
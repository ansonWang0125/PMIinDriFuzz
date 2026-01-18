CC := gcc
CFLAGS := -Wall
TARGET := pmi_test
OBJECT := $(TARGET).o
SOURCE := $(TARGET).c
HEADER := ../perf.h ../shmem.h

all: $(TARGET)

$(TARGET): $(OBJECT) ../perf.o ../shmem.o
	$(CC) -o $@ $^

OBJECT.o: $(SOURCE) $(HEADER)
	$(CC) -c $<

clean:
	rm $(TARGET)
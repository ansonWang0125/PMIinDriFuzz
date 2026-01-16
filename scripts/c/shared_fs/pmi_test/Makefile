CC := gcc
CFLAGS := -Wall
TARGET := pmi_test
SOURCE := $(TARGET).c
HEADER := ../perf.h

all: $(TARGET)
	
$(TARGET): $(SOURCE) $(HEADER)
	$(CC) -o $@ $<

clean:
	rm $(TARGET)
CC := gcc
CFLAGS := -Wall
TARGET := agent
SOURCE := $(TARGET).c
HEADER := ./perf.h

all: $(TARGET)
	
$(TARGET): $(SOURCE) $(HEADER)
	$(CC) -o $@ $<

clean:
	rm $(TARGET)
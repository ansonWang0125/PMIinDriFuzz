CC := gcc
CFLAGS := -Wall
TARGET := set_reset_flag read_ctrl_flag read_full

all: $(TARGET)
	
set_reset_flag: set_reset_flag.c
	$(CC) -o $@ $<

read_ctrl_flag: read_ctrl_flag.c
	$(CC) -o $@ $<

read_full: read_full.c
	$(CC) -o $@ $<

clean:
	rm $(TARGET)
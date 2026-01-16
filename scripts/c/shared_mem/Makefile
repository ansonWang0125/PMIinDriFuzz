CC := gcc
CFLAGS := -Wall
TARGET := set_reset_flag read_ctrl_flag

all: $(TARGET)
	
set_reset_flag: set_reset_flag.c
	$(CC) -o $@ $<

read_ctrl_flag: read_ctrl_flag.c
	$(CC) -o $@ $<

clean:
	rm $(TARGET)
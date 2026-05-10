# Makefile
# Calendar Management System
#
# Usage:
#   make          -> build the program
#   make clean    -> remove the compiled file

CC     = gcc
CFLAGS = -std=c11 -Wall -g
TARGET = calendar
SRCS   = main.c calendar_functions.c calendar_manager.c

all: $(TARGET)

$(TARGET): $(SRCS) calendar.h
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe events.txt

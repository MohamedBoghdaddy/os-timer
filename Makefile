CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I. -I./threads -I./devices -I./lib
LDFLAGS =

SRC = devices/timer.c threads/thread.c threads/synch.c threads/interrupt.c main.c
OBJ = $(SRC:.c=.o)

TARGET = os_project

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

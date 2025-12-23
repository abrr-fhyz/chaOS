CC = gcc
CFLAGS = -Wall -Wextra -I lib
TARGET = chaos
SRC_DIR = src
LIB_DIR = lib

# Find all .c files in src/ and root
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Pattern rule for .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
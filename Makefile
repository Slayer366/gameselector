# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image

# Source and target
SRC = gameselector.c
TARGET = gameselector

# Build rules
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean up
clean:
	rm -f $(TARGET)

.PHONY: all clean

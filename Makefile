CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lm
LIBS = -lpng -ljpeg   # add any other libs here

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/image-utils

$(TARGET): $(OBJ)
	@mkdir -p build
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/

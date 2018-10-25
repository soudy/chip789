CC?=gcc
CFLAGS=-Wall -Wextra -O2 -std=c11 -g
LIBS=-lSDL2 -lSDL2main

CFILES=$(wildcard *.c)
OBJFILES=$(patsubst %.c,%.o, $(CFILES))

all: build build/chip789

build/chip8: $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

build: $(OBJFILES)
	@mkdir -p build

clean:
	@rm -rf $(OBJFILES) build/

.PHONY: build clean

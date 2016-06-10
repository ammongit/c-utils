.PHONY: all debug force forcedebug clean

CC = gcc
FLAGS = -ansi -Wall -Wextra -pipe -O3

SOURCES = $(wildcard *.c)
TARGETS = $(patsubst %.c,bin/%,$(SOURCES))

all: bin $(TARGETS)

bin:
	@echo '[MKDIR] bin'
	@mkdir -p bin

bin/%: %.c
	@echo '[CC] $(@F)'
	@$(CC) $(FLAGS) $(EXTRA_FLAGS) $< -o $(@F)
	@mv $(@F) -t bin

debug:
	@make EXTRA_FLAGS='-g -Og'

force: clean all

forcedebug: clean debug

clean:
	@echo '[RMDIR] bin'
	@rm -rf bin


# Makefile Demos

export C_INCLUDE_PATH=../lib

DEMOS_SRC = $(wildcard *.c)
DEMOS_EXE = $(DEMOS_SRC:.c=)
CC=gcc

OBJECTS = ../build/ht_helper.o ../build/ht_decode_encode.o

default: $(DEMOS_EXE)

%: %.c
	$(CC) $(OBJECTS) $@.c -Wall $(LIBS) -I ../lib/ -o ../build/$@

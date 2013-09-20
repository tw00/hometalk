# Makefile lib

CC = gcc
CFLAGS = -g -Wall

.PHONY: default clean

OBJECTS = ht_helper.o ht_decode_encode.o ht_circularBuffer.o
HEADERS = $(wildcard *.h)

default: $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o ../build/$@

.PRECIOUS: $(OBJECTS)

clean:
	-rm -f ../build/ht_helper.o
	-rm -f ../build/ht_decode_encode.o
	-rm -f ../build/ht_circularBuffer.o

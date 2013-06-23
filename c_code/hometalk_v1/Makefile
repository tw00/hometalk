# Makefile

export C_INCLUDE_PATH=lib
export LD_LIBRARY_PATH=build

HEADERS = lib/ht_decode_encode.h lib/ht_helper.h lib/ht_common.h
OBJECTS = ht_helper.o ht_decode_encode.o
DEMOS_SRC = $(wildcard demos/*.c)
DEMOS_EXE = $(DEMOS_SRC:.c=)

.PHONY: default clean lib demos

default: demos

lib:
	( cd lib; make )

demos: lib
	( cd demos; make )

clean:
	-rm -f build/ht_test
	( cd lib; make clean )

#$(CC) $(OBJECTS) _Wall $(LIBS) - demos/demo_read_decode.c -Ilib -Lbuild -lht_helper -lht_decode_encode -o build/demo_read_decode
#$(CC) demos/demo_write_encode.c -o build/demo_write_encode
#ht_test: 
#	gcc demos/ht_test.c -o build/ht_test

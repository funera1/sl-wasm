#==========================================
#    Makefile: makefile for sl 5.1
#	Copyright 1993, 1998, 2014
#                 Toyoda Masashi
#		  (mtoyoda@acm.org)
#	Last Modified: 2014/03/31
#==========================================

# CC=gcc
CC=gcc
EMCC=emcc
CFLAGS=-O -Wall

all: sl

sl: sl.c sl.h
	$(CC) $(CFLAGS) -o sl sl.c -lncurses

sl-js: sl.c sl.h
	$(EMCC) $(CFLAGS) -o sl.html sl.c \
		-I/home/funera1/repo/ncurses-6.1/build/include/ \
		-L/home/funera1/repo/ncurses-6.1/build/lib \
		-lncurses

sl-wasi: sl.c sl.h
	$(EMCC) $(CFLAGS) -g -o sl.wasm sl.c \
		-I./ncurses-for-wasm/build/include/ \
		-L./ncurses-for-wasm/build/lib \
		-lncurses

clean:
	rm -f sl sl.wasm sl.html sl.js

distclean: clean

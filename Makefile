
PLATFORM ?= "Linux"

ifeq ($(origin CC),default)
ifeq ($(PLATFORM),"Linux")
CC = gcc
else ifeq ($(PLATFORM),"Windows")
CC = x86_64-w64-mingw32-gcc
endif
endif

CFLAGS ?= -std=c99 -Wpedantic

build/gplgp: build/main.o build/gpl.o
	$(CC) $(CFLAGS) build/main.o build/gpl.o -o build/gplgp

build/main.o: src/main.c src/gpl.h
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/gpl.o: src/gpl.c src/gpl.h src/license.h
	$(CC) $(CFLAGS) -c src/gpl.c -o build/gpl.o


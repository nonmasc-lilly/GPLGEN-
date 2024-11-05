
PLATFORM ?= "Linux"

ifeq ($(origin CC),default)
ifeq ($(PLATFORM),"Linux")
CC = gcc
else ifeq ($(PLATFORM),"Windows")
CC = x86_64-w64-mingw32-gcc
endif
endif

CFLAGS ?= -std=c99 -Wpedantic -Os -s -fdata-sections -ffunction-sections -Wl,--gc-sections

build/gplgp: build/main.o build/gpl.o build/license.o build/config.o
	$(CC) $(CFLAGS) build/main.o build/gpl.o build/license.o build/config.o -o build/gplgp

build/main.o: src/main.c src/gpl.h src/defs.h src/config.h
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/gpl.o: src/gpl.c src/gpl.h
	$(CC) $(CFLAGS) -c src/gpl.c -o build/gpl.o

build/license.o: src/license.c src/gpl.h
	$(CC) $(CFLAGS) -c src/license.c -o build/license.o

build/config.o: src/config.c src/config.h src/gpl.h
	$(CC) $(CFLAGS) -c src/config.c -o build/config.o

ifeq ($(PLATFORM),"Linux")
install: build/gplgp
	cp build/gplgp /usr/bin/gplgp
else
install:
	echo "install unsupported"
endif

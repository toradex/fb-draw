# file    	Makefile
# copyright 	Copyright (c) 2012 Toradex AG
# author  	Max Krummenacher <max.krummenacher@toradex.com>
# brief   	This makefile builds the project fb-draw 

CROSS_COMPILE ?= ~/gcc-linaro/bin/arm-linux-gnueabihf-

CC = $(CROSS_COMPILE)gcc
STRIP ?= $(CROSS_COMPILE)strip

CFLAGS ?= -Wall -g -O2
LDFLAGS ?=

PROG = fb-draw

OBJS = $(PROG).o

all: $(OBJS) 
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS) $(CFLAGS)
	#$(STRIP) $(PROG)

%o: %c 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<
	
clean:
	rm -f $(PROG) $(OBJS) *~ 2>/dev/null

install:
	install -d $(DESTDIR)$(prefix)/bin
	install -m 0755 $(PROG) $(DESTDIR)$(prefix)/bin

.PHONY : all clean install
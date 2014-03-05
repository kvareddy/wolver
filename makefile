.SUFFIXES: .o .c .cc

MAKEFLAGS=-j $(if $(CORES),$(CORES),1)
CC=g++
INCS=
CFLAGS=-W -Wall -Wextra -O3 -std=c++11 -g3 -ggdb
OBJS=
LIBS=
LDEPS=
LIBOBJ= $(addsuffix .o,$(basename $(wildcard wol[a-z]*.cpp)))
SRC=$(shell ls *.h *.cpp)
TARGETS=wolver

all: $(TARGETS)

-include dependencies

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

dependencies: $(SRC) makefile
	rm -f $@; $(CC) $(CFLAGS) -MM $(SRC)|sed -e 's,:,: makefile,'>$@

wolver: wolmain.o libwolver.a
	$(CC) $(CFLAGS) -o $@ wolmain.o -L. -lwolver

libwolver.a: $(LIBOBJ)
	rm -f $@
	ar rc $@ $(LIBOBJ)
	ranlib $@

clean:
	rm -f $(TARGETS)
	rm -f dependencies
	rm -f *.gcda *.gcno *.gcov *.gch *.plist
	rm -f log/*.log *.tmp
	rm -f *.o *.a
	rm -f btormbt btoruntrace

.PHONY: all clean


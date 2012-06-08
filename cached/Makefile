## Makefile

CC=g++
CFLAGS=-I/usr/include -O1 -g -fpermissive
ZMQCFLAGS=-D_REENTRANT -D_GNU_SOURCE -Wall -Wno-unused -fno-strict-aliasing
DEBUGFLAGS=$(CFLAGS) -ggdb -O0
LIBS=-lXpm -lX11 -ljpeg -lfontconfig -lfreetype -lpng12 -lz -lm -lgd -lsqlite3
ZMQLIBS=-lpthread -lm -lrt -lcrypt -lstdc++

DEPS = cached.h Makefile
OBJ = cached.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(ZMQFLAGS)

#debug:
#	$(CC) -c -o cached.o cached.cpp $(DEBUGFLAGS) $(ZMQFLAGS)
#	$(CC) -o cached cached.o -pg -L/usr/lib -L/usr/lib/gcc/i486-linux-gnu/4.3 -Bsymbolic-functions $(LIBS) $(ZMQLIBS)

cached: $(OBJ)
	$(CC) -o $@ $^ -pg -L/usr/lib -L/usr/lib/gcc/i486-linux-gnu/4.3 -Bsymbolic-functions $(LIBS) $(ZMQLIBS)

clean:
	rm *.o

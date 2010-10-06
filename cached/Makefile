## Makefile

CC=g++
CFLAGS=-Wfatal-errors -I/usr/include -O2 -g -pg
DEBUGFLAGS=$(CFLAGS) -Q -ggdb -O0
LIBS=-lXpm -lX11 -ljpeg -lfontconfig -lfreetype -lpng12 -lz -lm -lgd -lsqlite3

DEPS = cached.h Makefile
OBJ = cached.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cached: $(OBJ)
	$(CC) -o $@ $^ -pg -L/usr/lib -L/usr/lib/gcc/i486-linux-gnu/4.3 -Bsymbolic-functions $(LIBS)

clean:
	rm *.o

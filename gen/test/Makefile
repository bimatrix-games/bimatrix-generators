CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-L$(HOME)/local/lib

OBJ=multivariate.o
EXEC=multi

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	
all: multi

multi: multivariate.o
	$(CC) $(CFLAGS) multivariate.o ../matrix.o ../util.o -o multi -lm

clean:
	rm $(OBJ) $(EXEC)

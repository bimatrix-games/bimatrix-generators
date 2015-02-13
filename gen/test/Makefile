CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-L$(HOME)/local/lib

OBJ=multivariate.o partition.o
EXEC=multi partition

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	
all: multi partition

multi: multivariate.o
	$(CC) $(CFLAGS) multivariate.o ../matrix.o ../util.o -o multi -lm

partition: partition.o
	$(CC) $(CFLAGS) partition.o -o partition -lm

clean:
	rm $(OBJ) $(EXEC)

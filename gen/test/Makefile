CC=gcc
CFLAGS=-g -Wall -DDEBUG
LDFLAGS=-L$(HOME)/local/lib

MULTI_OBJ=multivariate.o matrix.o util.o
PART_OBJ=part.o partition.o
OBJ=$(MULTI_OBJ) $(PART_OBJ)
EXEC=multi partition

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	
all: multi partition

matrix.o : ../matrix.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

partition.o : ../partition.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

util.o : ../util.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


multi: $(MULTI_OBJ)
	$(CC) $(CFLAGS) $(MULTI_OBJ) -o multi -lm

partition: $(PART_OBJ)
	$(CC) $(CFLAGS) $(PART_OBJ) -o partition -lm

clean:
	rm $(OBJ) $(EXEC)

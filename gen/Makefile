CC=gcc
CFLAGS=-g -Wall
LDFLAGS=-L$(HOME)/local/lib

OBJ=matrix.o bm-gen.o tournament.o ranking.o util.o unit_games.o partition.o
EXEC=bm-gen

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
	
all: $(OBJ) 
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(EXEC) -lgmp -lm

clean:
	rm $(OBJ) $(EXEC)

CC=gcc
CFLAGS=-g -Wall -pthread
SRC=$(wildcard *.c)
HEADER=$(wildcard *.h)
OBJ=$(SRC:.c=.o)
EXEC=lecteur1 ecrivain1 lecteur2 ecrivain2 julia

all: $(EXEC)
lecteur1: conduct.c lecteur1.c $(HEADER) 
	$(CC) $(CFLAGS) -o $@ $^
ecrivain1: conduct.c ecrivain1.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
lecteur2: conduct.c lecteur2.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
ecrivain2: conduct.c ecrivain2.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
julia: conduct.c julia.c $(HEADER)
	$(CC) -o3 -ffast-math $(FLAGS) `pkg-config --cflags gtk+-3.0` -o $@ $^ `	pkg-config --libs gtk+-3.0` -lm

%.o: %.c
	$(CC) $(CFLAGS) $(CLIBS) -o $@ -c $<

clean:
	rm -f *.o core
rmproper:clean
	rm -f $(EXEC)


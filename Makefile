CC=gcc
CFLAGS=-g -Wall -pthread
CC_RAS=arm-linux-gnueabi-gcc
SRC=$(wildcard *.c)
HEADER=$(wildcard *.h)
OBJ=$(SRC:.c=.o)
EXEC=lecteur1 ecrivain1 lecteur2 ecrivain2 tube_reader tube_writer socket_serveur socket_client test_conduct_read test_conduct_write julia julia_raspberry

all: $(EXEC)
lecteur1: conduct.c lecteur1.c $(HEADER) 
	$(CC) $(CFLAGS) -o $@ $^
ecrivain1: conduct.c ecrivain1.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
lecteur2: conduct.c lecteur2.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
ecrivain2: conduct.c ecrivain2.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
tube_reader: tube_reader.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
tube_writer: tube_writer.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
socket_serveur: socket_serveur.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
socket_client: socket_client.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
test_conduct_read: conduct.c test_conduct_read.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
test_conduct_write: conduct.c test_conduct_write.c $(HEADER)
	$(CC) $(CFLAGS) -o $@ $^
julia: conduct.c julia.c $(HEADER)
	$(CC) -o3 -ffast-math $(FLAGS) `pkg-config --cflags gtk+-3.0` -o $@ $^ `pkg-config --libs gtk+-3.0` -lm
julia_raspberry: conduct.c julia.c $(HEADER)
	$(CC_RAS) -o3 -ffast-math $(FLAGS) `pkg-config --cflags gtk+-3.0` -o $@ $^ `pkg-config --libs gtk+-3.0` -lm
%.o: %.c
	$(CC) $(CFLAGS) $(CLIBS) -o $@ -c $<

clean:
	rm -f *.o core
rmproper:clean
	rm -f $(EXEC)


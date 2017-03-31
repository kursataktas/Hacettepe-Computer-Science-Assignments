CC = gcc
CFLAGS = -c -Wall

all: updatedatabase locater

updatedatabase: updatedatabase.o functions.o
	$(CC) updatedatabase.o functions.o -o updatedatabase

locater: locater.o functions.o
	$(CC) locater.o functions.o  -o locater

locater.o: locater.c
	$(CC) $(CFLAGS) locater.c

updatedatabase.o: updatedatabase.c
	$(CC) $(CFLAGS) updatedatabase.c

functions.o: functions.c
	$(CC) $(CFLAGS) functions.c

clean:
	rm -rf *o updatedatabase
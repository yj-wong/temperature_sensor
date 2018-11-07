CC = gcc
CFLAGS = -lWarn -pedantic

sensor: sensor.o libmyifttt.a
				cc sensor.o -L. -lmyifttt -lcurl -lm -o sensor

sensor.o: sensor.c ifttt.h
				$(CC) $(CFLAGS) -c -ansi $<

libmyifttt.a:	ifttt.o
	ar -rcs libmyifttt.a ifttt.o

ifttt.o: 	ifttt.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

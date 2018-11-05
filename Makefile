CC = gcc
CFLAGS = -lWarn -pedantic

sensor: sensor.o
				cc sensor.o -o sensor

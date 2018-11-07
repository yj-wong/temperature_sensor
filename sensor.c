#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ifttt.h"

int readSensor(char dir[]) {
	FILE *fd;
	int n;
	char buf[101];
	int t = 0;
	int i;

	fd = fopen(dir, "r");

	if (fd  == (FILE *)NULL) {
 		perror("Temperature sensor cannot be found");
		(void) exit(1);
 	}
	n = fread(buf, 1, 100, fd);
 	if(n == 0) {
 		perror("read fails!");
 		exit(1);
 	}
 	buf[n] = '\0';
	/*
 	fprintf(stdout, "Read '%s'\n", buf);
	*/
	for (i = 0; i < 5; i++) {
		char nChar = buf[n-6 + i];
		int num = nChar - '0';
		num *= pow(10, 4-i);
		t += num;
	}

 	(void) fclose(fd);

	return t;
}

void printCelcius(int t) {
	double	celcius = (double) t / 1000;
	printf("%.3f Celcius\n", celcius);
}

int main(int argc, char *argv[]) {

	int tOld, t, highest, lowest;
	char *t_ptr[10], *h_ptr[10], l_ptr[10];
	char dir[100] = "/sys/bus/w1/devices/";
	char dir2[] = "/w1_slave";

	if (argc != 2) {
		printf("Usage: program sensor_serial_number\n");
		return 0;
	}

	strcat(dir, argv[1]);
	strcat(dir, dir2);

	tOld = readSensor(dir);
	highest = tOld;
	lowest = tOld;
	printf("Initial temperature reading: ");
	printCelcius(tOld);
	printf("Sending email via ifttt server.\n");

	while (1) {
		t = readSensor(dir);
		if (t > highest) {
			highest = t;
			printCelcius(t);
		} else if (t < lowest) {
			lowest = t;
			printCelcius(t);
		} else {
			printCelcius(t);
		}

		if (t - tOld > 1000 || t - tOld < -1000) {
			tOld = t;
			printf("Temperature change exceeds 1 Celcius since last message.\n");
			printf("Current temperature: ");
			printCelcius(t);
			printf("Highest temperature: ");
			printCelcius(highest);
			printf("Lowest temperature: ");
			printCelcius(lowest);
			printf("Sending email via ifttt server.\n");
		}

		}


	return 0;
}

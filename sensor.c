#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ifttt.h"

char *setDir(char serial[], char dir[]) {
}

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

char *statement(int t, int n, char s[]) {
	double celcius = (double) t / 1000;

	if (n == 0) {
		sprintf(s, "Initial temperature: %.3f Celcius", celcius);
	} else if (n == 1) {
		sprintf(s, "Current temperature: %.3f Celcius", celcius);
	} else	if (n == 2) {
		sprintf(s, "Highest temperature: %.3f Celcius", celcius);
	} else if (n == 3) {
		sprintf(s, "Lowest temperature: %.3f Celcius", celcius);
	}

	return s;
}

int initialTemp(char dir[]) {
	int tInit = readSensor(dir);
	char i_s[50];

	printf("%s\n",statement(tInit, 0, i_s));
	printf("Sending email via ifttt server.\n");
	ifttt("https://maker.ifttt.com/trigger/temperature_sensor/with/key/x9XnvGbgm0kq1KFLFBCyZ", statement(tInit, 0, i_s) , "", "");

	return tInit;
}

void outputTemp(char dir[]) {
	int tOld, t, highest, lowest;
	char t_s[50], h_s[50], l_s[50];

	tOld = initialTemp(dir);
	highest = tOld;
	lowest = tOld;

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
			printf("%s\n%s\n%s\n", statement(t, 1, t_s), statement(highest, 2, h_s), statement(lowest, 3, l_s));
			printf("Sending email via ifttt server.\n");
			ifttt("https://maker.ifttt.com/trigger/temperature_sensor/with/key/x9XnvGbgm0kq1KFLFBCyZ", statement(t, 1, t_s) , statement(highest, 2, h_s), statement(lowest, 3, l_s));
		}

	}
}

int main(int argc, char *argv[]) {
	char dir[100] = "/sys/bus/w1/devices/";
	char dir2[] = "/w1_slave";
	char *t_ptr[10], *h_ptr[10], l_ptr[10];

	if (argc != 2) {
		printf("Usage: program sensor_serial_number\n");
		return 0;
	}

	strcat(dir, argv[1]);
	strcat(dir, dir2);

	outputTemp(dir);

	return 0;
}

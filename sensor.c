#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ifttt.h"

int readSensor(char dir[]) {
	FILE *fd;
	int n;
	char buf[101];
	int t = 0;
	int i;

	char dir1[] = "/sys/bus/w1/devices/28-03184177f1ff/w1_slave";
	printf("%s\n", dir);
	printf("%s\n", dir1);
	printf("%d %d\n", strlen(dir), strlen(dir1));
	printf("%d\n", (strncmp(dir, dir1, 0)));

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

double showCelcius(int t) {
	double	celcius = (double) t / 1000;
	printf("%d\n%.3f\n", t, celcius);

	return celcius;
}

int main(int argc, char *argv[]) {

	int tOld;
	int t;
	int highest;
	int lowest;
	char dir[100] = "/sys/bus/w1/devices/";
	char dir2[] = "/w1_slave";

	/*strcat(dir, "/sys/bus/w1/devices/");*/

	if (argc != 2) {
		printf("Usage: program sensor_serial_number\n");
		return 0;
	}

	strcat(dir, argv[1]);
	strcat(dir, dir2);

	t = readSensor(dir);
	ifttt("https://maker.ifttt.com/trigger/celcius_change/with/key/x9XnvGbgm0kq1KFLFBCyZ", "temperature", "1st", "hi");
	printf("Sending email via ifttt server.\n");
	highest = t;
	lowest = t;

	while (1) {
		tOld = t;
		t = readSensor(dir);
		showCelcius(t);

		if (t > highest) {
			highest = t;
		} else if (t < lowest) {
			lowest = t;
		}

		if (t - tOld > 1000 || t - tOld < 1000) {
			ifttt("https://maker.ifttt.com/trigger/celcius_change/with/key/x9XnvGbgm0kq1KFLFBCyZ", "", "", "");
			printf("Sending email via ifttt server.\n");
		}

		sleep(3);
	}


	return 0;
}

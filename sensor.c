#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	FILE *fd;
	int n;
	char buf[101];

	if((fd = fopen("test.txt", "r")) == (FILE *)NULL) {
 		perror("open of test.txt failed");
 		(void) exit(1);
 	}
	n = fread(buf, 1, 100, fd);
 	if(n == 0) {
 		perror("read fails!");
 		exit(1);
 	}
 	buf[n] = '\0';
 	fprintf(stdout, "Read '%s'\n", buf);
 	(void) fclose(fd);

	return 0;
}

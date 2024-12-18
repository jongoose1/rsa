#include <stdio.h>
#include <stdlib.h>
#include "../src/rsa.h"
#include <fcntl.h>
#include <unistd.h>

extern int dr;

int main(int argc, char ** argv) {

	dr = open("/dev/random", O_RDONLY);

	bignum a = bignum_random();
	bignum_print(&a);

	close(dr);

	return 0;
}

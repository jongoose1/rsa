#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/rsa.h"

/* Try 221 is composite with 137 as a witness. */

int main(int argc, char ** argv) {

	if (argc != 3) {
		printf("Need two numbers...\n");
		return 1;
	}

	bignum a = bignum_small(atoi(argv[1]));
	bignum b = bignum_small(atoi(argv[2]));

	int r = miller_rabin(&a, &b);
	if (r < 0)
		printf("Invalid input for Miller-Rabin test.\n");
	else if (r == 1)
		printf("%d is probably prime.\n", atoi(argv[1]));
	else
		printf("%d is a Miller-Rabin witness for the compositeness of %d.\n", atoi(argv[2]), atoi(argv[1]));

	return 0;
}

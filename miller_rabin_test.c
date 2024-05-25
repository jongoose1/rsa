#include <stdint.h>
#include <stdio.h>
#include "rsa.h"

#define CANDIDATE 221
#define WITNESS 137


int main(void) {
	bignum n = bignum_small(CANDIDATE);
	bignum a = bignum_small(WITNESS);

	int r = miller_rabin(&n, &a);
	if (r == -1)
		printf("Invalid input for Miller-Rabin test.\n");
	else if (r == 1)
		printf("%d is probably prime.\n", CANDIDATE);
	else
		printf("%d is a Miller-Rabin witness for the compositeness of %d.\n", WITNESS, CANDIDATE);

	return 0;
}

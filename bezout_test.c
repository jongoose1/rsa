#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"

/* Try 240 and 46. -9*240 + 47*46 = 2 */

int main(int argc, char ** argv) {

	if (argc != 3) {
		printf("Need two numbers...\n");
		return 1;
	}

	bignum a = bignum_small(atoi(argv[1]));
	bignum b = bignum_small(atoi(argv[2]));
	bignum x, y;

	bezout_coefficients(&a, &b, &x, &y);
	bignum_print(&x);
	bignum_print(&y);

	return 0;
}

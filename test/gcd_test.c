#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/rsa.h"

/* Try gcd(4444, 888811) = 11 */

int main(int argc, char ** argv) {

	if (argc != 3) {
		printf("Need two numbers...\n");
		return 1;
	}

	bignum a = bignum_small(atoi(argv[1]));
	bignum b = bignum_small(atoi(argv[2]));

	bignum c = bignum_gcd(&a, &b);
	bignum_print(&c);

	return 0;
}

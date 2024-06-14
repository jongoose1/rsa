#include <stdio.h>
#include <stdlib.h>
#include "../src/rsa.h"

/* Try 99 % 5 = 4 */

int main(int argc, char ** argv) {

	if (argc != 3) {
		printf("Need two numbers...\n");
		return 1;
	}

	bignum a = bignum_small(atoi(argv[1]));
	bignum b = bignum_small(atoi(argv[2]));

	bignum c = bignum_mod(&a, &b);
	bignum_print(&c);

	return 0;
}

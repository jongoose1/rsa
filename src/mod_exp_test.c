#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "rsa.h"

/* Try 421^1041243 mod 1092 = 1009 (0x3F1) */

int main(int argc, char ** argv) {

	if (argc != 4) {
		printf("Need three numbers...\n");
		return 1;
	}

	bignum b = bignum_small(atoi(argv[1]));
	bignum e = bignum_small(atoi(argv[2]));
	bignum m = bignum_small(atoi(argv[3]));

	bignum c = bignum_mod_exp(&b, &e, &m);
	bignum_print(&c);
	return 0;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"

int main(void) {
	srand(time(NULL));

	bignum a = bignum_zero();
	bignum b = bignum_zero();
	a.a[0] = 99;
	b.a[0] = 5;

	bignum c = bignum_mod(&a, &b);
	bignum_print(&c);

	c = random_large_probable_prime(1);
	bignum_print(&c);
	return 0;
}

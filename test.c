#include <stdint.h>
#include <stdio.h>
#include "rsa.h"
int main(void) {
	bignum a = bignum_zero();
	bignum b = bignum_zero();
	a.a[0] = 99;
	b.a[0] = 5;

	bignum c = bignum_mod(&a, &b);
	bignum_print(&c);
	return 0;
}

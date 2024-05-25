#include <stdint.h>
#include <stdio.h>
#include "rsa.h"
int main(void) {
	bignum b = bignum_zero();
	bignum e = bignum_zero();
	bignum m = bignum_zero();
	b.a[0] = 421;
	e.a[0] = 1041243;
	m.a[0] = 1092;

	bignum c = bignum_mod_exp(&b, &e, &m);
	bignum_print(&c);
	return 0;
}

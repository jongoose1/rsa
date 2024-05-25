#include <stdint.h>
#include <stdio.h>
#include "rsa.h"
int main(void) {
	bignum * b = new_bignum();
	bignum * e = new_bignum();
	bignum * m = new_bignum();
	b->a[0] = 421;
	e->a[0] = 1041243;
	m->a[0] = 1092;

	bignum * c = mod_exp(b, e, m);
	print_bignum(c);
	return 0;
}

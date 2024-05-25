#include <stdint.h>
#include <stdio.h>
#include "rsa.h"
int main(void) {
	bignum * a = new_bignum();
	bignum * b = new_bignum();
	a->a[0] = 99;
	b->a[0] = 5;

	bignum * c = mod(a, b);
	print_bignum(c);
	return 0;
}

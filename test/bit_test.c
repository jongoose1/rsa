#include <stdio.h>
#include "../src/rsa.h"

int last_one_bit (bignum const *);

int main() {
	bignum a = bignum_zero();
	a.a[0] = 0x00023199;
	printf("LAST ONE BIT = %d", last_one_bit(&a));
	return 0;
}

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../src/rsa.h"

int main() {
	srand(time(0));
	bignum a = bignum_half_random();
	bignum b;
	clock_t start = clock();
	bignum_mul(&a, &a);
	printf("MUL: %ld\n", clock() - start);
	start = clock();
	b = a;
	printf("ASSIGN: %ld\n", clock() - start);
	a = b;
	return 0;
}

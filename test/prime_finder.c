#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/rsa.h"

int main(void) {
	srand(time(0));
	bignum c = random_large_probable_prime(1);
	bignum_print(&c);
	return 0;
}

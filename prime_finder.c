#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"

int main(void) {
	srand(time(NULL));
	bignum c = random_large_probable_prime(1);
	bignum_print(&c);
	return 0;
}

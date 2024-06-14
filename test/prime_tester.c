#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/rsa.h"
#include <time.h>

/* Try 9737333 is prime and fails 10 composite tests. */

int main(int argc, char ** argv) {
	srand(time(0));

	if (argc != 3) {
		printf("Need two numbers...\n");
		return 1;
	}

	bignum a = bignum_small(atoi(argv[1]));
	int n = atoi(argv[2]);

	int tests_failed = 0, r;
	while (tests_failed < n) {
		bignum witness = bignum_random();
		r = miller_rabin(&a, &witness);
		if (r < 0)
			printf("Invalid input for Miller-Rabin test.\n");
		else if (r == 1) {
			printf("%d is probably prime.\n", atoi(argv[1]));
			tests_failed++;
		} else {
			bignum_print(&witness);
			printf("is a Miller-Rabin witness for the compositeness of %d.\n", atoi(argv[1]));
			break;
		}
	}
	return 0;
}

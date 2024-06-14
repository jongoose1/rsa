#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/rsa.h"
#include <time.h>

/* Try starting at 48 with 2 tests.. */

int main(int argc, char ** argv) {
	srand(time(0));

	if (argc != 3) {
		printf("Need two numbers...\n");
		return 1;
	}

	bignum candidate = bignum_small(atoi(argv[1]));
	int n = atoi(argv[2]);

	int tests_failed = 0, r;
	while (tests_failed < n) {
		tests_failed = 0;
		while (tests_failed < n) {
			bignum witness = bignum_random();
			r = miller_rabin(&candidate, &witness);
			if (r < 0)
				printf("Invalid input for Miller-Rabin test.\n");
			else if (r == 1) {
				bignum_print(&candidate);
				printf("is probably prime.\n");
				tests_failed++;
			} else {
				bignum_print(&witness);
				printf("is a Miller-Rabin witness for the compositeness of:\n");
				bignum_print(&candidate);
				break;
			}
		}
		candidate.a[0]++;
	}
	return 0;
}

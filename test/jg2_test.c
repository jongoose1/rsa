#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/rsa.h"

int bit_shift_right(bignum * a) {
	if (!a) return -1;
	int i;
	int r = a->a[0] & 1;
	for(i = 0; i < NWORDS-1; i++) {
		a->a[i] = (a->a[i] >> 1) + (a->a[i+1]<<31);
	}
	a->a[NWORDS-1] = a->a[NWORDS-1] >> 1;
	return r;
}

int main(int argc, char ** argv) {
	srand(time(0));

	if (argc != 2) {
		printf("Need an input...\n");
		return 1;
	}

	int n = strlen(argv[1]);

	keypair kp;
	keypair_load(&kp, "keys/625.kp");

	bignum hash1;
	hash1 = jg2(argv[1],n,&kp.pk);
	bignum_print(&hash1);

	/*change one bit and compare*/
	int d = rand() % n;
	int e = rand() % 8;
	argv[1][d] = argv[1][d] ^ (1 << e);

	bignum hash2;
	hash2 = jg2(argv[1],n,&kp.pk);
	bignum_print(&hash2);

	bignum diff = bignum_xor(&hash1, &hash2);
	bignum_print(&diff);

	int i;
	int zeros = 0;
	for (i = 0; i < NWORDS*16; i++){
		if (diff.a[0]%2 == 0) zeros++;
		bit_shift_right(&diff);
	}
	printf("same bits = %d\n", zeros);
	return 0;
}

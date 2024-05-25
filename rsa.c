#include <stdlib.h>
#include <stdio.h>
#include "rsa.h"

int bignum_print(bignum const *a) {
	if (!a) return 1;
	int i, j, k;
	k = 127;
	for(i = 0; i < 16; i++ ) {
		for(j = 0; j < 8; j++) {
			printf("%8x ",a->a[k--]);
		}
		printf("\n");
	}
	return 0;
}

bignum bignum_zero(void) {
	bignum r;
	int i;
	for (i = 0; i < 128; i++) r.a[i] = 0;
	return r;
}

bignum bignum_mul(bignum const *p, bignum const *q) {
	int i, j;
	u32 L, M;
	u64 x;
	bignum r;
	
	r = bignum_zero();

	if (!p || !q || bignum_is_zero(p) || bignum_is_zero(q)) return r;
	if (bignum_is_one(p)) return *q;
	if (bignum_is_one(q)) return *p;

	for(i = 0; i < 128; i++) {
		for(j = 0; j < 128; j++) {
			if(i + j > 127) continue;
			x = (u64) p->a[i] * q->a[j];
			L = x;
			M = x >> 32;
			r.a[i + j] = r.a[i + j] + L;
			if (i + j + 1 <= 127) r.a[i + j + 1] = r.a[i + j + 1] + M;
		}
	}
	return r;
}

int bignum_is_eq(bignum const *a, bignum const *b) {
	if(!a || !b) return 0;
	int i;
	for(i = 0; i < 128; i++) if (a->a[i] != b->a[i]) return 0;
	return 1;
}

int bignum_is_lt(bignum const *l, bignum const *r) {
	if(!l || !r) return 0;
	int i;
	for(i = 127; i >= 0; i--) {
		if(l->a[i] < r->a[i]) return 1;
		if(l->a[i] > r->a[i]) return 0;
	}
	return 0;
}

int bignum_is_lte(bignum const *l, bignum const *r) {
	if(!l || !r) return 0;
	int i;
	for(i = 127; i >= 0; i--) {
		if(l->a[i] < r->a[i]) return 1;
		if(l->a[i] > r->a[i]) return 0;
	}
	return 1;
}

int bignum_is_gt(bignum const *l, bignum const *r) {
	if(!l || !r) return 0;
	int i;
	for(i = 127; i >= 0; i--) {
		if(l->a[i] < r->a[i]) return 0;
		if(l->a[i] > r->a[i]) return 1;
	}
	return 0;
}

int bignum_is_gte(bignum const *l, bignum const *r) {
	if(!l || !r) return 0;
	int i;
	for(i = 127; i >= 0; i--) {
		if(l->a[i] < r->a[i]) return 0;
		if(l->a[i] > r->a[i]) return 1;
	}
	return 1;
}

int bignum_is_zero(bignum const *a) {
	if(!a) return 0;
	int i;
	for(i = 0; i<128; i++) if(a->a[i] != 0) return 0;
	return 1;
}

int bignum_is_one(bignum const *a) {
	if(!a) return 0;
	int i;
	if(a->a[0] != 1) return 0;
	for(i = 1; i<128; i++) if(a->a[i] != 0) return 0;
	return 1;
}

int bignum_is_even(bignum const *a) {
	return a->a[0] % 2 == 0;
}

int bignum_is_odd(bignum const *a) {
	return a->a[0] % 2 == 1;
}

int bignum_bit_shift_left(bignum * a) {
	if (!a) return -1;
	int i;
	int r = a->a[127] >> 31;
	for(i = 127; i >= 1; i--) {
		a->a[i] = (a->a[i] << 1) + (a->a[i-1]>>31);
	}
	a->a[0] = a->a[0] << 1;
	return r;
}

int bignum_bit_shift_right(bignum * a) {
	if (!a) return -1;
	int i;
	int r = a->a[0] & 1;
	for(i = 0; i < 127; i++) {
		a->a[i] = (a->a[i] >> 1) + (a->a[i+1]<<31);
	}
	a->a[127] = a->a[127] >> 1;
	return r;
}

int bignum_sub(bignum * a, bignum const *b) {
	/* a = a - b */
	if(!a || !b) return 1;
	if(bignum_is_gt(b, a)) return 1;

	int i;
	u64 c;
	for(i = 0; i < 128; i++) {
		if (a->a[i] >= b->a[i]) {
			a->a[i] = a->a[i] - b->a[i];
		} else {
			/* need to borrow */
			a->a[i + 1] = a->a[i + 1] - 1;
			c = a->a[i] + 0x100000000;
			a->a[i] = c - b->a[i];
		}
	}

	return 0;
}

bignum bignum_mod(bignum const *a, bignum const *m) {
	/* a = qm + r */
	if (!a || !m || bignum_is_eq(a, m) || bignum_is_zero(m)) return bignum_zero();
	if(bignum_is_lt(a, m)) return *a;

	bignum r, scratch;
	r = bignum_zero();
	scratch = *a;

	/* long division */
	int i;
	for(i = 0; i < 4096; i++) {
		bignum_bit_shift_left(&r);
		r.a[0] = r.a[0] + bignum_bit_shift_left(&scratch);
		if(bignum_is_gte(&r,m)) bignum_sub(&r, m);
	}
	return r;
}

int bignum_reduce(bignum *a, bignum const *m) {
	/* a = qm + r */
	if (!a || !m || bignum_is_zero(m)) return 1;
	if (bignum_is_lt(a, m)) return 0;
	if (bignum_is_eq(a, m)) {
		*a = bignum_zero();
		return 0;
	}

	bignum scratch;
	scratch = *a;
	*a = bignum_zero();

	/* long division */
	int i;
	for(i = 0; i < 4096; i++) {
		bignum_bit_shift_left(a);
		a->a[0] = a->a[0] + bignum_bit_shift_left(&scratch);
		if(bignum_is_gte(a,m)) bignum_sub(a, m);
	}

	return 0;
}

bignum bignum_mod_exp(bignum const *b, bignum const *e, bignum const *m) {
	/* b^e mod m */
	if (bignum_is_zero(e)) return bignum_zero();

	int i;
	bignum r, scratch;

	r = bignum_zero();
	r.a[0] = 1;
	scratch = *b;

	for(i = 0; i < 4096; i++) {
		if (e->a[i/32] & (1 << (i%32))) {
			r = bignum_mul(&r, &scratch);
			bignum_reduce(&r, m);
		}
		scratch = bignum_mul(&scratch, &scratch);
		bignum_reduce(&scratch, m);
	}
	return r;
}

bignum bignum_small(u32 x) {
	bignum r = bignum_zero();
	r.a[0] = x;
	return r;
}

int miller_rabin(bignum const * n, bignum const * a) {
	/* return 1: test fails, probably prime. */
	/* return 0: n composite. */
	/* return -1: invalid n */
	/* return -2: invalid a */

	bignum const three = bignum_small(3);
	bignum const one = bignum_small(1);
	
	bignum b = bignum_mod(a, n);

	/* Input invalid if n|a or n<3. */
	if (bignum_is_lt(n, &three)) return -1;
	if (bignum_is_zero(&b)) return -2;
	
	/* Write n-1 = q*2^k, with q odd */
	bignum q = *n;
	bignum_sub(&q, &one);
	const bignum negative_one = q;
	int k = 0;
	while (bignum_is_even(&q)) {
		bignum_bit_shift_right(&q);
		k++;
	}

	b = bignum_mod_exp(&b, &q, n);
	if (bignum_is_one(&b)) return 1;

	int i;
	for (i = 0; i < k; i++) {
		if (bignum_is_eq(&b, &negative_one)) return 1;
		b = bignum_mul(&b, &b);
		bignum_reduce(&b, n);
	}

	return 0;
}

bignum bignum_random(void) {
	bignum r;
	int i;
	for (i = 0; i < 128; i++) r.a[i] = rand();
	return r;
}

bignum bignum_half_random(void) {
	bignum r;
	int i;
	for (i = 0; i < 64; i++) r.a[i] = rand();
	return r;
}

/* 
** Generate random 2048 bit probable prime by performing n Miller-Rabin tests
** with random bases.
*/
bignum random_large_probable_prime(int n) {
	bignum witness, candidate;
	/* Note: Failing a Miller-Rabin test indicates a candidate is probably prime. */
	int tests_failed;
	do {
		candidate = bignum_half_random();
		for (tests_failed = 0; tests_failed < n; /* Do nothing. */) {
			witness = bignum_random();
			int result = miller_rabin(&candidate, &witness);
			if (result == -1 || result == 0) { 
				/* Invalid candidate or candidate is composite. */
				if (result == 0) { 
					bignum_print(&witness);
					printf("is a Miller-Rabin witness for the compositeness of\n");
					bignum_print(&candidate);
					printf("\n");
				}

				break;
			} else if (result == 1) {
				/* Test Failed. Candidate is probably prime. */
				tests_failed++;

				bignum_print(&candidate);
				printf("is probabely prime.\n");

			}
		}
	} while (tests_failed < n);
	return candidate;
}

int keygen(void) {
	bignum p, q, n;
	
	/* Choose two large prime numbers p and q. */
	p = random_large_probable_prime(10);
	q = random_large_probable_prime(10);

	/* Compute n = pq. */
	n = bignum_mul(&p, &q);

	/* Compute lambda(n). */

}

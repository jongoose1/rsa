#include <stdlib.h>
#include <stdio.h>
#include "rsa.h"

int bignum_print(bignum const *a) {
	if (!a) return 1;
	int i, j, k;
	k = 127;
	printf("Sign:  %d\n", a->sign?1:0);
	for(i = 0; i < 16; i++ ) {
		for(j = 0; j < 8; j++) {
			printf("%08x ",a->a[k--]);
		}
		printf("\n");
	}
	return 0;
}

bignum bignum_zero(void) {
	bignum r;
	int i;
	for (i = 0; i < 128; i++) r.a[i] = 0;
	r.sign = 0;
	return r;
}

int bignum_inplace_add(bignum * b, int i, u32 u) {
	/* b = b + u*2^(32*i) */
	u64 r;
	while(i < 128 && u) {
		r = (u64) b->a[i] + u;
		b->a[i] = r;
		u = r >> 32;
		i++;
	}
	return u;
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
			if(i + j > 127 || p->a[i] == 0 || q->a[j] == 0) continue;
			x = (u64) p->a[i] * q->a[j];
			L = x;
			M = x >> 32;
			bignum_inplace_add(&r, i+j, L);
			bignum_inplace_add(&r, i+j+1, M);
		}
	}
	r.sign = (p->sign != q->sign);
	return r;
}

int bignum_is_zero(bignum const *a) {
	/* -0 == 0 */
	if(!a) return 0;
	int i;
	for(i = 0; i<128; i++) if(a->a[i] != 0) return 0;
	return 1;
}

int bignum_is_one(bignum const *a) {
	/* -1 != 1 */
	if(!a) return 0;
	int i;
	if(a->a[0] != 1) return 0;
	for(i = 1; i<128; i++) if(a->a[i] != 0) return 0;
	if (a->sign) return 0;
	return 1;
}

int bignum_is_eq(bignum const *a, bignum const *b) {
	/* -0 == 0 */
	if (!a || !b) return 0;
	if (bignum_is_zero(a) && bignum_is_zero(b)) return 1;
	int i;
	for (i = 0; i < 128; i++) if (a->a[i] != b->a[i]) return 0;
	return (a->sign == b->sign);
}

int bignum_is_gt(bignum const *l, bignum const *r) {
	/* -0 !> 0 */
	if(!l || !r) return 0;
	if (bignum_is_eq(l, r)) return 0;
	if (l->sign && !r->sign) return 0;
	if (!l->sign && r->sign) return 1;
	
	/* Same sign. */
	int i;
	for(i = 127; i >= 0; i--) {
		if(l->a[i] < r->a[i]) return l->sign;
		if(l->a[i] > r->a[i]) return !l->sign;
	}
	return l->sign;
}

int bignum_is_gte(bignum const *l, bignum const *r) {
	return bignum_is_gt(l, r) || bignum_is_eq(l, r);
}
int bignum_is_lt(bignum const *l, bignum const *r) {
	return !bignum_is_gte(l, r);
}

int bignum_is_lte(bignum const *l, bignum const *r) {
	return !bignum_is_gt(l, r);
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


/* Helper. Like inplace sub, but not inplace. */
bignum simple_sub(bignum const *a, bignum const *b, int sign) {
	/* r = |a| - |b| */
	/* sign(r) = sign */
	/* prereq: |a| >= |b| */
	if(!a || !b) return bignum_zero();

	int i;
	u64 c;
	bignum r = *a;
	for(i = 0; i < 128; i++) {
		if (r.a[i] >= b->a[i]) {
			r.a[i] = r.a[i] - b->a[i];
		} else {
			/* need to borrow */
			r.a[i + 1] = r.a[i + 1] - 1;
			c = r.a[i] + 0x100000000;
			r.a[i] = c - b->a[i];
		}
	}
	r.sign = sign;
	return r;
}

bignum simple_add(bignum const *a, bignum const *b, int sign) {
	/* r = |a| + |b| */
	/* sign(r) = sign */
	if (!a || !b) return bignum_zero();

	int i;
	u64 c;
	u32 M, L;
	bignum r = bignum_zero();
	for (i = 0; i < 128; i++) {
		c = a->a[i] + b->a[i];
		L = c;
		M = c >> 32;
		r.a[i] = r.a[i] + L;
		if (i < 127) r.a[i+1] = r.a[i+1] + M;
	}
	r.sign = sign;
	return r;
}

bignum bignum_sub(bignum const *a, bignum const *b) {
	/* r = a - b */
	if (!a || !b) return bignum_zero();
	if (bignum_is_zero(b)) return *a;
	if (bignum_is_zero(a)) {
		bignum r = *b;
		r.sign = !r.sign;
		return r;
	}
	if (!a->sign && !b->sign) {
		return bignum_is_gt(a, b)?simple_sub(a, b, 0):simple_sub(b, a, 0);
	} else if (!a->sign && b->sign) {
		return simple_add(a, b, 0);
	} else if (a->sign && !b->sign) {
		return simple_add(a, b, 1);
	} else {
		return bignum_is_gt(a, b)?simple_sub(b, a, 0):simple_sub(a, b, 1);
	}
}

int bignum_inplace_sub(bignum * a, bignum const *b) {
	/* a = a - b */
	/* a and b non-negative */
	if(!a || !b) return 1;
	if(a->sign || b->sign) return 1;
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

int bezout_coefficients(bignum const *a, bignum const *b, bignum *x, bignum *y) {
	if (!a || !b || bignum_is_zero(a) || bignum_is_zero(b)) return 1;

	/* Use rotating indices */
	int i;
	bignum quotients[3], remainders[3], s[3], t[3], scratch;
	
	remainders[0] = *a;
	remainders[1] = *b;
	s[0] = bignum_small(1);
	s[1] = bignum_small(0);
	t[0] = bignum_small(0);
	t[1] = bignum_small(1);

	i = 1;
	do { 
		i = (i + 1) % 3;
		quotients[i] = bignum_div(&remainders[(i+1)%3], &remainders[(i+2)%3], &remainders[i]);
		scratch = bignum_mul(&quotients[i], &s[(i+2)%3]);
		s[i] = bignum_sub(&s[(i+1)%3], &scratch);
		scratch = bignum_mul(&quotients[i], &t[(i+2)%3]);
		t[i] = bignum_sub(&t[(i+1)%3], &scratch);
	} while (!bignum_is_zero(&remainders[i]));
	*x = s[(i+2)%3];
	*y = t[(i+2)%3];
	return 0;
}

bignum bignum_div(bignum const *a, bignum const *m, bignum * r) {
	/* a = qm + r */
	if (!a || !m || bignum_is_lt(a, m) || bignum_is_zero(m)) return bignum_zero();
	
	bignum r_deref, q, scratch;
	q = bignum_zero();
	scratch = *a;

	if (!r) r = &r_deref; /* Discard remainder. */

	*r = bignum_zero();

	if (bignum_is_eq(a, m)) return bignum_small(1);

	/* long division */
	int i;
	for(i = 0; i < 4096; i++) {
		bignum_bit_shift_left(r);
		bignum_bit_shift_left(&q);
		r->a[0] = r->a[0] + bignum_bit_shift_left(&scratch);
		if(bignum_is_gte(r, m)) {
			bignum_inplace_sub(r, m);
			q.a[0] = q.a[0] + 1;
		}
	}
	return q;
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
		if(bignum_is_gte(&r,m)) bignum_inplace_sub(&r, m);
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
		if(bignum_is_gte(a,m)) bignum_inplace_sub(a, m);
	}

	return 0;
}

bignum bignum_mod_exp(bignum const *b, bignum const *e, bignum const *m) {
	/* b^e mod m */
	if (bignum_is_zero(e)) return bignum_small(1);

	int i;
	bignum r, scratch;

	r = bignum_small(1);
	scratch = bignum_mod(b, m);

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
	/* n = candidate, a = witness */

	bignum const three = bignum_small(3);
	bignum const one = bignum_small(1);
	
	bignum b = bignum_mod(a, n);

	/* Input invalid if n|a or n<3. */
	if (bignum_is_lt(n, &three)) return -1;
	if (bignum_is_zero(&b)) return -2;
	
	/* Write n-1 = q*2^k, with q odd */
	bignum q = *n;
	bignum_inplace_sub(&q, &one);
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
	r.sign = 0;
	return r;
}

bignum bignum_half_random(void) {
	bignum r;
	int i;
	for (i = 0; i < 64; i++) r.a[i] = rand();
	for (i = 64; i < 128; i++) r.a[i] = 0;
	r.sign = 0;
	return r;
}

/* 
** Generate random 2048 bit probable prime by performing n Miller-Rabin tests
** with random bases.
*/
bignum random_large_probable_prime(int n) {
	bignum witness, candidate;
	/* Note: Failing a Miller-Rabin test indicates a candidate is probably prime. */
	int tests_failed = 0;
	int number_candidates = 0;
	while (tests_failed < n) {
		candidate = bignum_half_random();
		number_candidates++;
		tests_failed = 0;
		while (tests_failed < n) {
			witness = bignum_random();
			int result = miller_rabin(&candidate, &witness);
			if (result == -1 || result == 0) { 
				/* Invalid candidate or candidate is composite. */
				if (result == 0) { 
					bignum_print(&witness);
					printf("is a Miller-Rabin witness for the compositeness of candidate #%d:\n", number_candidates);
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
	}
	return candidate;
}

bignum bignum_gcd(bignum const *a, bignum const *b) {
	if (!a || !b || bignum_is_zero(a) || bignum_is_zero(b)) return bignum_small(0);
	if (bignum_is_one(a) || bignum_is_one(b)) return bignum_small(1);

	bignum r1, r2, *p1, *p2, *temp;
	r1 = *a;
	r2 = *b;
	p1 = &r1;
	p2 = &r2;

	while (!bignum_is_zero(p2)) {
		bignum_reduce(p1, p2);
		temp = p1;
		p1 = p2;
		p2 = temp;
	}

	return *p1;
}

bignum bignum_lcm(bignum const *a, bignum const *b) {
	bignum ab = bignum_mul(a, b);
	bignum gcd = bignum_gcd(a, b);
	return bignum_div(&ab, &gcd, 0);
}

int keygen(void) {
	bignum p, q, n, lambda;
	bignum const one = bignum_small(1);
	
	/* Choose two large prime numbers p and q. */
	p = random_large_probable_prime(10);
	q = random_large_probable_prime(10);

	/* Compute n = pq. */
	n = bignum_mul(&p, &q);

	/* Compute lambda(n) = lcm(p - 1, q - 1) */
	bignum_inplace_sub(&p, &one);
	bignum_inplace_sub(&q, &one);
	lambda = bignum_lcm(&p, &q);

	/* Choodean integer e such that 1 < e < lambda(n) and gcd (e, lambda(n)) = 1 */
	bignum const e = bignum_small(65537);

	/* Determine d as the multiplicative inverse of e mod lambda(n) */
	bignum d, c;
	bezout_coefficients(&e, &lambda, &d, &c);

	printf("Public Key:\n");
	printf("n:\n");
	bignum_print(&n);
	printf("e:\n");
	bignum_print(&e);
	printf("\n");
	printf("Private Key:\n");
	printf("d:\n");
	bignum_print(&d);

	return 0;
}

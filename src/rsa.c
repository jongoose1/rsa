#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rsa.h"

/* Begin helper functions. */

/* O(log(n)) */
int bit_shift_left(bignum * a) {
	if (!a) return -1;
	int i;
	int r = a->a[NWORDS-1] >> 31;
	for(i = NWORDS-1; i >= 1; i--) {
		a->a[i] = (a->a[i] << 1) + (a->a[i-1]>>31);
	}
	a->a[0] = a->a[0] << 1;
	return r;
}

/* O(log(n)) */
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

/* O(log(n)) */
int simple_inplace_add(bignum * b, int i, u32 u) {
	/* b = b + u*2^(32*i) */
	u64 r;
	while(i < NWORDS && u) {
		r = (u64) b->a[i] + u;
		b->a[i] = r;
		u = r >> 32;
		i++;
	}
	return u;
}

/* O(log(n)) */
int simple_inplace_sub(bignum * b, int i, u32 u) {
	/* b = b - u*2^(32*i) */
	if (i >= NWORDS) return u;
	int borrow = b->a[i] < u;
	b->a[i] = b->a[i] - u;
	return borrow?simple_inplace_sub(b, i+1, 1):0;
}

/* O(log(n)) */
int inplace_sub(bignum * a, bignum const *b) {
	/* a = a - b */
	/* |a| >= |b| */
	if(!a || !b) return 1;
	int i;
	u32 borrow = 0;
	u32 next_borrow;
	for (i = 0; i < NWORDS; i++) {
		next_borrow = ((borrow && a->a[i] == 0) || (a->a[i] - borrow < b->a[i]))?1:0;
		a->a[i] = a->a[i] - b->a[i] - borrow;
		borrow = next_borrow;
	}
	return 0;
}

/* O(log(n)) */
int inplace_add(bignum *a, bignum const *b) {
	/* r = a + b */
	/* a and b non-negative */
	if (!a || !b) return 1;
	int i;
	u64 sum_carry = 0;
	for (i = 0; i < NWORDS; i++) {
		sum_carry = sum_carry + a->a[i] + b->a[i];
		a->a[i] = sum_carry; /* Least significant 32 bits */
		sum_carry = sum_carry >> 32;
	}
	return 0;
}

/* O(log^2(n)) */
bignum sub(bignum const *a, bignum const *b, int sign) {
	/* r = |a| - |b| */
	/* sign(r) = sign */
	/* prereq: |a| >= |b| */

	if(!a || !b) return bignum_zero();
	bignum r = *a;
	inplace_sub(&r, b);
	r.sign = sign;
	return r;
}

/* O(log(n)) */
bignum add(bignum const *a, bignum const *b, int sign) {
	/* r = |a| + |b| */
	/* sign(r) = sign */
	if (!a || !b) return bignum_zero();
	bignum r = *a;
	inplace_add(&r, b);
	r.sign = sign;
	return r;
}


int last_one_bit(bignum const *a) {
	/* e.g. 0x00023199 returns 17 */
	int r;
	for (r = NWORDS *32 - 1; r > 0; r = r - 32) {
		if (a->a[r/32] != 0) {
			while ((a->a[r/32] & (1 << (r%32))) == 0) r = r - 1;
			return r;
		}
	}
	return -1;
}

int bignum_quarter_print(bignum const *a){
	if (!a) return 1;
	int i, j, k;
	k = NWORDS/4-1;
	for(i = 0; i < NWORDS/32; i++ ) {
		for(j = 0; j < 8; j++) {
			printf("%08x ",a->a[k--]);
		}
		printf("\n");
	}
	return 0;
}

int bignum_half_print(bignum const *a){
	if (!a) return 1;
	int i, j, k;
	k = NWORDS/2-1;
	for(i = 0; i < NWORDS/16; i++ ) {
		for(j = 0; j < 8; j++) {
			printf("%08x ",a->a[k--]);
		}
		printf("\n");
	}
	return 0;
}

/* End helper functions. */

/* O(log(n)) */
int bignum_print(bignum const *a) {
	if (!a) return 1;
	int i, j, k;
	k = NWORDS-1;
	printf("Sign:  %d\n", a->sign?1:0);
	for(i = 0; i < NWORDS/8; i++ ) {
		for(j = 0; j < 8; j++) {
			printf("%08x ",a->a[k--]);
		}
		printf("\n");
	}
	return 0;
}

/* O(log(n)) */
bignum bignum_zero(void) {
	bignum r;
	int i;
	for (i = 0; i < NWORDS; i++) r.a[i] = 0;
	r.sign = 0;
	return r;
}

/* O(log(n)) */
int bignum_is_zero(bignum const *a) {
	/* -0 == 0 */
	if(!a) return 0;
	int i;
	for(i = 0; i < NWORDS; i++) if(a->a[i] != 0) return 0;
	return 1;
}

/* O(log(n)) */
int bignum_is_one(bignum const *a) {
	/* -1 != 1 */
	if(!a) return 0;
	int i;
	if(a->a[0] != 1) return 0;
	for(i = 1; i<NWORDS; i++) if(a->a[i] != 0) return 0;
	if (a->sign) return 0;
	return 1;
}

/* O(log(n)) */
int bignum_is_eq(bignum const *a, bignum const *b) {
	/* -0 == 0 */
	if (!a || !b) return 0;
	if (bignum_is_zero(a) && bignum_is_zero(b)) return 1;
	int i;
	for (i = 0; i < NWORDS; i++) if (a->a[i] != b->a[i]) return 0;
	return (a->sign == b->sign);
}

/* O(log(n)) */
int bignum_is_gt(bignum const *l, bignum const *r) {
	/* -0 !> 0 */
	if(!l || !r) return 0;
	if (bignum_is_eq(l, r)) return 0;
	if (l->sign && !r->sign) return 0;
	if (!l->sign && r->sign) return 1;
	if (l->sign && r->sign) {
		bignum minus_l = *l;
		bignum minus_r = *r;
		minus_l.sign = 0;
		minus_r.sign = 0;
		return bignum_is_lt(&minus_l, &minus_r);
	}
	/* Both positive. */
	int i;
	for(i = NWORDS-1; i >= 0; i--) {
		if(l->a[i] < r->a[i]) return 0;
		if(l->a[i] > r->a[i]) return 1;
	}
	return 0;
}

/* O(log(n)) */
int bignum_is_gte(bignum const *l, bignum const *r) {
	return bignum_is_gt(l, r) || bignum_is_eq(l, r);
}

/* O(log(n)) */
int bignum_is_lt(bignum const *l, bignum const *r) {
	return !bignum_is_gte(l, r);
}

/* O(log(n)) */
int bignum_is_lte(bignum const *l, bignum const *r) {
	return !bignum_is_gt(l, r);
}

/* O(1) */
int bignum_is_even(bignum const *a) {
	return a->a[0] % 2 == 0;
}

/* O(1) */
int bignum_is_odd(bignum const *a) {
	return a->a[0] % 2 == 1;
}

/* O(log(n)) */
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
		return bignum_is_gt(a, b)?sub(a, b, 0):sub(b, a, 0);
	} else if (!a->sign && b->sign) {
		return add(a, b, 0);
	} else if (a->sign && !b->sign) {
		return add(a, b, 1);
	} else {
		return bignum_is_gt(a, b)?sub(b, a, 0):sub(a, b, 1);
	}
}

/* O(log(n)) */
bignum bignum_add(bignum const *a, bignum const *b) {
	/* r = a + b */
	/* a + b = a - (-b) */
	bignum b2 = *b;
	b2.sign = !b2.sign;
	return bignum_sub(a, &b2); 
}

/* O(log^3(n)) */
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

/* O(log^2(n)) */
bignum bignum_mul(bignum const *p, bignum const *q) {
	int i, j;
	u32 L, M;
	u64 x;
	bignum leasts;
	bignum mosts;
	bignum r = bignum_zero();
	if (!p || !q || bignum_is_zero(p) || bignum_is_zero(q)) return r;
	if (bignum_is_one(p)) return *q;
	if (bignum_is_one(q)) return *p;
	for(i = 0; i < NWORDS; i++) {
		leasts = bignum_zero();
		mosts = bignum_zero();
		for(j = 0; j < NWORDS; j++) {
			if(i + j > NWORDS-1 || p->a[i] == 0 || q->a[j] == 0) continue;
			x = (u64) p->a[i] * q->a[j];
			L = x;
			M = x >> 32;
			leasts.a[i+j] = L;
			if (i+j+1 < NWORDS) mosts.a[i+j+1] = M;
		}
		inplace_add(&r, &leasts);
		inplace_add(&r, &mosts);
	}
	r.sign = (p->sign != q->sign);
	return r;
}

/* O(log^2(n)) */
bignum bignum_div(bignum const *a, bignum const *m, bignum * r) {
	/* a = qm + r */
	if (!a || !m || bignum_is_zero(m)) {
		*r = bignum_zero();
		return bignum_zero();
	} else if (bignum_is_lt(a, m)) {
		*r = *a;
		return bignum_zero();
	} else if (bignum_is_eq(a, m)) {
		*r = bignum_zero();
		return bignum_small(1);
	}
	bignum r_deref, q;
	q = bignum_zero();
	if (!r) r = &r_deref; /* Discard remainder. */
	*r = bignum_zero();
	if (bignum_is_eq(a, m)) return bignum_small(1);
	/* long division */
	int i;
	for(i = last_one_bit(a); i >= 0; i--) {
		bit_shift_left(r);
		bit_shift_left(&q);
		r->a[0] = r->a[0] + ((a->a[i/32] >> (i%32)) & 1);
		if(bignum_is_gte(r, m)) {
			inplace_sub(r, m);
			q.a[0] = q.a[0] + 1;
		}
	}
	return q;
}

/* O(log^2(n)) */
bignum bignum_mod(bignum const *a, bignum const *m) {
	/* a = qm + r */
	bignum r;
	bignum_div(a, m, &r);
	return r;
}

/* O(log^2(n)) */
int bignum_reduce(bignum *a, bignum const *m) {
	/* a = qm + r */
	if (!a || !m || bignum_is_zero(m)) return 1;
	if (bignum_is_lt(a, m)) return 0;
	if (bignum_is_eq(a, m)) {
		*a = bignum_zero();
		return 0;
	}
	bignum r;
	bignum_div(a, m, &r);
	*a = r;
	return 0;
}

/* O(log^3(n)) */
bignum bignum_mod_exp(bignum const *b, bignum const *e, bignum const *m) {
	/* b^e mod m */
	if (bignum_is_zero(e)) return bignum_small(1);
	int i, lob;
	bignum r, scratch;
	r = bignum_small(1);
	scratch = bignum_mod(b, m);
	lob = last_one_bit(e);
	for(i = 0; i <= lob; i++) {
		if (e->a[i/32] & (1 << (i%32))) {
			r = bignum_mul(&r, &scratch);
			bignum_reduce(&r, m);
		}
		scratch = bignum_mul(&scratch, &scratch);
		bignum_reduce(&scratch, m);
	}
	return r;
}

/* O(log(n)) */
bignum bignum_small(u32 x) {
	bignum r = bignum_zero();
	r.a[0] = x;
	return r;
}

/* O(log^3(n)) */
int miller_rabin(bignum const * n, bignum const * a) {
	/* return 1: test fails, probably prime. */
	/* return 0: n composite. */
	/* return -1: invalid n */
	/* return -2: invalid a */
	/* n = candidate, a = witness */
	int i, k;
	bignum const three = bignum_small(3);
	bignum const one = bignum_small(1);
	bignum b = bignum_mod(a, n);
	/* Input invalid if n|a or n<3. */
	if (bignum_is_lt(n, &three)) return -1;
	if (bignum_is_zero(&b)) return -2;
	/* Write n-1 = q*2^k, with q odd */
	bignum q = *n;
	inplace_sub(&q, &one);
	bignum const negative_one = q;
	k = 0;
	while (bignum_is_even(&q)) {
		bit_shift_right(&q);
		k++;
	}
	b = bignum_mod_exp(&b, &q, n);
	if (bignum_is_one(&b)) return 1;
	for (i = 0; i < k; i++) {
		if (bignum_is_eq(&b, &negative_one)) return 1;
		b = bignum_mul(&b, &b);
		bignum_reduce(&b, n);
	}
	return 0;
}

/* O(log(n)) */
bignum bignum_random(void) {
	bignum r;
	int i;
	for (i = 0; i < NWORDS; i++) r.a[i] = rand();
	r.sign = 0;
	return r;
}

/* O(log(n)) */
bignum bignum_half_random(void) {
	bignum r;
	int i;
	for (i = 0; i < NWORDS/2; i++) r.a[i] = rand();
	for (i = NWORDS/2; i < NWORDS; i++) r.a[i] = 0;
	r.sign = 0;
	return r;
}

/* O(log(n)) */
bignum bignum_quarter_random(void) {
	bignum r;
	int i;
	for (i = 0; i <NWORDS/4; i++) r.a[i] = rand();
	for (i = NWORDS/4; i < NWORDS; i++) r.a[i] = 0;
	r.sign = 0;
	return r;
}

/* this is a shit way of doing things. */
static bignum primorial_init() {	
	bignum primorial = bignum_small(223092870);

#if NWORDS>=128
	primorial.a[30] = 0x3CB2BBE8;
	primorial.a[29] = 0x60C95193;
	primorial.a[28] = 0xAC35C2F7;
	primorial.a[27] = 0xBC3271E0;
	primorial.a[26] = 0x45DCC6AE;
	primorial.a[25] = 0x6BE995BA;
	primorial.a[24] = 0x7A73E8E7;
	primorial.a[23] = 0x5FDAFBEB;
	primorial.a[22] = 0x255C1FA7;
	primorial.a[21] = 0xFD220A11;
	primorial.a[20] = 0x35B3D273;
	primorial.a[19] = 0x3116044E;
	primorial.a[18] = 0xC1BD8E93;
	primorial.a[17] = 0x5CFDCA9F;
	primorial.a[16] = 0xD60AE558;
	primorial.a[15] = 0x15DA4CDB;
	primorial.a[14] = 0x25B55FB0;
	primorial.a[13] = 0xC26BEF47;
	primorial.a[12] = 0xC9D17E0A;
	primorial.a[11] = 0x02499251;
	primorial.a[10] = 0x61AE33AA;
	primorial.a[9] = 0x43035D90;
	primorial.a[8] = 0x1EF3041F;
	primorial.a[7] = 0x962B0C84;
	primorial.a[6] = 0x412D3D80;
	primorial.a[5] = 0x34901A62;
	primorial.a[4] = 0x663664A4;
	primorial.a[3] = 0x0366DF56;
	primorial.a[2] = 0x7AB682B6;
	primorial.a[1] = 0x348ACFCE;
	primorial.a[0] = 0x775EC712;
#endif
	return primorial;
}

/* 
** Generate random NWORDS*8 bit probable prime by performing n Miller-Rabin tests
** with random bases.
*/
/* O(log^4(n)) */
bignum random_large_probable_prime(int n) {
	bignum witness, candidate;
	bignum minimum = bignum_zero();
	bignum gcd;
	bignum const one = bignum_small(1);
	/* A primorial number is a product of the first primes. */
	/* We can use this number to weed out most of out candidates before having to perform a Miller-Rabin test. */
	bignum const primorial = primorial_init();

	/* sqrt(2) */
	/* Use this as a minimum so that n = pq is at least NWORDS*16 bits long. */
	minimum.a[NWORDS/4-1] = 0xB504F000;


	/* Note: Failing a Miller-Rabin test indicates a candidate is probably prime. */
	int tests_failed = 0;
	int number_candidates = 0;
	while (tests_failed < n) {
		
		/* Find an appropriate candidate. */
		while (1) {
			candidate = bignum_quarter_random();
			
			/* set most significant bit to one, so more likely to meet minimum. */ 
			candidate.a[NWORDS/4 - 1] = candidate.a[NWORDS/4 -1] | (1 << 31);
			if (bignum_is_lt(&candidate, &minimum)) continue;

			/* set least significant bit to one, so more likely to be prime. */
			candidate.a[0] = candidate.a[0] | 1;
			gcd = bignum_gcd(&candidate, &primorial);
			if (bignum_is_eq(&gcd, &one)) break;
		}
		number_candidates++;

		tests_failed = 0;
		while (tests_failed < n) {
			witness = bignum_random();
			bignum_reduce(&witness, &candidate);
			int result = miller_rabin(&candidate, &witness);
			if (result == -1 || result == 0) { 
				/* Invalid candidate or candidate is composite. */
				if (result == 0) { 
					bignum_quarter_print(&witness);
					printf("is a Miller-Rabin witness for the compositeness of candidate #%d:\n", number_candidates);
					bignum_quarter_print(&candidate);
					printf("\n");
				}
				break;
			} else if (result == 1) {
				/* Test Failed. Candidate is probably prime. */
				tests_failed++;
				printf("Candidate #%d:\n", number_candidates);
				bignum_quarter_print(&candidate);
				printf("failed test #%d and is probably prime.\n\n", tests_failed);
			}
		}
	}
	return candidate;
}

/* O(log^3(n)) */
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

/* O(log^3(n)) */
bignum bignum_lcm(bignum const *a, bignum const *b) {
	bignum ab = bignum_mul(a, b);
	ab.sign = 0;
	bignum gcd = bignum_gcd(a, b);
	return bignum_div(&ab, &gcd, 0);
}

/* O(log^4(n)) */
keypair keygen(void) {
	bignum p, q, n, lambda;
	bignum const one = bignum_small(1);
	bignum n_minimum = bignum_zero();
	n_minimum.a[NWORDS/2 - 1] = 1 << 31;
	n = bignum_zero();
	while (bignum_is_lt(&n, &n_minimum)) {
		/* Choose two large prime numbers p and q. */
		p = random_large_probable_prime(10);
		q = random_large_probable_prime(10);
	
		/* Compute n = pq. */
		n = bignum_mul(&p, &q);
	}
	/* Compute lambda(n) = lcm(p - 1, q - 1) */
	p = bignum_sub(&p, &one);
	q = bignum_sub(&q, &one);
	lambda = bignum_lcm(&p, &q);
	/* Choodean integer e such that 1 < e < lambda(n) and gcd (e, lambda(n)) = 1 */
	bignum const e = bignum_small(65537);
	bignum gcd = bignum_gcd(&e, &lambda);
	assert(bignum_is_eq(&gcd, &one));
	/* Determine d as the multiplicative inverse of e mod lambda(n) */
	bignum d, c;
	bezout_coefficients(&e, &lambda, &d, &c);
	if (d.sign) {
		d.sign = 0;
		bignum_reduce(&d, &lambda);
		d.sign = 1;
		d = bignum_add(&d, &lambda);
	}
	keypair keys;
	keys.pk.e = e;
	keys.pk.n = n;
	keys.sk.d = d;
	return keys;
}

/* O(log(n)) */
int bignum_pad(bignum * m) {
	int i;
	if (NPADDING == 0) return 0;
	for (i = MSIZE/4; i < CSIZE/4 - 1; i++) m->a[i] = rand();
	return 0;
}

/* O(log^3(n)) */
int inplace_encrypt(bignum * m, public_key const *pk) {
	bignum max_message = bignum_zero();
	max_message.a[MSIZE/4] = 1;
	m->sign = 0;
	if (!m || !pk || bignum_is_zero(&pk->n) || bignum_is_gte(m, &max_message)) return 1;
	bignum_pad(m);
	*m = bignum_mod_exp(m ,&pk->e, &pk->n);
	return 0;
}

/* O(log^3(n)) */
bignum encrypt(bignum const *m, public_key const *pk) {
	bignum r = *m;
	return inplace_encrypt(&r, pk)?bignum_zero():r;
}

/* O(log^3(n)) */
int inplace_decrypt(bignum * m, keypair const *kp) {
	if (!m || !kp || bignum_is_zero(&kp->pk.n)) return 1;
	*m = bignum_mod_exp(m ,&kp->sk.d, &kp->pk.n);
	return 0;
}

/* O(log^3(n)) */
bignum decrypt(bignum const *m, keypair const *kp) {
	bignum r = *m;
	return inplace_decrypt(&r, kp)?bignum_zero():r;
}

/* O(m*log^3(n)) */
int encrypt_file(char const *fplainname, char const *fciphername, public_key const *pk) {
	FILE *fplain, *fcipher;
	char buffer[11] = "cipher.txt";
	bignum m;
	u32 chunks = 0;
	if(!fplainname || !pk || bignum_is_zero(&pk->n) || strlen(fplainname) + 1 > MSIZE) return 1;
	fplain = fopen(fplainname, "rb");
	if (!fplain) return 1;
	if (!fciphername) fciphername = buffer;
	fcipher = fopen(fciphername, "wb");
	if (!fcipher) return 1;
	/* Metadata. 0 placeholder. */
	m = bignum_zero();
	if (fwrite(&m, CSIZE, 1, fcipher) != 1) return 1;
	chunks++;
	/* Filename. Must fit into one chunk. */
	m = bignum_zero();
	if (strlen(fplainname) + 1 > MSIZE) return 1; 
	memcpy(m.a, fplainname, strlen(fplainname) + 1);
	inplace_encrypt(&m, pk);
	if (fwrite(m.a, CSIZE, 1, fcipher) != 1) return 1;
	chunks++;
	/* Data. */
	u32 bytes_read;
	do {
		m = bignum_zero();
		bytes_read = fread(m.a, 1, MSIZE, fplain);
		inplace_encrypt(&m, pk);
		if (fwrite(m.a, CSIZE, 1, fcipher) != 1) return 1;
		chunks++;
	} while (bytes_read == MSIZE);
	/* Go back and write metadata. */
	m = bignum_zero();
	m.a[0] = chunks;
	m.a[1] = bytes_read;
	inplace_encrypt(&m, pk);
	fseek(fcipher, 0, SEEK_SET);
	if (fwrite(m.a, CSIZE, 1, fcipher) != 1) return 1;
	fclose(fplain);
	fclose(fcipher);
	return 0;
}

/* O(m*log^3(n)) */
int decrypt_file(char const *fciphername, keypair const *kp) {
	FILE *fplain, *fcipher;
	bignum c;
	if (!fciphername || !kp || bignum_is_zero(&kp->pk.n)) return 1;
	fcipher = fopen(fciphername, "rb");
	if (!fcipher) return 1;
	/* Metadata. */
	c = bignum_zero();
	if (fread(c.a, CSIZE, 1, fcipher) != 1) return 1;
	inplace_decrypt(&c, kp);
	u32 remaining_chunks = c.a[0] - 1;
	u32 last_chunk_size = c.a[1];
	/* Filename. */
	c = bignum_zero();
	if (fread(c.a, CSIZE, 1, fcipher) != 1) return 1;
	remaining_chunks--;
	inplace_decrypt(&c, kp);
	fplain = fopen ((char *) c.a, "wb");
	if (!fplain) return 1;
	/* Data. */
	c = bignum_zero();
	while (fread(c.a, CSIZE, 1, fcipher) == 1) {
		remaining_chunks--;
		inplace_decrypt(&c, kp);
		if (remaining_chunks > 0) {
			if (fwrite(c.a, MSIZE, 1, fplain) != 1) return 1;
		} else {
			if (fwrite(c.a, last_chunk_size, 1, fplain) != 1) return 1;
		}
		c = bignum_zero();
	}
	fclose(fplain);
	fclose(fcipher);
	return 0;
}

/* O(log(n)) */
int keypair_save(keypair const *keys, char const *filename) {
	int r;
	FILE *f;
	if (!filename || !keys) return 1;
	f = fopen(filename, "wb");
	if (!f) return 2;
	r = fwrite(keys, sizeof(keypair), 1, f);
	if (r != 1) return 3;
	r = fclose(f);
	if (r) return 4;
	return 0;
}

/* O(log(n)) */
int keypair_load(keypair *keys, char const *filename) {
	int r;
	FILE *f;
	if (!filename || !keys) return 1;
	f = fopen(filename, "rb");
	if (!f) return 2;
	r = fread(keys, sizeof(keypair), 1, f);
	if (r != 1) return 3;
	r = fclose(f);
	if (r) return 4;
	return 0;
}

/* O(log(n)) */
int public_key_save(public_key const *pk, char const *filename) {
	int r;
	FILE *f;
	if (!filename || !pk) return 1;
	f = fopen(filename, "wb");
	if (!f) return 2;
	r = fwrite(pk, sizeof(public_key), 1, f);
	if (r != 1) return 3;
	r = fclose(f);
	if (r) return 4;
	return 0;
}

/* O(log(n)) */
int public_key_load(public_key *pk, char const *filename) {
	int r;
	FILE *f;
	if (!filename || !pk) return 1;
	f = fopen(filename, "rb");
	if (!f) return 2;
	r = fread(pk, sizeof(public_key), 1, f);
	if (r != 1) return 3;
	r = fclose(f);
	if (r) return 4;
	return 0;
}

/* O(log(n)) */
int keypair_print(keypair const *keys) {
	if (!keys) return 1;
	public_key_print(&keys->pk);
	printf("Private Key:\n");
	printf("d:\n");
	bignum_half_print(&keys->sk.d);
	return 0;
}

/* O(log(n)) */
int public_key_print(public_key const *pk) {
	if (!pk) return 1;
	printf("Public Key:\n");
	printf("e:\n");
	bignum_half_print(&pk->e);
	printf("n:\n");
	bignum_half_print(&pk->n);
	return 0;
}

#ifndef RSA_H
#define RSA_H

#include <stdint.h>
#include <stddef.h>

typedef uint64_t u64;
typedef uint32_t u32;

typedef struct bignum {
	u32 a[128]; /* a[0] is LEAST SIGNIFICANT, a[127] is MOST SIGNIFICANT */
	int sign;   /* zero = positive, non-zero = negative */
} bignum;

typedef struct public_key {
	bignum * e;
	bignum * n;
} public_key;

typedef struct secret_key {
	bignum * d;
} secret_key;

typedef struct keypair {
	public_key * pk;
	secret_key * sk;
} keypair;

int bignum_print(bignum const *a);
bignum bignum_zero(void);
bignum bignum_mul(bignum const *p, bignum const *q);
int bignum_inplace_add(bignum * b, int i, u32 u);
int bignum_inplace_sub(bignum *a, bignum const *b);
bignum bignum_sub(bignum const *a, bignum const *b);
bignum bignum_div(bignum const *a, bignum const *m, bignum *r);
bignum bignum_mod(bignum const *a, bignum const *m);
int bignum_reduce(bignum *a, bignum const *m);
int bignum_is_eq(bignum const *a, bignum const *b);
int bignum_is_lt(bignum const *l, bignum const *r);
int bignum_is_lte(bignum const *l, bignum const *r);
int bignum_is_gt(bignum const *l, bignum const *r);
int bignum_is_gte(bignum const *l, bignum const *r);
int bignum_is_zero(bignum const *a);
int bignum_is_one(bignum const *a);
int bignum_is_even(bignum const *a);
int bignum_is_odd(bignum const *a);
int bignum_bit_shift_left(bignum *a);
int bignum_bit_shift_right(bignum * a);
bignum bignum_mod_exp(bignum const *b, bignum const *e, bignum const *m);
bignum bignum_small(u32 x);
int miller_rabin(bignum const * n, bignum const * a);
int bezout_coefficients(bignum const *a, bignum const *b, bignum *x, bignum *y);
bignum bignum_random(void);
bignum bignum_half_random(void);
bignum random_large_probable_prime(int n);
bignum bignum_gcd(bignum const *a, bignum const *b);
bignum bignum_lcm(bignum const *a, bignum const *b);

#endif

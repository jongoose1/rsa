#ifndef RSA_H
#define RSA_H

#include <stdint.h>
#include <stddef.h>

typedef uint64_t u64;
typedef uint32_t u32;

typedef struct bignum {
	u32 a[128]; /* a[0] is LEAST SIGNIFICANT, a[127] is MOST SIGNIFICANT */
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

int bignum_print(const bignum *a);
bignum bignum_zero();
bignum bignum_mul(const bignum *p, const bignum *q);
int bignum_sub(bignum *a, const bignum *b);
bignum bignum_mod(const bignum *a, const bignum *m);
int is_eq(const bignum *a, const bignum *b);
int is_lt(const bignum *l, const bignum *r);
int is_lte(const bignum *l, const bignum *r);
int is_gt(const bignum *l, const bignum *r);
int is_gte(const bignum *l, const bignum *r);
int is_zero(const bignum *a);
int is_one(const bignum *a);
int bit_shift_left(bignum *a);
bignum bignum_mod_exp(const bignum *b, const bignum *e, const bignum *m);
int miller_rabin(bignum * n, bignum * a);

#endif

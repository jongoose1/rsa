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

int bignum_print(bignum const *a);
bignum bignum_zero();
bignum bignum_mul(bignum const *p, bignum const *q);
int bignum_sub(bignum *a, bignum const *b);
bignum bignum_mod(bignum const *a, bignum const *m);
int is_eq(bignum const *a, bignum const *b);
int is_lt(bignum const *l, bignum const *r);
int is_lte(bignum const *l, bignum const *r);
int is_gt(bignum const *l, bignum const *r);
int is_gte(bignum const *l, bignum const *r);
int is_zero(bignum const *a);
int is_one(bignum const *a);
int bit_shift_left(bignum *a);
bignum bignum_mod_exp(bignum const *b, bignum const *e, bignum const *m);
int miller_rabin(bignum * n, bignum * a);

#endif

#ifndef RSA_H
#define RSA_H

#include <stdint.h>
#include <stddef.h>

typedef uint64_t u64;
typedef uint32_t u32;

typedef struct bignum {
	u32 * a; /* a[0] is LEAST SIGNIFICANT, a[127] is MOST SIGNIFICANT */
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

bignum * new_bignum(void);
int delete_bignum(bignum * bn);
int assign(bignum * l, bignum * r);
bignum * copy_bignum(bignum * a);
int print_bignum(bignum * a);
bignum * mul(bignum * p, bignum * q);

int is_eq(bignum * a, bignum * b);
int is_lt(bignum * l, bignum * r);
int is_lte(bignum * l, bignum * r);
int is_gt(bignum * l, bignum * r);
int is_gte(bignum * l, bignum * r);
int is_zero(bignum * a);
int is_one(bignum * a);
int bit_shift_left(bignum * a);
int bignum_sub(bignum * a, bignum * b);
bignum * mod_exp(bignum * b, bignum * e, bignum * m);
bignum * mod(bignum * a, bignum * m);
int miller_rabin(bignum * n, bignum * a);

#endif

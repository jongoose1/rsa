#ifndef RSA_H
#define RSA_H

#include <stdint.h>
#include <stddef.h>

/*
Number of 4 byte words in a bignum
128 -> 2048 bit modulus
256 -> 4096 bit modulus
512 -> 8192 bit modulus
1024 -> 16384 bit modulus
*/
#define NWORDS 64

/* Number of 4 byte words used in padding */
#define NPADDING (NWORDS/128)

/* Size in bytes of each ciphertext message */
#define CSIZE (NWORDS*2)

/* Maximum size in bytes of each plaintext message */
/* the '- 4' is needed so message < modulus (no two messages can be congruent mod N) */
/* modulus is gauranteed to be NWORDS*16 bits long (MSB = 1) */
#define MSIZE (CSIZE - 4 - NPADDING*4)

/* block size in bytes for JG2 hashing */
#define BSIZE ((CSIZE - 8)/2)

typedef uint64_t u64;
typedef uint32_t u32;

typedef struct bignum {
	u32 a[NWORDS]; /* a[0] is LEAST SIGNIFICANT, a[NWORDS-1] is MOST SIGNIFICANT */
	int sign;   /* zero = positive, non-zero = negative */
} bignum;

typedef struct public_key {
	bignum e;
	bignum n;
} public_key;

typedef struct secret_key {
	bignum d;
	int encrypted;
} secret_key;

typedef struct keypair {
	public_key pk;
	secret_key sk;
} keypair;

bignum bignum_zero(void);
bignum bignum_small(u32 x);
bignum bignum_random(void);
bignum bignum_half_random(void);
bignum bignum_quarter_random(void);
bignum random_large_probable_prime(int n, int tid);
bignum bignum_mul(bignum const *p, bignum const *q);
bignum bignum_add(bignum const *a, bignum const *b);
bignum bignum_sub(bignum const *a, bignum const *b);
bignum bignum_div(bignum const *a, bignum const *m, bignum *r);
bignum bignum_mod(bignum const *a, bignum const *m);
bignum bignum_mod_exp(bignum const *b, bignum const *e, bignum const *m);
bignum bignum_gcd(bignum const *a, bignum const *b);
bignum bignum_lcm(bignum const *a, bignum const *b);
int bignum_print(bignum const *a);
int bignum_pad(bignum * m);
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
int miller_rabin(bignum const * n, bignum const * a);
int bezout_coefficients(bignum const *a, bignum const *b, bignum *x, bignum *y);
int inplace_encrypt(bignum * m, public_key const *pk, int dont_pad);
bignum encrypt(bignum const *m, public_key const *pk);
int inplace_decrypt(bignum * m, keypair const *kp);
bignum decrypt(bignum const *m, keypair const *kp);
int encrypt_file(char const *fplainname, char const *fciphername, public_key const *pk);
int decrypt_file(char const *fciphername, keypair const *kp);
int keypair_save(keypair const *keys, char const *filename);
int keypair_load(keypair *keys, char const *filename);
int public_key_save(public_key const *pk, char const *filename);
int public_key_load(public_key *pk, char const *filename);
int keypair_print(keypair const *keys);
int public_key_print(public_key const *pk);
keypair keygen(void);
bignum jg2(void * d, size_t n, public_key const *pk);
bignum bignum_xor(bignum const *a, bignum const *b);
int encrypt_secret_key(char * password, keypair *kp);
int decrypt_secret_key(char * password, keypair *kp);

#endif

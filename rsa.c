#include <stdlib.h>
#include <stdio.h>
#include "rsa.h"

int bignum_print(const bignum *a){
	if (!a) return 1;
	int i, j, k;
	k = 127;
	for(i = 0; i < 16; i++ ){
		for(j = 0; j < 8; j++){
			printf("%8x ",a->a[k--]);
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

bignum bignum_zero(){
	bignum r;
	int i;
	for (i = 0; i < 128; i++) r.a[i] = 0;
	return r;
}

bignum bignum_mul(const bignum *p, const bignum *q){
	int i, j;
	u32 L, M;
	u64 x;
	bignum r;
	
	r = bignum_zero();

	if (!p || !q || is_zero(p) || is_zero(q)) return r;
	if (is_one(p)) return *q;
	if (is_one(q)) return *p;

	for(i = 0; i < 128; i++){
		for(j = 0; j < 128; j++){
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


/* Space: O(1) */
/* Time:  O(1) */
int is_eq(const bignum *a, const bignum *b){
	if(!a || !b) return 0;
	int i;
	for(i = 0; i < 128; i++) if (a->a[i] != b->a[i]) return 0;
	return 1;
}

/* Space: O(1) */
/* Time:  O(1) */
int is_lt(const bignum *l, const bignum *r){
	if(!l || !r) return 0;
	int i;
	for(i = 127; i >= 0; i--){
		if(l->a[i] < r->a[i]) return 1;
		if(l->a[i] > r->a[i]) return 0;
	}
	return 0;
}

/* Space: O(1) */
/* Time:  O(1) */
int is_lte(const bignum *l, const bignum *r){
	if(!l || !r) return 0;
	int i;
	for(i = 127; i >= 0; i--){
		if(l->a[i] < r->a[i]) return 1;
		if(l->a[i] > r->a[i]) return 0;
	}
	return 1;
}

/* Space: O(1) */
/* Time:  O(1) */
int is_gt(const bignum *l, const bignum *r){
	if(!l || !r) return 0;
	int i;
	for(i = 127; i >= 0; i--){
		if(l->a[i] < r->a[i]) return 0;
		if(l->a[i] > r->a[i]) return 1;
	}
	return 0;
}

/* Space: O(1) */
/* Time:  O(1) */
int is_gte(const bignum *l, const bignum *r){
	if(!l || !r) return 0;
	int i;
	for(i = 127; i >= 0; i--){
		if(l->a[i] < r->a[i]) return 0;
		if(l->a[i] > r->a[i]) return 1;
	}
	return 1;
}

/* Space: O(1) */
/* Time:  O(1) */
int is_zero(const bignum *a){
	if(!a) return 0;
	int i;
	for(i = 0; i<128; i++) if(a->a[i] != 0) return 0;
	return 1;
}

/* Space: O(1) */
/* Time:  O(1) */
int is_one(const bignum *a){
	if(!a) return 0;
	int i;
	if(a->a[0] != 1) return 0;
	for(i = 1; i<128; i++) if(a->a[i] != 0) return 0;
	return 1;
}

int bit_shift_left(bignum * a){
	if (!a) return -1;
	int i;
	int r = a->a[127] >> 31;
	for(i = 127; i >= 1; i--){
		a->a[i] = (a->a[i] << 1) + (a->a[i-1]>>31);
	}
	a->a[0] = a->a[0] << 1;
	return r;
}

int bignum_sub(bignum * a, const bignum *b){
	/* a = a - b */
	if(!a || !b) return 1;
	if(is_gt(b, a)) return 1;

	int i;
	u64 c;
	for(i = 0; i < 128; i++){
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

bignum bignum_mod(const bignum *a, const bignum *m){
	/* a = qm + r */
	if (!a || !m || is_eq(a, m) || is_zero(m)) return bignum_zero();
	if(is_lt(a, m)) return *a;

	bignum r, scratch;
	r = bignum_zero();
	scratch = *a;

	/* long division */
	int i;
	for(i = 0; i < 4096; i++){
		bit_shift_left(&r);
		r.a[0] = r.a[0] + bit_shift_left(&scratch);
		if(is_gte(&r,m)) bignum_sub(&r, m);
	}
	return r;
}

bignum bignum_mod_exp(const bignum *b, const bignum *e, const bignum *m){
	/* b^e mod m */
	if (is_zero(e)) return bignum_zero();

	int i;
	bignum r, scratch;

	r = bignum_zero();
	r.a[0] = 1;
	scratch = *b;

	for(i = 0; i < 4096; i++) {
		if (i < 10) {
			printf("b^2^%d:\n",i);
			bignum_print(&scratch);
		}
		if (e->a[i/32] & (1 << (i%32))) {
			printf("Word: %d, Bit: %d is 1\n", i/32, i%32);
			r = bignum_mul(&r, &scratch);
			r = bignum_mod(&r, m);
			/* @todo use reduce() */
			printf("New Total:\n");
			bignum_print(&r);
		}
		scratch = bignum_mul(&scratch, &scratch);
		scratch = bignum_mod(&scratch, m);
	}
	return r;
}

int miller_rabin(bignum * n, bignum * a){
	return 0;
}
/*
int keygen(void){
	bignum * p = new_bignum();
	if(!p) return NULL;
	bignum * q = new_bignum();
	if(!q) return NULL;
	choose p & q;
	
}
*/


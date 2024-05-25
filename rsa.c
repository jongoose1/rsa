#include <stdlib.h>
#include <stdio.h>
#include "rsa.h"

bignum * new_bignum(void){
	bignum * bn = malloc(sizeof(bignum));
	if(!bn) return NULL;
	bn->a = calloc(128, sizeof(u32));
	if(!bn->a){
		free(bn);
		return NULL;
	}
	return bn;
}

int delete_bignum(bignum * bn){
	if (!bn) return 0;
	free(bn->a);
	free(bn);
	return 0;
}

int assign(bignum * l, bignum * r){
	if(!l || !r) return 1;
	int i;
	for(i =0;i <128; i++) l->a[i] = r->a[i];
	return 0;
}

bignum * copy_bignum(bignum * a){
	if(!a) return NULL;
	int i;
	bignum * r = new_bignum();
	if(!r) return NULL;
	for(i=0;i<128;i++) r->a[i] = a->a[i];
	return r;
}

int print_bignum(bignum * a){
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

bignum * mul(bignum * p, bignum * q){
	if(!p || !q) return NULL;
	int i, j;
	u32 L, M;
	u64 x;
	bignum * r = new_bignum();
	if(!r) return NULL;
	if(is_zero(p) || is_zero(q)) return r;
	if(is_one(p)) {
		assign(r, q);
		return r;
	}
	if(is_one(q)) {
		assign(r, p);
		return r;
	}
	for(i = 0; i < 128; i++){
		for(j = 0; j < 128; j++){
			if(i + j > 127) continue;
			x = (u64) p->a[i] * q->a[j];
			L = x;
			M = x >> 32;
			r->a[i + j] = r->a[i + j] + L;
			if (i + j + 1 <= 127) r->a[i + j + 1] = r->a[i + j + 1] + M;
		}
	}
	return r;
}


/* Space: O(1) */
/* Time:  O(1) */
int is_eq(bignum * a, bignum * b){
	if(!a || !b) return 0;
	int i;
	for(i = 0; i < 128; i++) if (a->a[i] != b->a[i]) return 0;
	return 1;
}

/* Space: O(1) */
/* Time:  O(1) */
int is_lt(bignum * l, bignum * r){
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
int is_lte(bignum * l, bignum * r){
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
int is_gt(bignum * l, bignum * r){
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
int is_gte(bignum * l, bignum * r){
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
int is_zero(bignum * a){
	if(!a) return 0;
	int i;
	for(i = 0; i<128; i++) if(a->a[i] != 0) return 0;
	return 1;
}

/* Space: O(1) */
/* Time:  O(1) */
int is_one(bignum * a){
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

int bignum_sub(bignum * a, bignum * b){
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

bignum * mod(bignum * a, bignum * m){
	/* a = qm + r */
	if (!a || !m) return NULL;


	bignum * r = new_bignum();
	bignum * scratch = copy_bignum(a);
	if(!r || !scratch) {
		delete_bignum(scratch);
		delete_bignum(r);
		return NULL;
	}
	if(is_eq(a, m)) return r;
	if(is_lt(a, m)) {
		assign(r, a);
		return r;
	}

	/* long division */
	int i;
	for(i = 0; i < 4096; i++){
		bit_shift_left(r);
		r->a[0] = r->a[0] + bit_shift_left(scratch);
		if(is_gte(r,m)) {
			bignum_sub(r, m);
		}
	}
	delete_bignum(scratch);
	return r;
}

bignum * mod_exp(bignum * b, bignum * e, bignum * m){
	/* b^e mod m */
	int i;
	bignum * r = new_bignum();
	bignum * scratch = copy_bignum(b);
	if(!r || !scratch) {
		delete_bignum(scratch);
		delete_bignum(r);
		return NULL;
	}
	r->a[0] = 1;

	if (is_zero(e))
		return r;

	bignum *t1, *t2;
	for(i = 0; i < 4096; i++) {
		if (i < 10) {
			printf("b^2^%d:\n",i);
			print_bignum(scratch);
		}
		if (e->a[i/32] & (1 << (i%32))) {
			printf("Word: %d, Bit: %d is 1\n", i/32, i%32);
			t1 = mul(r, scratch);
			t2 = mod(t1, m);
			delete_bignum(t1);
			t1 = r;
			delete_bignum(t1);
			r = t2;
			printf("New Total:\n");
			print_bignum(r);
		}
		t1 = mul(scratch, scratch);
		t2 = mod(t1, m);
		delete_bignum(t1);
		delete_bignum(scratch);
		scratch = t2;
	}
	delete_bignum(scratch);
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


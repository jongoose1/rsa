#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"

int main(int argc, char ** argv) {
	bignum a, b, gcd, lcm, product, sum, difference, mod, quotient, gcd_result,
	lcm_result, product_result, sum_result, difference_result, mod_result,
	quotient_result;
	
	a = bignum_zero();
	a.a[1] = 0x7;
	a.a[0] = 0x8e8ad668;
	
	b = bignum_zero();
	b.a[1] = 0x36;
	b.a[0] = 0x9b58fb70;
	
	gcd = bignum_small(200);

	lcm = bignum_zero();
	lcm.a[2] = 0x2;
	lcm.a[1] = 0x103232D3;
	lcm.a[0] = 0xE552D430;

	product = bignum_zero();
	product.a[2] = 0x19C;
	product.a[1] = 0xA737B58B;
	product.a[0] = 0x28B5C580;

	sum = bignum_zero();
	sum.a[1] = 0x3E;
	sum.a[0] = 0x29E3D1D8;

	difference = bignum_zero();
	difference.a[1] = 0x2F;
	difference.a[0] = 0x0CCE2508;

	mod = bignum_zero();
	mod.a[1] = 0x1;
	mod.a[0] = 0xB58D1E98;

	quotient = bignum_small(7);

	gcd_result = bignum_gcd(&b, &a);
	lcm_result = bignum_lcm(&b, &a);
	product_result = bignum_mul(&b, &a);
	sum_result = bignum_add(&b, &a);
	difference_result = bignum_sub(&b, &a);
	mod_result = bignum_mod(&b, &a);
	quotient_result = bignum_div(&b, &a, 0);

	bignum_is_eq(&gcd, &gcd_result)?printf("GCD PASSED\n"):printf("GCD FAILED\n");
	bignum_is_eq(&lcm, &lcm_result)?printf("LCM PASSED\n"):printf("LCM FAILED\n");
	bignum_is_eq(&product, &product_result)?printf("MUL PASSED\n"):printf("MUL FAILED\n");
	bignum_is_eq(&sum, &sum_result)?printf("ADD PASSED\n"):printf("ADD FAILED\n");
	bignum_is_eq(&difference, &difference_result)?printf("SUB PASSED\n"):printf("SUB FAILED\n");
	bignum_is_eq(&mod, &mod_result)?printf("MOD PASSED\n"):printf("MOD FAILED\n");
	bignum_is_eq(&quotient, &quotient_result)?printf("DIV PASSED\n"):printf("DIV FAILED\n");

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/rsa.h"

int main(int argc, char ** argv) {
	keypair kp;
	keypair_load(&kp, "keys/121.kp");
	decrypt_secret_key("password", &kp);

	bignum m = bignum_zero();
	m.a[0] = 0xFACEFACE;
	bignum_print(&m);
	
	bignum signature = sign_bignum(&m, &kp);
	bignum_print(&signature);

	printf("%d", verify_bignum(&m, &signature, &kp.pk));

	return 0;
}

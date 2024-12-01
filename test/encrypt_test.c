#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/rsa.h"

int main(void) {
	srand(time(0));
	keypair keys;
	keypair_load(&keys, "keys.kp");
	keypair_print(&keys);

	bignum message = bignum_zero();
	message.a[0] = 0x48656C6C; /* Hell */
#if 0
	message.a[1] = 0x6F2C206E; /* o, n */
	message.a[2] = 0x6F6F6E65; /* oone */
	message.a[3] = 0x21000000; /* !    */
#endif
	printf("Plaintext:\n");
	bignum_print(&message);

	inplace_encrypt(&message, &keys.pk, 0);

	printf("Ciphertext:\n");
	bignum_print(&message);

	inplace_decrypt(&message, &keys);

	printf("Plaintext:\n");
	bignum_print(&message);

	return 0;
}

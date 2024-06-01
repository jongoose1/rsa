#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"

int main(void) {
	srand(time(NULL));
	keypair keys;
	keypair_load(&keys, "mykeys.kp");
	keypair_print(&keys);

	bignum message = bignum_zero();
	message.a[0] = 0x48656C6C; /* Hell */
	message.a[1] = 0x6F2C206E; /* o, n */
	message.a[2] = 0x6F6F6E65; /* oone */
	message.a[3] = 0x21000000; /* !    */

	printf("Plaintext:\n");
	bignum_print(&message);

	inplace_encrypt(&message, &keys.pk);

	printf("Ciphertext:\n");
	bignum_print(&message);

	inplace_decrypt(&message, &keys);

	printf("Plaintext:\n");
	bignum_print(&message);

	return 0;
}

#include "rsa.h"

int main(void) {
	keypair keys, keys2;
	keys.pk.e = bignum_small(65537);
	keys.pk.n = bignum_small(100);
	keys.sk.d = bignum_small(42);
	if(keypair_save(&keys, "keys.kp")) return 1;
	if(keypair_load(&keys2, "keys.kp")) return 1;
	keypair_print(&keys);
	keypair_print(&keys2);
	return 0;
}

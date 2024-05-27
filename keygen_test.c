#include "rsa.h"

int main(void) {
	keypair keys = keygen();
	keypair_print(&keys);
	return keypair_save(&keys, "mykeys.kp");
}

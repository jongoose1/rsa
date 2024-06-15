#include <stdio.h>
#include "rsa.h"

int print_usage(char * exe) {
	printf("Usage: %s keypair.kp publickey.pk\n", exe);
	return 0;
}

int main(int argc, char ** argv) {
	if (argc < 3) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc > 3) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}

	keypair kp;
	keypair_load(&kp, argv[1]);
	return public_key_save(&kp.pk, argv[2]);
}

/* decrypt.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s cipher.txt keypair.kp password\n", exe);
		return 0;
}

int main(int argc, char **argv) {
	if (argc > 4) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc < 4) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}

	char *fciphername, *fkeyname, *password;
	fciphername = argv[1];
	fkeyname = argv[2];
	password = argv[3];

	keypair kp;
	if (keypair_load(&kp, fkeyname)) return 1;
	decrypt_secret_key(password, &kp);
	return decrypt_file(fciphername, &kp);
}

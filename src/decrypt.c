/* decrypt.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s cipher.txt plain.txt keypair.kp password\n", exe);
		return 0;
}

int main(int argc, char **argv) {
	if (argc > 5) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc < 5) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}

	char *fciphername, *fplainname, *fkeyname, *password;
	fciphername = argv[1];
	fplainname = argv[2];
	fkeyname = argv[3];
	password = argv[4];

	keypair kp;
	if (keypair_load(&kp, fkeyname)) return 1;
	decrypt_secret_key(password, &kp);
	return decrypt_file(fciphername, fplainname, &kp);
}

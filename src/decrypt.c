/* decrypt.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s cipher.txt keypair.kp\n", exe);
		return 0;
}

int main(int argc, char **argv) {
	if (argc > 3) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc < 3) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}

	char  *fkeyname, password[64];
	FILE *fcipher = fopen(argv[1], "rb");
	if (!fcipher) return 1;
	fkeyname = argv[2];
	if (isatty(STDOUT_FILENO)) {
		if (get_password(password, 64, "Enter password:") == -1) return 1;
	} else {
		/* output is redirected, do not print prompt to encsure decrypted file is exactly the same as its original version */
		if (get_password(password, 64, 0) == -1) return 1;
	}	

	keypair kp;
	if (keypair_load(&kp, fkeyname)) return 1;
	decrypt_secret_key(password, &kp);
	
	return decrypt_stream(fcipher, stdout, &kp);
}

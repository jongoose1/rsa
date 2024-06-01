/* decrypt.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s --help\n", exe);
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
	int i, found_ciphertext;
	char *fciphername, *fkeyname;
	found_ciphertext = 0;
	fciphername = NULL;
	fkeyname = NULL;
	for(i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0) {
			print_usage(argv[0]);
			return 0;
		} else if (found_ciphertext) {
			fkeyname = argv[i];
		} else {
			fciphername = argv[i];
			found_ciphertext = 1;
		}
	}
	if (!fciphername || !fkeyname) {
		print_usage(argv[0]);
		return 1;
	}

	keypair kp;

	if (keypair_load(&kp, fkeyname)) return 1;

	return decrypt_file(fciphername, &kp);
}

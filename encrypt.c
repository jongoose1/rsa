/* encrypt.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s --help\n", exe);
		printf("Usage: %s plain.txt publickey.pk\n", exe);
		printf("Usage: %s plain.txt -kp keypair.kp\n", exe);
		printf("Usage: %s plain.txt publickey.pk -o cipher.txt\n", exe);
		return 0;
}

int main(int argc, char **argv) {
	if (argc > 6) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc < 3) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	int i, kp_mode, found_plaintext, looking_ciphertext, custom_output;
	char *fplainname, *fkeyname, *fciphername;
	char buffer[11] = "cipher.txt";
	kp_mode = 0;
	found_plaintext = 0;
	looking_ciphertext = 0;
	custom_output = 0;
	fplainname = NULL;
	fkeyname = NULL;
	fciphername = NULL;
	for(i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--help") == 0) {
			print_usage(argv[0]);
			return 0;
		} else if (strcmp(argv[i], "-kp") == 0) {
			kp_mode = 1;
		} else if (strcmp(argv[i], "-o") == 0) {
			looking_ciphertext = 1;
			custom_output = 1;
		} else if (looking_ciphertext) {
			fciphername = argv[i];
			looking_ciphertext = 0;
		} else if (found_plaintext) {
			fkeyname = argv[i];
		} else {
			fplainname = argv[i];
			found_plaintext = 1;
		}
	}
	if (!custom_output) fciphername = buffer;
	if (!fplainname || !fkeyname || !fciphername) {
		print_usage(argv[0]);
		return 1;
	}

	keypair kp;

	if (kp_mode) {
		if (keypair_load(&kp, fkeyname)) return 1;
	} else {
		if (public_key_load(&kp.pk, fkeyname)) return 1;
	}

	return encrypt_file(fplainname, fciphername, &kp.pk);
}

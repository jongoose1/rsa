/* encrypt.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s plain.txt cipher.txt publickey.pk\n", exe);
		printf("Usage: %s plain.txt cipher.txt -kp keypair.kp\n", exe);
		return 0;
}

int main(int argc, char **argv) {
	if (argc > 5) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc < 4) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	
	keypair kp;
	/* plain, cipher, key */
	char *fnames[3];
	int kp_mode = 0, stage = 0, i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-kp") == 0) {
			kp_mode = 1;
			if (argc != 5) {
				printf("Invalid number of arguments\n");
				print_usage(argv[0]);
				return 1;
			}
		} else fnames[stage++] = argv[i];
	}

	if (kp_mode && keypair_load(&kp, fnames[2])) return 1;
	else if (public_key_load(&kp.pk, fnames[2])) return 1;
	return encrypt_file(fnames[0], fnames[1], &kp.pk);
}

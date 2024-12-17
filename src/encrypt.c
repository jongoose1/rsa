/* encrypt.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s plain.txt cipher.txt publickey.pk\n", exe);
		printf("Option: -s\n");
		printf("	Use stdin for plaintext. EOF (CTRL+D) signifies end of message.\n");
		printf("Option: -kp\n");
		printf("	Encrypt with keypair, e.g: %s plain.txt cipher.txt -kp keypair.kp\n", exe);
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
	char *non_option_args[3];
	int kp_mode = 0, stage = 0, i;
	int s_mode = 0;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-kp") == 0) {
			kp_mode = 1;
			if (argc != 5) {
				printf("Invalid number of arguments\n");
				return 1;
			}
		} else if (strcmp(argv[i], "-s") == 0) {
			s_mode = 1;
		} else non_option_args[stage++] = argv[i];
	}
	
	FILE *p, *c;
	char *fkeyname;

	if (s_mode) {
		p = stdin;
		c = fopen(non_option_args[0], "wb");
		if (!c) return 1;
		fkeyname = non_option_args[1];
	} else {
		p = fopen(non_option_args[0], "rb");
		c = fopen(non_option_args[1], "wb");
		if (!c || !p) return 1;
		fkeyname = non_option_args[2];
	}

	if (kp_mode && keypair_load(&kp, fkeyname)) return 1;
	else if (public_key_load(&kp.pk, fkeyname)) return 1;

	if (encrypt_stream(p, c, &kp.pk)) return 1;
	if (!s_mode) fclose(p);
	fclose(c);
	return 0;
}

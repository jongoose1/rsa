#include <stdio.h>
#include <string.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s file.txt signature.txt publickey.pk\n", exe);
		printf("Usage: %s file.txt signature.txt -kp keypair.kp\n", exe);
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
	
	/* file, signature, key */
	char *fnames[3];
	int kp_mode = 0, stage = 0, i;
	keypair kp;
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
	if (verify_file(fnames[0], fnames[1], &kp.pk)) printf("File has been verified by the signature.\n");
	else printf("File has NOT been verified by the signature.\n");
	return 0;
}

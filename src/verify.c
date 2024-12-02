/* verify.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	
	char *fname, *fsignname;
	keypair kp;
	
	fname = argv[1];
	fsignname = argv[2];
	if (strcmp(argv[3], "-kp") == 0) {
		if (argc != 5) {
			print_usage(argv[0]);
			return 1;
		}
		if (keypair_load(&kp, argv[4])) return 1;
	} else {
		if (public_key_load(&kp.pk, argv[3])) return 1;
	}

	if (verify_file(fname, fsignname, &kp.pk)) {
		printf("File has been verified by the signature.\n");
	} else {
		printf("File has NOT been verified by the signature.\n");
	}
	return 0;
}

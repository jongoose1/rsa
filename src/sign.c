#include <stdio.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s file.txt signature.txt keypair.kp\n", exe);
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

	char *fname, *fsignname, *fkeyname, password[64];
	fname = argv[1];
	fsignname = argv[2];
	fkeyname = argv[3];
	if (get_password(password, 64, "Enter password:") == -1) return 1;

	keypair kp;
	if (keypair_load(&kp, fkeyname)) return 1;
	decrypt_secret_key(password, &kp);

	return sign_file(fname, fsignname, &kp);
}

#include <stdio.h>
#include <unistd.h>
#include "rsa.h"

#define PSIZE 1024

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

	char  *fkeyname, password[PSIZE];
	FILE *fcipher = fopen(argv[1], "rb");
	if (!fcipher) return 1;
	fkeyname = argv[2];
	
	if (get_password(password, PSIZE, "Enter password:") == -1) return 1;

	keypair kp;
	if (keypair_load(&kp, fkeyname)) return 1;
	decrypt_secret_key(password, &kp);
	
	return decrypt_stream(fcipher, stdout, &kp);
}

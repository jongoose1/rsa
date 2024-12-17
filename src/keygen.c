#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"

int print_usage(char * exe) {
	printf("Usage: %s keypair.kp\n", exe);
	return 0;
}

int main(int argc, char ** argv) {
	srand(time(0));
	if (argc < 2) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc > 3) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	
	char password[64];
	if (get_password(password, 64, "Create a Password:") == -1) return 1;
	int nthreads = 1;
	if (argc == 3) {
		nthreads = atoi(argv[2]);
		if (nthreads < 1 || nthreads > 100) nthreads = 1;
	}
	keypair kp = keygen(nthreads);
	encrypt_secret_key(password, &kp);

	return keypair_save(&kp, argv[1]);
}

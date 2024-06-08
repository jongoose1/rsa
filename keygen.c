#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"

int print_usage(char * exe) {
	printf("Usage: %s mykeys.kp\n", exe);
	return 0;
}

int main(int argc, char ** argv) {
	srand(time(0));
	if (argc < 2) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc > 2) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}

	keypair keys = keygen();
	keypair_print(&keys);
	return keypair_save(&keys, argv[1]);
}

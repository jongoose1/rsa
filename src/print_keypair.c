#include <stdio.h>
#include "rsa.h"

int print_usage(char * exe) {
	printf("Usage: %s keypair.kp\n", exe);
	return 0;
}

int main(int argc, char ** argv) {
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
	keypair keys;
	keypair_load(&keys, argv[1]);
	keypair_print(&keys);
	return 0;
}

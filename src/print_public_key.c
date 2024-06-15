#include <stdio.h>
#include "rsa.h"

int print_usage(char * exe) {
	printf("Usage: %s public_key.pk\n", exe);
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
	public_key pk;
	public_key_load(&pk, argv[1]);
	public_key_print(&pk);
	return 0;
}

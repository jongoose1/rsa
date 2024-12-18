#include <stdio.h>
#include <string.h>
#include "rsa.h"

int print_usage(char * exe) {
		printf("Usage: %s file publickey.pk\n", exe);
		printf("Option: -kp\n");
		printf("	Hash with keypair, e.g: %s file -kp keypair.kp\n", exe);
		printf("Omit file to grab from stdin\n");
		return 0;
}

int main(int argc, char **argv) {
	if (argc > 4) {
		printf("Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	if (argc < 2) {
		printf("Too few arguments.\n");
		print_usage(argv[0]);
		return 1;
	}
	
	keypair kp;
	/* file, key */
	char *non_option_args[2];
	int kp_mode = 0, stage = 0, i;
	int s_mode = 0;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-kp") == 0) {
			kp_mode = 1;
		} else non_option_args[stage++] = argv[i];
	}
	if (stage == 1) s_mode = 1;
	else if (stage<1 || stage>2) {
		printf("Promblem parsing arguments\n");
		print_usage(argv[0]);
		return 1;
	}
	
	FILE *fp;
	char *fkeyname;

	if (s_mode) {
		fp = stdin;
		fkeyname = non_option_args[0];
	} else {
		fp = fopen(non_option_args[0], "rb");
		if (!fp) return 1;
		fkeyname = non_option_args[1];
	}

	if (kp_mode && keypair_load(&kp, fkeyname)) return 1;
	else if (public_key_load(&kp.pk, fkeyname)) return 1;

	bignum r = jg2_stream(fp, &kp.pk);
	bignum_print_256(&r);
	if (!s_mode) fclose(fp);
	return 0;
}

#include <stdio.h>

int gcd(int a, int b){
	int temp;
	while(1) {
		temp = a % b;
		if (temp == 0) return b;
		a = b;
		b = temp;
	}
}

#define PLAINTEXT 4
int main(void){
	unsigned long i;
	unsigned long e = 2;
	unsigned long p = 3;
	unsigned long q = 7;
	unsigned long n = p * q;
	unsigned long phi = (p-1)*(q-1);
	while ( e < phi ) {
		if (gcd(e, phi) == 1) break;
		else e++;
	}
	unsigned long k = 2;
	unsigned long d = (1 + (k * phi))/e;
	unsigned long msg = PLAINTEXT;

	printf("plaintext: %ld\n", msg);
	unsigned long c = msg;
	for(i = 1; i < e; i++){
		c = c * msg;
		c = c % n;
	}
	printf("ciphertext: %ld\n", c);

	unsigned long m = c;
	for(i = 1; i < d; i++) {
		m = m * c;
		m = m % n;
	}
	printf("plaintext: %ld\n", m);

	return 0;
}

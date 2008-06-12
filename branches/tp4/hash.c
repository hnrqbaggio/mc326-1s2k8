#include "hash.h"

int hashFunction(char * chave) {
	int h = 0, i;
	
	for (i = 0; i < strlen(chave); i++) {
		if (chave[i] != ' ') h += (chave[i] * (i + 1)) % H;
	}
	
	return h % H;
}

int hashDescritor(char descritor) {
	int n = 0, m;
	
	/* Faz & com potencias de 2 para ver quais bits do descritor estão ligados. */
	for (m = 1; m < 256; m*=2) {
		if (descritor & m) n++;
	}
	
	return n;
}

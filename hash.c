#include "hash.h"

int hashFunction(char * chave) {
	int h = 0, i;
	
	for (i = 0; i < strlen(chave); i++) {
		h += (chave[i] * (i + 1)) % H;
	}
	
	return h % H;
}

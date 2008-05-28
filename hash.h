#ifndef HASH_H_
#define HASH_H_

#include <string.h>
#define H 53

int hashFunction(char * chave) {
	int h = 0, i;
	
	for (i = 0; i < strlen(chave); ) {
		h += (chave[i] * (++i)) % H
	}
	
	return h % H;
}

#endif /*HASH_H_*/

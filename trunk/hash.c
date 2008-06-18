#include "hash.h"

int hashDescritor(char descritor) {
	int n = 0, m;
	
	/* Faz & com potencias de 2 para ver quais bits do descritor estão ligados. */
	for (m = 1; m < 256; m*=2) {
		if (descritor & m) n++;
	}
	
	return n;
}

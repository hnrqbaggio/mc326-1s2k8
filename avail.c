#include "avail.h"


int openAvail(const char *nomeArqAvail){
  int leitura;
  FILE *fav;

  fav = fopen(nomeArqAvail, "r");

  if (fav) {
    fscanf(fav, FORMATO_INT, &leitura);
    fclose(fav);
  } else { /* Sem arquivo eh lista vazia. */
    leitura = -1;
  }

  return leitura;
}

void gravaAvail(availList avail, const char *nomeArq) {
  FILE *fav;

  fav = fopen(nomeArq, "w"); /* Sobrescreve o arq antigo. */
  fprintf(fav, FORMATO_INT, avail);
  fclose(fav);
}

#include "avail.h"

int opcl_avail(FILE* avail){
  int leitura;

  /* Se o arquivo nao existe eu abro um novo */
  if(avail == NULL) fopen(avail, r+);

  else fclose (avail);

  fscanf(avail,"%d", &leitura);

    fclose(avail);
  return leitura;
}

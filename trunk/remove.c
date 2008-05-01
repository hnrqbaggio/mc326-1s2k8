#include "remove.h"

TIndice * removePk(char *chave, TIndice *indPrim, FILE *base, availList *avail) {
  ElementoIndice temp, *result;
  int offset, fim;

  strcpy(temp.pk, chave);
  temp.nrr = -1; /* Soh pra manter inicializado. */

  result = (ElementoIndice *) bsearch(&temp, indPrim->vetor, indPrim->tamanho, sizeof(ElementoIndice), compare);

  /* Apesar da chave ser valida, eh bom evitar um segfault por uso indevido da funcao. */
  if (result) {
    offset = result->nrr * TAM_REG;
    fseek(base, offset, SEEK_SET);

    /* Aponta o registro pro primeiro da avail list*/
    fprintf(base, FORMATO_INT, *avail);

    /* Atualiza a cabeca da avail list */
    *avail = result->nrr;

    /* Atualiza o indice. */
    fim = indPrim->tamanho-1;

    strcpy(temp.pk, result->pk);
    temp.nrr = result->nrr;

    strcpy(result->pk, indPrim->vetor[fim].pk);
    result->nrr = indPrim->vetor[fim].nrr;

    strcpy(indPrim->vetor[fim].pk, temp.pk);
    indPrim->vetor[fim].nrr = temp.nrr;

    (indPrim->tamanho)--;

    ordenaIndice(indPrim);

  } else {
    /* Essa fica pra dps. */
  }

  return indPrim;
}

IndSec * removeSk(char *chave, IndSec *indSecun, FILE *fsk, availList *avail){return 0;}

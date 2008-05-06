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

IndSec * removeSk(char *chave, IndSec *indSecun, char *pk, const int tipoCampo, availList *avail) {

  FILE *fsk;
  Sk temp, *result;
  char *token;
  int offset, prox, fim, atual, ant, tmp;
  /* Variaveis auxiliares para se trabalhar com a lista invertida. */

  switch (tipoCampo){
  case 0: /* Campo a ser lido eh o titulo. */
    fsk = fopen(ARQ_IS_TITULO,"r+");
    
    break;
  case 1: /* Campo Tipo */
    fsk = fopen(ARQ_IS_TIPO,"r+");
    
    break;
  case 2: /* Campo Autor */
    fsk = fopen(ARQ_IS_AUTOR,"r+");
    
    break;
  case 3: /* Campo Ano */
    fsk = fopen(ARQ_IS_ANO,"r+");
    
    break;
  }

  token = strtok(chave, " ");


  strcpy(temp.key, token);
  temp.next = -1;
  temp.lenght = strlen(token) + 2 * sizeof(int);

  result = (Sk *) bsearch(&temp, indSecun->vetor, indSecun->tamanho, sizeof(Sk), compareSk);

  if (result) {
    /* Deslocamento para escrever no arquivo.  
     * next * PK + cabecalho, pra poder escrever no campo numerico. */
    offset = result->next * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;
    
    /* Agora, temos q remover a chave da lista invertida. Ha uma
       particularidade quando a chave esta no primeiro elemento da
       lista, pois isso implica numa atualização da cabeca da lista (a
       SK) na RAM. Nos demais casos, fazemos uma atualização em
       posicoes no arquivo. */

    fseek(fsk, offset, SEEK_SET);
    fgets(token, TAM_TITULO, fsk);
    fscanf(fsk, FORMATO_INT, &prox);

    if (strcmp(pk, token)) { /* o primeiro elemento da lista sera removido. */

      tmp = *avail;
      *avail = result->next;
      fseek(fsk, -TAM_NUMERO, SEEK_CUR); /* Vai colocar o reg no inicio da lista invertida. */
      fprintf(fsk, FORMATO_INT, tmp);

      if (prox != -1) {
	result->next = prox;

      } else {

	/* Atualiza o indice. */
	fim = indSecun->tamanho-1;

	strcpy(temp.key, result->key);
	temp.next = result->next;

	strcpy(result->key, indSecun->vetor[fim].key);
	result->next = indSecun->vetor[fim].next;

	strcpy(indSecun->vetor[fim].key, temp.key);
	indSecun->vetor[fim].next = temp.next;

	(indSecun->tamanho)--;

	qsort(indSecun->vetor, indSecun->tamanho, sizeof(Sk), compareSk);
	
      }

      return indSecun;
    }

    /* Se chagamos aqui, eh pq o elemento procurado nao esta na primeira posicao. */

    atual = result->next;
    
    while (prox != -1) {

      /* Vai pra proxima posicao na lista invertida. */
      offset = prox * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;
      fseek(fsk, offset, SEEK_SET);

      fgets(token, TAM_TITULO, fsk);

      if (strcmp(pk, chave)) {
      
	atual = prox;
	fscanf(fsk, FORMATO_INT, &prox);

	/* Atualiza a avail list */
	fseek(fsk, -TAM_NUMERO, SEEK_CUR);
	fprintf(fsk, FORMATO_INT, *avail);
	*avail = atual;

	/* Atualiza a lista invertida. */
	offset = ant * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO + TAM_TITULO;
	fseek(fsk, offset, SEEK_SET);
	fprintf(fsk, FORMATO_INT, prox);

	/* Remocao concluida e tudo atualizado. */
	return indSecun;

      } else {

	/* Vai pro proxima posicao na lista e guarda um 'ponteiro' pra atual. */
	ant = prox;
	fscanf(fsk, FORMATO_INT, &prox);

      }

    }

  }

  /* Se chegamos a esse ponto, nao foi possivel remover. Deixamos o indice como estava. */
  fprintf(stderr, "\n\nNada foi removido!\n\n");

  return indSecun;
}



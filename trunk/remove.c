#include "remove.h"

IndicePrim * removePk(char *chave, IndicePrim *indPrim, FILE *base, availList *avail) {
  Pk temp, *result;
  int offset, fim;

  strcpy(temp.pk, chave);
  temp.nrr = -1; /* Soh pra manter inicializado. */

	/*Como o indice primario ja esta correto, devido a consulta realizada anteriormente, nao troco de indice primario*/
  result = (Pk *) bsearch(&temp, indPrim->vetor, indPrim->tamanho, sizeof(Pk), compare);

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

  } else {/*Erro desconhecido*/
    printf("Erro ao remover obra de arte.\n");
  }

  return indPrim;
}

/*Remove a chave do indice secundario*/
IndSec * removeSk(char *chave, IndSec *indSecun, char *pk, availList *avail) {

  FILE *arq;
  Sk temp, *result;
  char *token, pk2[TAM_TITULO+1], nomeArq[TAM_NOME_ARQ];
  int offset, prox, fim, atual, ant, tmp; /* Variaveis auxiliares para se trabalhar com a lista invertida. */

  /*Arquivo que contem as pks dos indices secundarios*/
  sprintf(nomeArq, "%s%s", indSecun->tipoCampo, EXTENSAO_PK);	
  arq = fopen(nomeArq, "r+");
   
   /*Jogo a chave para maiuscula para nao ocorrer conflito com o BIG FILE*/
   maiuscula(chave);
  token = strtok(chave, " ");


  while (token) {

    strcpy(temp.key, token);
    temp.next = -1;
    temp.lenght = strlen(token);

		/*Troco o indice secundario*/
		indSecun = trocaIndSec(indSecun, temp.key);
		
    result = (Sk *) bsearch(&temp, indSecun->vetor, indSecun->tamanho, sizeof(temp), compareSk);

    if (result) {
      /* Deslocamento para escrever no arquivo.  
       * next * PK + cabecalho, pra poder escrever no campo numerico. */
      offset = result->next * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;
    
      /* Agora, temos q remover a chave da lista invertida. Ha uma
	 particularidade quando a chave esta no primeiro elemento da
	 lista, pois isso implica numa atualização da cabeca da lista (a
	 SK) na RAM. Nos demais casos, fazemos uma atualização em
	 posicoes no arquivo. */

      fseek(arq, offset, SEEK_SET);
      fgets(pk2, TAM_TITULO+1, arq);
      fscanf(arq, FORMATO_INT, &prox);

      if (!strcmp(pk, pk2)) { /* o primeiro elemento da lista sera removido. */

	tmp = *avail;
	*avail = result->next;
	fseek(arq, -TAM_NUMERO, SEEK_CUR); /* Vai colocar o reg no inicio da lista invertida. */
	fprintf(arq, FORMATO_INT, tmp);

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
	
      } else {

	atual = result->next;
    
	while (prox != -1) {

	  /* Vai pra proxima posicao na lista invertida. */
	  offset = prox * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;
	  fseek(arq, offset, SEEK_SET);

	  fgets(pk2, TAM_TITULO+1, arq);

	  if (!strcmp(pk, pk2)) {
      
	    atual = prox;
	    fscanf(arq, FORMATO_INT, &prox);

	    /* Atualiza a avail list */
	    fseek(arq, -TAM_NUMERO, SEEK_CUR);
	    fprintf(arq, FORMATO_INT, *avail);
	    *avail = atual;

	    /* Atualiza a lista invertida. */
	    offset = ant * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO + TAM_TITULO;
	    fseek(arq, offset, SEEK_SET);
	    fprintf(arq, FORMATO_INT, prox);


	  } else {

	    /* Vai pro proxima posicao na lista e guarda um 'ponteiro' pra atual. */
	    ant = prox;
	    fscanf(arq, FORMATO_INT, &prox);

	  }

	}

      }

    }
    /* Pega o proximo token na string. */
    token = strtok(NULL, " ");
  }

  fclose(arq);
  return indSecun;
}



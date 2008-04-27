#include "busca.h"

TIndice * buscaMultipla(IndSec *indSecun, char *string) {
  int tam = 0, pos, offset;
  char *token;
  TIndice **lista = NULL; /* Vetor que contera ponteiros para as listas de cada token. */
  TIndice *intersecao;
  ElementoIndice *temp;
  Sk *chave, *res;

  FILE *fsk;
  fsk = fopen(ARQ_IS_TITULO, "r");

  chave = (Sk *) malloc(sizeof(Sk));

  token = strtok(string, " ");
  while (token) {
    tam++; /* Uma nova lista*/

    /* Aloco espaco pra um novo indice no vetor. */
    lista = (TIndice **) realloc(lista, sizeof(TIndice *) * tam);

    /* Inicializo a chave pra busca binaria. */
    strncpy(chave->key, token, strlen(token)+1);
    chave->lenght = strlen(token) + 2*sizeof(int);
    chave->next = -1;

    res = (Sk*) bsearch(chave, indSecun->vetor, indSecun->tamanho, sizeof(Sk), compareSk);

    if (res) {

      offset = res->next * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;

      /* Percorre a lista invertida carregando-a na memoria. */
      do {
	fseek(fsk, offset, SEEK_SET);
	fscanf(fsk, "%s", temp->pk);
	fscanf(fsk, FORMATO_INT, &(temp->nrr)); /* Nesse caso, o campo nrr
						eh o proximo elemento da
						lista invertida. */
	/* Ve se o indice precisa de mais espaco. */
	lista[tam] = realocaIndice(lista[tam]);

	pos = lista[tam]->tamanho;
	/*lista[tam]->vetor[pos].pk = temp->pk;*/
	lista[tam]->vetor[pos].nrr = temp->nrr;

	(lista[tam]->tamanho)++;

	/* Proximo elemento da lista. */
	offset = temp->nrr * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;
      } while (temp->nrr != -1);
    }

    /* Faz a intersecao das listas obtidas. */
    intersecao = intersection(lista);

    /* Imprime a saida. */


    /* Pega um novo token na string de busca. */
    token = strtok(NULL, " ");
  }
}

TIndice * intersection(TIndice ** lista){}

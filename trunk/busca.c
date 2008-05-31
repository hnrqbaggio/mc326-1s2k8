#include "busca.h"

void buscaSk(char *chave, IndicePrim *indPrim, IndSec *indSecun, FILE *base) {
  
  Sk temp, *result;
  Pk temp2, *result2;

  FILE *fhtml, *fsk;
  TObra reg;

  int offset;

  switch(indSecun->tipoCampo){
  case TITULO: /* Campo a ser lido eh o titulo. */
    fsk = fopen(ARQ_IS_TITULO,"r");
    break;
  case TIPO: /* Campo Tipo */
    fsk = fopen(ARQ_IS_TIPO,"r");
    break;
  case AUTOR: /* Campo Autor */
    fsk = fopen(ARQ_IS_AUTOR,"r");
    break;
  case ANO: /* Campo Ano */
    fsk = fopen(ARQ_IS_ANO,"r");
    break;
  }

  fhtml = fopen(ARQ_HTML, "w");
  
  /*Busco chave no indice secundario*/
  strcpy(temp.key, chave);
  temp.lenght = strlen(chave);
  temp.next = -1;

  result = (Sk*) bsearch(&temp, indSecun->vetor, indSecun->tamanho, sizeof(temp), compareSk);
  
  if (result) { /*Encontrou chave*/
    /*Cabecalho do HTML*/
    headHtml(fhtml);
    temp2.nrr = result->next;

    while (temp2.nrr != -1) {
      
      offset = temp2.nrr * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;
      fseek(fsk, offset, SEEK_SET);

      fgets(temp2.pk, TAM_TITULO+1, fsk);
      fscanf(fsk, FORMATO_INT, &(temp2.nrr));

      result2 = (Pk *) bsearch(&temp2, indPrim->vetor, indPrim->tamanho, sizeof(temp2), compare);

      offset = TAM_REG * result2->nrr;
      fseek(base, offset, SEEK_SET);

      /* leitura do registro */
      fgets(reg.titulo, TAM_TITULO + 1, base);
      fgets(reg.tipo, TAM_TIPO + 1, base);
      fgets(reg.autor, TAM_AUTOR + 1, base); 
      fgets(reg.ano, TAM_ANO + 1, base);
      fgets(reg.valor, TAM_VALOR + 1, base);
      fgets(reg.imagem, TAM_IMAGEM + 1, base);

      /*Insere no arquivo HTML*/
    	preencheHtml(fhtml, reg);
    }
    /*Finaliza as tags abertas do HTML*/
    endHtml(fhtml);
    
  } else {/*Nenhum registro encontrado*/
    printf("************************************\n\n");
    printf("Nenhum registro foi encontrado.     \n\n");
    printf("************************************\n\n");
  }

  fclose(fsk);
  fclose(fhtml);
}

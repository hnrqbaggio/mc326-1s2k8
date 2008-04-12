/* MC326 - Estrutura de Arquivos
 * TP2 - Catalogo de Obras de Arte
 * Grupo 24 */

/* 
 * Este programa implementa a gravacao dos dados de obras de arte
 * fornecidos pelo usuario num arquivo base.dat Tambem possibilita a
 * consulta de obras de arte atraves de uma pk, gerando o resultado em
 * html.
 */

#include <stdio.h>
#include "catalogo.h"
#include "leitura.h"

int main(int argc, char **argv){
  int ent;
  char option;
  FILE *arq;
  TIndice *ind;
  ElementoIndice *elem;
  char temp[TAM_TITULO + 1]; /* String auxiliar para a leitura das emtradas */

  arq = abreCatalogo(NOME_BASE);
  ind = carregaIndice(arq, ind);

  elem = (ElementoIndice *) malloc(sizeof(ElementoIndice));

  /*menu do programa*/
  printf("|------------------------------------|\n");
  printf("|Catalogo de Obras de Arte - Grupo 24|\n");
  printf("|------------------------------------|\n");

  do {
    printf("\n******************************\n");
    printf("* 1 - Inserir nova obra      *\n");
    printf("* 2 - Buscar obra por titulo *\n");
    printf("* 3 - Listar todas as obras  *\n");
    printf("* 4 - Sair                   *\n");
    printf("******************************\n");
    printf("Entre com umas das opcoes acima: ");
    scanf("%d", &ent);
		
    switch (ent) {
    case 1:
      /*Funcionalidades do TP1*/
      insereObra(arq, ind);
      break;

    case 2:
      do { 
	/* Leitura do nome a ser procurado. Completa com espaços o nome
	   para ter o mesmo tamanho que o nome gravado na base.dat*/
	leTexto(temp, sizeof(temp), "Digite o nome da obra: ");
	preencher(temp, sizeof(temp));

	strcpy(elem->pk, temp);
	elem->nrr = -1; /* apenas para inicializar. */

	/*Busca o nrr do pk e mostra no html*/
	consulta(elem, arq, ind);
	printf("\nDeseja fazer uma nova consulta? [s/n]: ");	    
      } while (scanf("%c", &option) == 1 && option != 'n');

      break;

    case 3:
      /*Mostra a base inteira em ordem ASCII, no html*/
      listaBase(arq, ind);
      break;

    case 4:
      /*fecha todos os arquivos abertos e libera memoria para sair do programa*/
      fechaCatalogo(arq);
      gravaIndice(ind);
      free(elem);
      break;
    }
  }
  while (ent != 4); 

  return 0;
}

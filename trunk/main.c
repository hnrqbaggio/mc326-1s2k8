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
#include "sk.h"

int main(int argc, char **argv){
   
  char temp[TAM_TITULO + 1];
  int ent;
  char option;
 
  /* Ponteiro pra base de dados. */
  FILE *arq;

  /* Variavais de indice primario e sua avail list da base de dados. */
  TIndice *ind;
  ElementoIndice *elem;
  availList availBase = FIM_DE_LISTA;

  /* Variaveis de indice secundario e respectivas avail lists. */
  IndSec *secTitulo, *secAutor, *secTipo, *secAno;
  availList availTitulo, availTipo, availAno, availAutor;
  availTitulo = availTipo = availAno = availAutor = FIM_DE_LISTA;

  arq = abreCatalogo(ARQ_BASE);
  ind = carregaIndice(arq, ind);

  secTitulo =  geraSk(ind, arq, &availTitulo, TITULO);
  secTipo =  geraSk(ind, arq, &availTipo, TIPO);
  secAutor =  geraSk(ind, arq, &availAutor, AUTOR);
  secAno =  geraSk(ind, arq, &availAno, ANO);

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
    printf("* 5 - Gerar SK's             *\n");
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
			gravaIndSk(secTitulo, TITULO);
			gravaIndSk(secTipo, TIPO);
			gravaIndSk(secAutor, AUTOR);
			gravaIndSk(secAno, ANO);
      break;
    }
  }
  while (ent != 4); 

  return 0;
}

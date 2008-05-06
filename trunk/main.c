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
#include "menu.h"
#include "remove.h"
#include "avail.h"

int main(int argc, char **argv){

  int ent, option;
  TObra obra, obra2, *consultaObra;
 
  /* Ponteiro pra base de dados. */
  FILE *arq;
  /* Ponteiro utilizado para abrir os arquivos de indice secundario*/
  FILE *fsk;

  /* Variavais de indice primario e sua avail list da base de dados. */
  TIndice *ind;
  ElementoIndice *elem;
  availList availBase = FIM_DE_LISTA;

  /* Variaveis de indice secundario e respectivas avail lists. */
  IndSec *secTitulo, *secAutor, *secTipo, *secAno;
  availList availTitulo, availTipo, availAno, availAutor;

  /*Abre a base e o indice primario*/
  arq = abreCatalogo(ARQ_BASE);
  ind = carregaIndice(arq, ind);
  availBase = openAvail(ARQ_AVAIL_BASE);

  /*Abre os indices secundarios*/
  secTitulo =  geraSk(ind, arq, &availTitulo, TITULO);
  secTipo =  geraSk(ind, arq, &availTipo, TIPO);
  secAutor =  geraSk(ind, arq, &availAutor, AUTOR);
  secAno =  geraSk(ind, arq, &availAno, ANO);

  /* Carrega as avail lists dos arquivos. */
  availTitulo = openAvail(ARQ_AVAIL_TITULO);
  availTipo = openAvail(ARQ_AVAIL_TIPO);
  availAutor = openAvail(ARQ_AVAIL_AUTOR);
  availAno = openAvail(ARQ_AVAIL_ANO);

  elem = (ElementoIndice *) malloc(sizeof(ElementoIndice));

  /*Entrada do programa*/
  printf("|------------------------------------|\n");
  printf("|Catalogo de Obras de Arte - Grupo 24|\n");
  printf("|------------------------------------|\n");

  /*Looping do menu do programa*/
  do {
    /*Imprime o menu principal e le a entrada do usuario*/
    ent = geraMenu();
		
    switch (ent) {
    case 1:/*Inserir nova obra*/
      /*leitura da obra a ser inserida e gravacao no indice primario*/
      obra = *(leObra(ind, &obra));
      /*Insercao nos indices secundarios*/
      /*titulo*/
      fsk = fopen(ARQ_IS_TITULO,"r+");
      strcpy(obra2.titulo, obra.titulo);
      secTitulo = insereSk(secTitulo, fsk, obra2.titulo, obra.titulo, &availTitulo);
      fclose(fsk);
      break;

    case 2:
      do {
	/*Imprime o menu de busca*/
	option = geraMenuBusca();
				
	switch (option) {
	  /*Aqui o codigo de busca multipla*/
	case 1:/*Busca pelo titulo*/
	  break;
						
	case 2:/*Busca pelo tipo*/
	  break;
						
	case 3:/*Busca pelo autor*/
	  break;
						
	case 4:/*Busca por ano*/
	  break;
						
	case 0:/*Menu anterior*/
	  break;
						
	default:
	  printf("\n*** Opcao invalida *** \n");
	  break;
	}
      } while (option != 0);
      break;

    case 3:
      /*Mostra a base inteira em ordem ASCII, no html*/
      listaBase(arq, ind);
      break;

    case 4: /* Remocao */
      leTexto(elem->pk, sizeof(elem->pk), "Digite a PK da Obra: ");
      preencher(elem->pk, sizeof(elem->pk));
      elem->nrr = -1;

      consultaObra = consulta(elem, arq, ind);
      ind = removePk(elem->pk, ind, arq, &availBase);
      /*Remove todas as Sks */
      secTitulo = removeSk(obra.titulo, secTitulo, elem->pk, TITULO, availTitulo);
      secAutor = removeSk(obra.autor, secAutor, elem->pk, AUTOR, availAutor);
      secTipo = removeSk(obra.tipo, secTipo, elem->pk, TIPO, availTipo);
      secAno = removeSk(obra.ano, secAno, elem->pk, ANO, availAno);

      break;

    case 0:
      /*fecha todos os arquivos abertos e libera memoria para sair do programa*/
      fechaCatalogo(arq);
      gravaIndice(ind);

      /* Grava os indices nos arquivos. */
      gravaIndSk(secTitulo, TITULO);
      gravaIndSk(secTipo, TIPO);
      gravaIndSk(secAutor, AUTOR);
      gravaIndSk(secAno, ANO);

      /* Grava as avail lists. */
      gravaAvail(availBase, ARQ_AVAIL_BASE);
      gravaAvail(availTitulo, ARQ_AVAIL_TITULO);
      gravaAvail(availTipo, ARQ_AVAIL_TIPO);
      gravaAvail(availAutor, ARQ_AVAIL_AUTOR);
      gravaAvail(availAno, ARQ_AVAIL_ANO);

      free(elem);
      break;
			
    default:
      printf("\n*** Opcao invalida *** \n");
      break;	
    }
  }
  while (ent != 0); 

  return 0;
}

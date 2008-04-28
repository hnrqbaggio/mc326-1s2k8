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

int main(int argc, char **argv){
   
  char temp[TAM_TITULO + 1];
  int ent;
  int option;
 
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

	/*Abre a base e o indice primario*/
  arq = abreCatalogo(ARQ_BASE);
  ind = carregaIndice(arq, ind);

	/*Abre os indices secundarios*/
  secTitulo =  geraSk(ind, arq, &availTitulo, TITULO);
  secTipo =  geraSk(ind, arq, &availTipo, TIPO);
  secAutor =  geraSk(ind, arq, &availAutor, AUTOR);
  secAno =  geraSk(ind, arq, &availAno, ANO);

  elem = (ElementoIndice *) malloc(sizeof(ElementoIndice));

  /*Entrada do programa*/
  printf("|------------------------------------|\n");
  printf("|Catalogo de Obras de Arte - Grupo 24|\n");
  printf("|------------------------------------|\n");

	/*Looping do menu do programa*/
  do {
		/*Imprime o menu principal e le a entrada do usuario*/
		ent = geraMenu(&ent);
		
		switch (ent) {
    case 1:/*Inserir nova obra*/
      /*Funcionalidades do TP1*/
      insereObra(arq, ind);
      break;

    case 2:
      do {
				/*Imprime o menu de busca*/
				option = geraMenuBusca(&option);
				
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

    case 0:
      /*fecha todos os arquivos abertos e libera memoria para sair do programa*/
      fechaCatalogo(arq);
      gravaIndice(ind);
      free(elem);
			gravaIndSk(secTitulo, TITULO);
			gravaIndSk(secTipo, TIPO);
			gravaIndSk(secAutor, AUTOR);
			gravaIndSk(secAno, ANO);
      break;
			
		default:
			printf("\n*** Opcao invalida *** \n");
			break;	
    }
  }
  while (ent != 0); 

  return 0;
}

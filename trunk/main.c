/* MC326 - Estrutura de Arquivos
 * Catalogo de Obras de Arte
 * Grupo 24 */

/***************  TP5 *************************/

#include <stdio.h>
#include "btree.h"


int main(int argc, char **argv){

  /*Variavies auxiliares*/
	BTree * btree;
  FILE * base;
   
  /*Abre base de dados*/
  base = fopen("base.dat", "r");
  
  /* Cria a estrutura da arvore. */
  btree = makeTree();
  
  /*Carrega root da arvore Caso nao exista, cria a partir da base.*/
	loadIndex(btree, base);
	
	
  
  printf("Good bye!\n");
  return 0;
}

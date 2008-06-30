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
  int option, chave, result, procura;
   
  /*Abre base de dados*/
  base = fopen("base.dat", "r");
  
  /* Cria a estrutura da arvore. */
  btree = makeTree();
  
  /*Carrega root da arvore Caso nao exista, cria a partir da base.*/
  loadIndex(btree, base);

  do {
    printf("1 - Insercao\n");
    printf("2 - Busca\n");
    printf("3 - Remocao\n");
    printf("0 - Sair\n");
    printf("Digite uma opção: ");
    scanf("%d", &option);

    switch(option) {
    case 1:
      printf("Digite a chave a ser inserida: ");
      scanf("%d", &chave);
      result = BTreeInsert(btree, chave);

      if (result) printf("Insercao bem sucedida!\n");
      else printf("Insercao mal sucedida. Verifique repeticao!\n");

      break;
    case 2: 
      printf("Digite a o numero a ser buscado: ");
      scanf("%i", &procura);
      result = search(&btree, procura);
      break;
    case 3:
      printf("Digite a chave a ser removida: ");
      scanf("%d", &chave);
      result = BTreeRemove(btree, chave);

      if (result) printf("Remocao bem sucedida!\n");
      else printf("Remocao mal sucedida.\n");
      break;
    case 0:
      break;
    }
  } while(option);
  
  /* Libera a memoria usada pela estrutura de arvore. */
  free(btree);
  free(base); /* E tambem o ponteiro do arquivo da base. */

  printf("Good bye!\n");
  return 0;
}

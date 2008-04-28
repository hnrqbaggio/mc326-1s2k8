#include "menu.h"

int geraMenu(int *ent) {
		

  printf("\n******************************\n");
  printf("* 1 - Inserir nova obra      *\n");
	printf("* 2 - Buscar obra            *\n");
	printf("* 3 - Listar todas as obras  *\n");
	printf("* 0 - Sair                   *\n");
	printf("******************************\n");
	printf("Entre com umas das opcoes acima: ");
	scanf("%d", ent);
	
	return *ent;
}

int geraMenuBusca(int *ent) {
	
	printf("\n\n******************************\n");
	printf("*   1 - Buscar pelo titulo      *\n");
	printf("*   2 - Buscar pelo tipo        *\n");
	printf("*   3 - Buscar pelo autor       *\n");
	printf("*   4 - Buscar pelo ano         *\n");
	printf("*   0 - Voltar ao menu anterior *\n");
	printf("******************************\n");
	printf("Entre com umas das opcoes acima: ");
	scanf("%d", ent);
	
	return *ent;
}

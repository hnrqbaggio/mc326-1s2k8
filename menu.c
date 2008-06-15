#include "menu.h"

void menuBoasVindas(){
  printf("\n===========================================\n");
  printf("= Catalogo de Obras de Artes - Versao 4.0 =\n");
  printf("===========================================\n");
}

void menuInicio() {
	printf("\n== Iniciando Catalogo de Obras de Arte. ==\n");
}

int geraMenu() {
	
	int ent;

   	printf("\n=============================\n");
   	printf("=   1 - Inserir nova obra   =\n");
	printf("=   2 - Buscar obra         =\n");
	printf("=   3 - Remover registro    =\n");
	printf("=   0 - Sair                =\n");
	printf("=============================\n");
	printf("Entre com umas das opcoes acima: ");
	scanf("%d", &ent);
	
	return ent;
}

int geraMenuBusca() {
	
	int ent;
	
	printf("\n==================================\n");
	printf("= 1 - Buscar pelo titulo         =\n");
	printf("= 2 - Buscar pelo tipo           =\n");
	printf("= 3 - Buscar pelo autor          =\n");
	printf("= 4 - Buscar pelo ano            =\n");
  	printf("= 5 - Buscar por titulo completo =\n");
  	printf("= 6 - Buscar por conteudo        =\n");
  	printf("= 0 - Voltar ao menu anterior    =\n");
	printf("==================================\n");
	printf("Entre com umas das opcoes acima: ");
	scanf("%d", &ent);
	
	return ent;
}

int geraNotificaInsere() {
  char ent2;
	
  printf("Deseja inserir uma nova obra? [s/n]: ");
  scanf("%c", &ent2);

  return ((ent2 == 'n') ? 0 : 1);
}

void printSearchSuccess() {
	 printf("\n-----------------------------------------------------\n");
    printf("A consulta esta disponivel no arquivo %s\n", ARQ_HTML);
    printf("-----------------------------------------------------\n");
}

void printSearchFailed() {
    printf("\n-------------------------------\n");
    printf("Nenhum registro foi encontrado.\n");
    printf("-------------------------------\n");
}

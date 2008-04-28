#include <stdio.h>


/**
  * Funcao que gera menu principal para que usuario escolha a opcao desejada:
  * inserir, buscar, mostrar todas as obras, sair.
  * O retorno é a opcao escolhinda
*/
int geraMenu (int *);

/**
  * Funcao que gera menu de busca para que usuario escolha o tipo de busca a ser realizada:
  * titulo, tipo, autor, ano e as opcoes de voltar ao menu anterior e sair do programa.
  * A opcao de remocao sera dada ao usuario apos ele consultar uma obra do catalogo.
  * O retorno é a opcao escolhinda
*/
int geraMenuBusca (int *);

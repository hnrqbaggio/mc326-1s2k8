#include <stdio.h>


/**
  * Funcao que gera menu principal para que usuario escolha a opcao desejada:
  * inserir, buscar, mostrar todas as obras, sair.
  * O retorno é a opcao escolhinda
*/
int geraMenu ();

/**
  * Funcao que gera menu de busca para que usuario escolha o tipo de busca a ser realizada:
  * titulo, tipo, autor, ano e as opcoes de voltar ao menu anterior e sair do programa.
  * A opcao de remocao sera dada ao usuario apos ele consultar uma obra do catalogo.
  * O retorno é a opcao escolhinda
*/
int geraMenuBusca ();

/**
  * Funcao que exibe notificacao de obra inserida e
  * verifica se o usuario deseja inserir novamente uma obra.
  * O retorno é 1 ou 0, de acordo se o usuario quer ou nao
  * inserir uma nova obra.
*/
int geraNotificaInsere ();

/**
 * Funcao que exibe notificacao de obra nao inserida pois
 * ja esta catalogada e informa a opcao de insercao.
 * O retorno é 1 ou 0, de acordo se o usuario quer ou nao
 * inserir uma nova obra.
 */
int geraNotificaErroInsere ();

/**
 * Funcao que exibe notificacao de remocao concluida.
 */
void geraNotificaRemocao();

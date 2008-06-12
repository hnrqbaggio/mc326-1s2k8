/** @file menu.h
 * @brief Biblioteca que implementa as funcoes de geracao de menu para interacao com o usuario. 
*/

#include <stdio.h>
#include "constantes.h"

/** @brief Funcao que gera menu principal para que usuario escolha a opcao desejada.
 * 
 * @return int A opcao escolhida.
*/
int geraMenu ();

/** @brief Funcao que gera menu de busca para que usuario escolha o tipo de busca a ser realizada.
 * 
 * @return int A opcao escolhida.
*/
int geraMenuBusca ();

/** @brief Funcao que exibe notificacao de obra inserida e
 * verifica se o usuario deseja inserir novamente uma obra.
 * 
 * @return int 1 para usuario inserir nova obra, 0 para nao inserir.
*/
int geraNotificaInsere ();

/** @brief Funcao que exibe que o registro buscado esta disponivel
 * no arquivo html. 
 * 
 * @return void
*/
void printSearchSuccess();

/** @brief Funcao que exibe que a busca nao encontrou registros. 
 * 
 * @return void
*/
void printSearchFailed();

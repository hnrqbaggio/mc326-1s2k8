/** @file sk.h
 * @brief Biblioteca relativa aos indices secundarios.
*/

#ifndef _SK_H
#define _SK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "catalogo.h"

/** @{
 * @name Nomes dos arquivos de indices secundarios.
 */
#define ARQ_IS_TITULO ("titulo.si")
#define ARQ_IS_TIPO ("tipo.si")
#define ARQ_IS_AUTOR ("autor.si")
#define ARQ_IS_ANO ("ano.si")
/** @} */

/** @{
 * @name Nomes dos arquivos das avail lists.
 */
#define ARQ_AVAIL_TITULO ("titulo.av")
#define ARQ_AVAIL_TIPO ("tipo.av")
#define ARQ_AVAIL_AUTOR ("autor.av")
#define ARQ_AVAIL_ANO ("ano.av")
/** @} */

/** @{
 * @name Constantes usadas para determinar o tipo do campo para o indice
 */
#define TITULO 0
#define TIPO 1
#define AUTOR 2
#define ANO 3
/** @} */

/** Constante usada para indicar o fim de uma lista invertida. */
#define FIM_DE_LISTA -1

/**
 * @brief Estrutura que representa um chave secundaria.
*/
typedef struct _sk{
  char key[TAM_TITULO+1]; /**< Vetor que contem a string que eh a chave propriamente. */
  int next; /**< Proxima chave no indice. */
  int lenght; /**< Tamanho da string. */
} Sk;

/**
 * @brief Estrutura que representa um indice secundario.
 */
typedef struct _vetorsk{
  Sk *vetor; /**< Vetor dinamico de chaves secundarias. */
  int tamanho; /**< Numero de elementos validos no vetor. */
  int alocado; /**< Tamanho alocado para o vetor. */
  int tamDisco; /**< Tamanho do arquivo em disco de PKs. */
} IndSec;


/**
 * @brief Constroi um indice secundario.
 * 
 * Se existe o arquivo de indice, ele carrega parte de estrutura pra memoria 
 * e deixa a outra no disco. Caso nao exista o indice, ele ira constri-lo 
 * a partir da base de dados. Para isso, ela utiliza as funcoes auxiliares 
 * carregaSk e criaSk.
 * 
 * @param indPrim Indice primario.
 * @param base Base de dados.
 * @param avail Avail list da base de dados.
 * @param tipoCampo Constante que indica o tipo de campo cujo indice sera gerado.
 * @return Indice secundario.
 */
IndSec * geraSk(TIndice *indPrim, FILE *base, availList *avail, const int tipoCampo);

/**
 * @brief Carrega um indice secundario a partir do arquivo em disco.
 * 
 * Armazena a parte que ficara na RAM em um vetor de SKs e retorna o
 * ponteiro para a estrutura de indices secundarios.
 *
 * @param arqSk Arquivo de indice secundario.
 * @param avail Avail list do arquivo.
 * @return Indice secundario.
 */
IndSec * carregaSk(FILE *arqSk, availList *avail);

/**
 * @brief Cria o indice secundario a partir da base de dados, carregando as
 * informacoes de cada registro.
 *
 * Essa funcao aloca espaco pro vetor de SKs. Ha uma funcao especifica
 * para liberar esse espaco.
 * 
 * @param indPrim Indice primario.
 * @param base Base de dados.
 * @param avail Avail list da base de dados.
 * @param tipoCampo Constante que indica o tipo de campo cujo indice sera gerado.
 * @return Indice Secundario.
 */
IndSec * criaSk(TIndice *indPrim, FILE *base, availList *avail, const int tipoCampo);

/**
 * @brief Insere uma SK no vetor que contem o indice secundario pra um
 * determinado campo, e atualiza a lista invertida no disco.
 *
 * @param indSecun Indice secundario.
 * @param fsk Arquivo de indice secundario.
 * @param pk String com a chave primaria do registro.
 * @param campo String com a palavra (token).
 * @param Avail List do arquivo de indice.
 * @return Indice secundario.
 */
IndSec * insereSk(IndSec *indSecun, FILE *fsk, char *pk, char *campo, availList *avail);

/**
 * @brief Grava o indice secundário da RAM para o arquivo no final deste, 
 * e o tamanho do arquivo que sempre esta em disco.
 * 
 * Libera a memoria utilizada nesses e fecha o arquivo de SK.
 *
 * @param sec Indice secundario. 
 * @param tipoCampo Constante que define o tipo do indice.
 * @return void. 
*/
void gravaIndSk(IndSec *sec, const int tipoCampo);

/**
 * @brief Funcao usada para realocar espaco para o vetor da SK caso seja necessario. 
 * 
 * Ela ira verificar se o numero de elementos eh sufuciente para o espaco alocado 
 * para o vetor e reservar mais caso seja necessario.
 *
 * @param sec Indice secundario.
 * @return sec Indice secundario. 
 */
IndSec * realocaIndSec(IndSec *sec);

/**
 *@brief Funcao usada para comparacao de sks. Usadas por qsort e bsearch.
 * 
 * Mesmo padrao que a funcao compare, contida em 'catalogo.h'.
 * 
 * @param a Parametro a ser comparado.
 * @param b Parametro a ser comparado.
 * @return int Retorna se os parametros sao iguais ou nao.
*/
int compareSk(const void *a, const void *b);

#endif

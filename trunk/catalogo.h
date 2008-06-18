/** @file catalogo.h
 * @brief Biblioteca que implementa as funcoes relacionadas a base de dados,
 * ao indice primario e a geracao do arquivo html. 
*/

#ifndef _CATALOGO_H
#define _CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "hash.h"
#include "avail.h"

/** Funcoes de manipulacao da base de dados **/
  
/**
 * @brief Abre um arquivo que sera a base de dados. 
 * 
 * @param nome Uma string contendo o nome do arquivo.
 * @return Um ponteiro para a base de dados.
 */
FILE * abreCatalogo(char * nome);

/**
 * @brief Fecha o arquivo da base de dados.
 * @param base O ponteiro para o arquivo da base de dados.
 */
void fechaCatalogo(FILE * base);

/**
 * @brief Grava uma obra na base de dados. 
 * @param obra a ser inserida.
 * @param base o ponteiro pro arquivo da base de dados.
 * @param avBase A avail list da base de dados.
 * @param primario O indice primario.
 * @return O nrr da obra inserida ou o tamanho da base.
 */
int gravaObra(TObra obra, FILE * base, availList * avBase, Index * primario);


/*** Funcoes de manipulacao de indice ***/
/** 
 * @brief Carrega o indice primario do arquivo para a memoria. 
 * Cria o indice caso o arquivo nao exista.
 * 
 * @param base A base de dados.
 * @param primario O indice primario que sera carregado.
 * 
 * @return Um ponteiro para o indice primario atualizado.
 */
Index * iniciaPk(FILE * base, Index * primario);

/**
  * @brief Carrega o indice primario para a memoria de acordo com o id especificado.
  * 
  * @param indice O indice primario alocado em memoria.
  * @param id Id do indice a ser carregado.
  */
void carregaIndicePrim(Index * indice, int id);

/**
 * @brief Grava o indice primario da memoria para o arquivo. 
 * 
 * @param indice Indice a ser gravado.
*/
void gravaIndicePrim(Index *indice);

/** @brief Inicializa um indice, alocando memoria 
 * para a sua propria estrutura e tambem para o vetor de keys.
 * Tambem inicializa os demais campos do indice com valores 
 * apropriados.
 * 
 * @param tipoIndice Uma string que determina o campo da obra
 * ao qual o indice se refere.
 * 
 * return O Indice alocado com valores iniciais.
 */
Index * makeIndice(char *tipoIndice);

/** 
 * @brief Realiza a ordenacao do indice.
 * 
 * @param indice O indice a ser ordenado.
 */
void ordenaIndice(Index * indice);

/** @brief Funcao usada para realocar espaco para o vetor do indice caso seja necessario. 
 * 
 * Ela verifica se o numero de elementos e sufuciente para o espaco alocado para o vetor 
 * e reservar mais caso seja necessario.
 *
 * @param indice O indice a ser realocado ou nao. 
 */
void realocaIndice(Index *indice);

/**
 * @brief Troca de indice de acordo com a id do arquivo a ser aberto.
 * 
 * @param indice Indice a ser trocado.
 * @param id Id do arquivo a ser aberto e carregado para a memoria.
*/
void trocaIndice(Index * indice, int id);

/**
 * @brief Libera os indices e vetores alocados na utilizacao do programa.
 * 
 * @param indPrim Indice primario.
 * @param indTitulo Indice secundario do titulo.
 * @param indTipo Indice secundario do tipo.
 * @param indAutor Indice secundario do autor.
 * @param indAno Indice secundario do ano.
 * @param indDescritor Indice secundario do descritor de imagens.
 * @return void
*/
void liberaIndices(Index *indPrim, Index *indTitulo, Index *indTipo,
Index *indAutor, Index *indAno, IndDesc *indDescritor);

/** 
 * @brief Funcao de comparacao para as funcoes qsort e bsearch da stdlib.h.
 * 
 * Foi possivel usar uma unica funcao pois para a bsearch passamos nao
 * uma string, mas um elemento de indice, que eh do mesmo tipo que os
 * elementos do vetor no qual a busca eh feita. Isso permitiu usar uma
 * funcao de comparacao com ambos os parametros iguais, como ocorre
 * com a qsort.
 * 
 * Os parametros da funcao recebem um cast para
 * Pk * e tem seus campos pk copiadaos para strings
 * auxiliares, as quais sao comparadas usando strcmp.
 * 
 * @param a Parametro a ser comparado.
 * @param b Parametro a ser comparado.
 * @return int Retorna se os parametros sao iguais ou nao.
 */
int compare(const void *a, const void *b);

/**
 * @brief Esta funcao realiza a leitura de um registro com base no valor 
 * do seu NRR obtido atraves do indice primario.
 * 
 * @param obra Um ponteiro para a estrutura onde serao armazenados os dados lidos.
 * @param nrr O numero relativo do registro na base de dados.
 * @param base A base de dados. 
 */
void leRegistro(TObra *obra, int nrr, FILE *base);

#endif

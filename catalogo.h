/** @file catalogo.h
 * @brief Biblioteca que implementa as funcoes relacionadas a base de dados,
 * ao indice primario e a geracao do arquivo html. 
*/

#ifndef _CATALOGO_H
#define _CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int gravaObra(TObra obra, FILE * base, availList * avBase, IndPrim * primario);


/*** Funcoes de manipulacao do indice ***/
/** 
 * @brief Carrega o indice primario do arquivo para a memoria. 
 * Cria o indice caso o arquivo nao exista.
 * 
 * @param base A base de dados.
 * @param indice O indice primario que sera carregado.
 * 
 * @return Um ponteiro para o indice primario atualizado.
 */
IndPrim * iniciaPk(FILE * base, IndPrim * indice);

/**
  * @brief Abre o arquivo de indice primario correspondente ao valor de hash 
  * e carrega pra RAM.
  * 
  * @param indice O indice primario.
  * @return O indice atualizado.
  */
IndPrim * abrePk(IndPrim * indice);

/**
 * @brief Grava o indice da memoria para o arquivo. 
 * 
 * @param indice Indice primario.
 * @return Indice primario.
*/
IndPrim * gravaPk(IndPrim *indice);

/** 
 * @brief Realiza a ordenacao de indice um indice primario.
 * @param indice O indice a ser ordenado.
 * @return O indice ordenado.
 */
void ordenaIndice(IndPrim * indice);

/** @brief Funcao usada para realocar espaco para o vetor do indice caso seja necessario. 
 * 
 * Ela verifica se o numero de elementos e sufuciente para o espaco alocado para o vetor 
 * e reservar mais caso seja necessario.
 *
 * @param ind Indice Primario.
 * @return Indice Primario.  
 */
IndPrim * realocaIndPrim(IndPrim *ind);

/**
 * @brief Troca os indices primarios.
 * 
 * @param indice Indice primario.
 * @param chave String a ser calculada o valor do hash para ser aberto o
 * indice correspondente.
 * @return Indice primario.
*/
IndPrim * trocaIndPrim(IndPrim *, char *);

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
void liberaIndices(IndPrim *indPrim, IndSec *indTitulo, IndSec *indTipo,
IndSec *indAutor, IndSec *indAno, IndDesc *indDescritor);

/** 
 * @brief Realiza uma consulta na base usando a chave primaria.
 * 
 * @param chave String a ser buscado no indice primario.
 * @param base Base de dados.
 * @param indice Indice primario.
 * @return A obra de dados encontrada ou NULL caso nao a busca nao encontre resultados. 
 */
TObra * consulta(Pk *chave, FILE *base, IndPrim *indice);

/** 
 * @brief Lista todos os registros da base de dados. 
 * 
 * Percorre o vetor do indice primario e para cada 
 * elemento dele, pega os dados na base. Os resultados sao 
 * passados pra funcao que gera o HTML. Faz essa operacao 
 * para cada arquivo de indice primario.
 * 
 * @param base Base de dados.
 * @param indice Indice primario.
 * @return void
 */
void listaBase(FILE *base, IndPrim *indice);

/** @brief Insere uma obra na tabela de obras de arte do arquivo HTML.
 * 
 * @param b Arquivo html.
 * @param valores Obra de arte a ser mostrada no arquivo.
 * @return Arquivo html.
 */
FILE * preencheHtml(FILE *b, TObra valores);

/** @brief Gera cabecalho do arquivo html.
 * 
 * @param b Arquivo html.
 * @return Arquivo html.
 */
FILE * headHtml(FILE *b);

/** @brief Gera o final do html.
 * 
 * @param b Arquivo html.
 * @return Arquivo html.
 */
FILE * endHtml(FILE *b);

/** @brief Transforma para maiuscula a string passada como parametro.
 * 
 * @param chave String a ser mudada para maiuscula.
 * @return void.
*/
void maiuscula(char *chave);

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

#endif

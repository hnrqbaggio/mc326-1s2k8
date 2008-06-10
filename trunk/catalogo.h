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
 * @param indice O indice primario que será carregado.
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
 * @brief Realiza a ordenacao de indice um indice primario.
 * @param indice O indice a ser ordenado.
 * @return O indice ordenado.
 */
void ordenaIndice(IndPrim * indice);

/* Grava o indice da memoria para o arquivo. */
IndPrim * gravaPk(IndPrim *);

/* 
 * Realiza uma consulta na base usando a chave primaria.
 * Recebe a pk, o base de dados, o indice primario. 
 */
TObra * consulta(Pk *, FILE *, IndPrim *);

/* 
 * Lista todos os registros da base de dados. Ela percorre o vetor do
 * indice passado como paramentro e para cada elemento dele, busca os
 * dados no primeiro parametro, que eh a base de dados. Os resultados
 * sao passados pra funcao que gera o HTML.
 */
void listaBase(FILE *, IndPrim *);

/*** Funcoes auxiliares. ***/
/* 
 * Funcao de comparacao para as funcoes qsort e bsearch da stdlib.h.
 * Foi possivel usar uma unica funcao pois para a bsearch passamos nao
 * uma string, mas um elemento de indice, que eh do mesmo tipo que os
 * elementos do vetor no qual a busca eh feita. Isso permitiu usar uma
 * funcao de comparacao com ambos os parametros iguais, como ocorre
 * com a qsort.  

 * Os parametros da funcao recebem um cast para
 * Pk * e tem seus campos pk copiadaos para strings
 * auxiliares, as quais sao comparadas usando strcmp.
 */
int compare(const void *a, const void *b);

/* 
 * Funcao usada para realocar espaco para o vetor do indice caso seja
 * necessario. Ela ira verificar se o numero de elementos eh
 * sufuciente para o espaco alocado para o vetor e reservar mais caso
 * seja necessario. 
 */
IndPrim * realocaIndPrim(IndPrim *ind);

/** 
 * Preenche as tabelas do HTML fornecido como parametro com a obra tambem fornecida
 * e retorna o arquivo HTML.
 */
FILE * preencheHtml(FILE *,TObra);

/**
 * Gera cabecalho do arquivo HTML.
 */
FILE * headHtml(FILE *);

/**
 * Gera o final do HTML.
 */
FILE * endHtml(FILE *);

/**
 * Troca os indices primarios.
 * Recebe o indice prmario e a chave a ser calculada o hash*/
IndPrim * trocaIndPrim(IndPrim *, char *);

/**
 * Transforma para maiuscula a string passada como parametro
 * 
*/
void maiuscula(char *);

/**
 * Libera os indices e vetores alocados na utilizacao do programa
 * 
*/
void liberaIndices(IndPrim *, IndSec *, IndSec *, IndSec *, IndSec *);

#endif

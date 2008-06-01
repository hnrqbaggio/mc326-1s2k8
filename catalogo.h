#ifndef _CATALOGO_H
#define _CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"
#include "avail.h"

/*** Funcoes de manipulacao da base de dados ***/

/* 
 * Abre um arquivo que sera a base de dados do programa e retorna um
 * ponteiro pra esse arquivo.  A string passada como argumento eh o
 * nome do arquivo no disco.
 */
FILE * abreCatalogo(char *);

/* Fecha o arquivo da base de dados bassado como parametro. */
void fechaCatalogo(FILE *);

/* 
 * Grava uma obra na base de dados. Os parametros sao a obra e o
 * ponteiro pro arquivo da base e a avail list da base.
 * Retorna o nrr da obra inserida.
 */
int gravaObra(TObra, FILE *, availList *);


/*** Funcoes de manipulacao do indice ***/
/* Carrega o indice do arquivo para a memoria. Cria o indice caso o
 * arquivo nao exista. 
 */
IndicePrim * iniciaPk(FILE *, IndicePrim *);

/**
  * Abre o arquivo de indice primario correspondente ao valor de hash 
  * e carrega pra RAM. Fecha o arquivo e retorna o indice primario.
  * O valor de hash e passado na propria estrutura de indice primario.
*/
IndicePrim * abrePk(IndicePrim *);

/* Realiza a ordenacao do indice passado como parametro. */
void ordenaIndice(IndicePrim *);

/* Grava o indice da memoria para o arquivo. */
void gravaPk(IndicePrim *);

/* 
 * Realiza uma consulta na base usando a chave primaria.
 * Recebe a pk, o base de dados, o indice primario. 
 */
TObra * consulta(Pk *, FILE *, IndicePrim *);

/* 
 * Lista todos os registros da base de dados. Ela percorre o vetor do
 * indice passado como paramentro e para cada elemento dele, busca os
 * dados no primeiro parametro, que eh a base de dados. Os resultados
 * sao passados pra funcao que gera o HTML.
 */
void listaBase(FILE *, IndicePrim *);

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
IndicePrim * realocaIndPrim(IndicePrim *ind);

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
 * O arquivo que sera aberto tem seu valor de hash armazenado no indice*/
IndicePrim * trocaIndPrim(IndicePrim *);

#endif

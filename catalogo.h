#ifndef _CATALOGO_H
#define _CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"
#include "avail.h"

/* Definicoes de tamanhos de campos e do tamanho do registro */
#define TAM_TITULO 200
#define TAM_TIPO 100
#define TAM_AUTOR 125
#define TAM_ANO 4
#define TAM_VALOR 12
#define TAM_IMAGEM 9
#define TAM_REG (TAM_TITULO+TAM_TIPO+TAM_AUTOR+TAM_ANO+TAM_VALOR+TAM_IMAGEM)

/* Defines para os nomes dos arquivos.*/
#define ARQ_BASE ("/tmp/base00.dat")       /* Base de dados */
#define ARQ_HTML ("/tmp/base24.html")      /* Html para saida dos resultados de busca */
#define ARQ_PK ("primario")                /* Arquivo que contem os registros do indice primario */
#define ARQ_AVAIL_BASE ("base24.av")       /* Avail list da base de dados */
#define EXTENSAO_PK (".chp")
/* 
 * Constante para fazer reallocs mais eficientes.  a primeira alocacao
 * sera feita com esse valor, e dobrando sempre que atingir o maximo 
 */
#define VETOR_MIN 20

/* Define para a mascara usada para ler e gravar inteiros. */
#define FORMATO_INT ("%08d")
#define TAM_NUMERO 8

/* Constantes usadas como parametros na funcao geraHtml */
#define ALL 0  /* Adiciona tanto o cabecalho quanto o fim do
		* html. usado quando eh uma consulta de um registro
		* especifico. */
#define HEAD 1 /* Deve ser inserido um cabecalho no html */
#define END 2  /* Deve fechar as tags do html: </table></body></html> */
#define MEIO 3 /* Diferencia os registros que nao sao nem inicio nem
		* fim da listagem, logo, devem apenas ser inseridos */

/* 
 * TAD que usaremos para representar uma obra.
 * Um char a mais em cada campo pro '\0'. 
 */
typedef struct obra {
  char titulo[TAM_TITULO + 1];
  char tipo[TAM_TIPO + 1];
  char autor[TAM_AUTOR + 1];
  char ano[TAM_ANO + 1];
  char valor[TAM_VALOR + 1];
  char imagem[TAM_IMAGEM + 1];
}TObra;

/* 
 * TAD usado para representar um elemento do indice.  PK eh uma string
 * que contera a chave primaria do registro, e nrr sera o seu numero
 * relativo de registro, correspondente a posicao deste no arquivo da
 * base.
 */
typedef struct _pk {
  char pk[TAM_TITULO + 1];
  int nrr;
} Pk;

/* TAD que representara o indice */
/* 
 * Vetor eh um vetor dinamico que ira armazenar os elementos de indice.  

 * Tamanho eh o numero de elementos validos, assim, indica
 * tambem a proxima posicao livre do vetor. Sera usado nas funcoes de
 * insercao, ordenacao e busca. 
 
 * Alocado eh o espaco alocado para o vetor atraves da alocacao dinamica com malloc e 
 * usado para controlar quando realocar espaco para o vetor. 
 */
typedef struct struct_vet {
  Pk * vetor;
  int tamanho;
  int alocado;
  int tamBase;
  int valorHash;
} IndicePrim;

/**
 * Este inteiro será usado para representar a cabeca de uma avail
 * list, que sera um alista invertida de posicoes livres no disco.
 */
typedef int availList;

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
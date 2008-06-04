#ifndef TIPOS_H_
#define TIPOS_H_
#include "constantes.h"
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
typedef struct _vetorPk {
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

/**
 * Estrutura que representa um chave secundaria.
 * 
 * key eh o vetor que contem a string que eh a chave propriamente
 * dita. next eh um apontador para a proxima chave no indice, e lenght
 * eh o tamanho da string.
*/
typedef struct _sk{
  char key[TAM_TITULO+1];
  int next;
  int lenght;
} Sk;

/**
 * Estrutura que representa um indice secundario, formado por um vetor
 * de chaves secundarias e quatro inteiros que indicam o numero de
 * elementos no indice, o espaco alocado na memoria para este indice,
 * o tamanho da parte do indice no disco, e o noh cabeca da avail list
 * desse indice, que sera usado na operacao de insercao e remocao de
 * chaves.
 */
typedef struct _vetorSk {
  Sk *vetor;
  int tamanho;
  int alocado;
  int tamDisco;
  int valorHash;
  char tipoCampo[TAM_NOME_ARQ];
} IndSec;


/* Tipos de dados para os descritores das imagens. */
typedef struct _descritor {
	char pk[TAM_TITULO+1];
	char valorDescritor;
	double similaridade;
	char imagem[TAM_IMAGEM];
} Descritor;

typedef struct _vetorDescritor {
	Descritor *vetor;
	int tamanho;
	int alocado;
	int valorHash;
} IndDesc;

#endif /*TIPOS_H_*/

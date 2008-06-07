#ifndef TIPOS_H_
#define TIPOS_H_
#include "constantes.h"

/*! \file tipos.h
 *  \brief Arquivo que contem os tipo abstratos de dados usados no programa.
 */

/*! \brief TAD que usaremos para representar uma obra de arte. */
typedef struct {
  char titulo[TAM_TITULO + 1];   /**< Titulo da obra de arte. */
  char tipo[TAM_TIPO + 1];       /**< Tipo da obra. */
  char autor[TAM_AUTOR + 1];     /**< Nome do autor. */
  char ano[TAM_ANO + 1];         /**< Ano de publicacao. */
  char valor[TAM_VALOR + 1];     /**< Valor da Obra. */
  char imagem[TAM_IMAGEM + 1];   /**< Imagem correspondente. */
} TObra;

/*! \brief TAD usado para representar uma chave primaria. */
typedef struct {
  char pk[TAM_TITULO + 1]; /**< String usada para armazenar a chave primaria propriamente dita. */
  int nrr;    					/**< Numero relativo do registro na base de dados. */
} Pk;

/*! \brief TAD que representa o indice primario. */
typedef struct {
  Pk * vetor;    /**< Vetor eh um vetor dinamico que ira armazenar os elementos de indice.*/
  int tamanho;   /**< Eh o numero de elementos validos, assim, indica tambem a proxima posicao livre do vetor. */
  int alocado;   /**< Espaco reservado na memoria para o vetor. Aumanta conforme o tamanho do indice se iguala a ele. */
  int tamBase;   /**< Tamanho da base de dados. Usado nas operacoes de insercao quando a avail list eh vazia. */
  int valorHash; /**< Valor da funcao de espalhamento para as strings das chaves primarias contidas no indice. */
} IndPrim;

/*!
 * \brief	Este inteiro sera usado para representar a cabeca de uma avail
 * 			list, que sera um a lista invertida de posicoes livres no disco.
 */
typedef int availList;

/*! \brief TAD que representa uma chave secundaria. */
typedef struct {
  char key[TAM_TITULO+1]; 	/**< String que eh a chave propriamente dita. */
  int next; 					/**< Apontador para a proxima chave no indice. */
  int lenght;					/**< Tamanho da string que contem a chave. */
} Sk;

/*! \brief TAD que representa um indice secundario. */
typedef struct {
  Sk *vetor; 								/**< Vetor dinamico de chaves secundarias. */
  int tamanho;								/**< Tamanho do vetor do indice. */
  int alocado;								/**< Espaco alocado para o vetor. Cresce dinamicamente com o /tamanho/ */
  int tamDisco;							/**< Tamanho do BIGFILE ao qual o indice eh associado. Usado para controlar operacoes de insercao. */
  int valorHash;							/**< Valor da funcao de espalhamento das chaves do indice. */
  char tipoCampo[TAM_NOME_ARQ+1];	/**< Tipo de campo da \a Obra ao qual o indice esta associado. */
} IndSec;


/*! \brief TAD de um descritor de imagem. */
typedef struct {
	char pk[TAM_TITULO+1]; 		/**< Chave primaria do registro ao qual o descritor pertence. */
	char valorDescritor;			/**< Valor do descritor da imagem da \a Obra. */
	double similaridade;			/**< Valor da similaridade da imagem com uma imagem de referencia. Usado na operacao de busca por conteudo.*/
	char imagem[TAM_IMAGEM+1];	/**< Imagem da Obra. */
} Descritor;

/*! \brief TAD de um indice de descritores. */
typedef struct {
	Descritor *vetor;	/**< Vetor dinamico que contem os descritores do indice. */
	int tamanho;		/**< Tamanho do vetor e consequantemente do indice. */
	int alocado;		/**< Espaco alocado para o vetor do indice. Como nos demais indices, cresce dinamicamente com o tamanho do vetor. */
	int valorHash;		/**< Valor da funcao de espalhamento dos descritores do indice. */
} IndDesc;

#endif /*TIPOS_H_*/

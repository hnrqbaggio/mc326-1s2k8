#ifndef TIPOS_H_
#define TIPOS_H_
#include "constantes.h"

/*! \file tipos.h
 *  \brief Arquivo que contem os tipo abstratos de dados usados no programa.
 */

/*! \brief Estrutura usada para representar uma obra de arte. */
typedef struct {
  char titulo[TAM_TITULO + 1];   /**< Titulo da obra de arte. */
  char tipo[TAM_TIPO + 1];       /**< Tipo da obra. */
  char autor[TAM_AUTOR + 1];     /**< Nome do autor. */
  char ano[TAM_ANO + 1];         /**< Ano de publicacao. */
  char valor[TAM_VALOR + 1];     /**< Valor da Obra. */
  char imagem[TAM_IMAGEM + 1];   /**< Imagem correspondente. */
} TObra;

/**
 * @brief Estrutura que ira representar uma folha da Arvore B+.
 */
typedef struct {
	int numChaves; /**< Numero de chaves no arquivo de indice. */
	int id; /**< ID que indica o arquivo de indice ao qual a folha esta associada. */
	char * chave; /** A maior chave do indice (lexicograficamente). */
} BTLeaf;

/** 
 * @brief Estrutura que representa um noh da Arvore B+. 
 */
typedef struct {
	int numChaves; /**< Numero de chaves no noh. Usado para controlar operacoes da arvore. */
	char chaves[B_ORDER-1][TAM_TITULO]; /**< vetor de strings que sao os delimitadores das chaves. */
	int filhos[B_ORDER]; /**< Vetor de ponteiros para os filhos do noh. */
	int level; /**< Nivel do noh - sua altura. A raiz tem level = altura - 1. */
} BTNode;

/*! \brief Estrutura usada para representar uma chave primaria. */
typedef struct {
  char key[TAM_TITULO + 1]; /**< String usada para armazenar a chave primaria propriamente dita. */
  int nrr;    					   /**< Numero relativo do registro na base de dados. */
} indexKey;

/*! \brief Estrutura que representa o indice generico. */
typedef struct index {
  indexKey * vetor;    
  int tamanho;   
  int alocado;   
  int tamFile;
  int id;   
  char tipoCampo[15];
  BTNode *root; 
  struct index * (*carrega) (struct index *, FILE *);
  struct index * (*grava) (struct index *);
} Index;

/*!
 * \brief	Este inteiro sera usado para representar a cabeca de uma avail
 * 			list, que sera um a lista invertida de posicoes livres no disco.
 */
typedef int availList;

/*! \brief Estrutura de um descritor de imagem. */
typedef struct {
  char pk[TAM_TITULO+1]; 		/**< Chave primaria do registro ao qual o descritor pertence. */
  char valorDescritor;			/**< Valor do descritor da imagem da \a Obra. */
  double similaridade;			/**< Valor da similaridade da imagem com uma imagem de referencia. Usado na operacao de busca por conteudo.*/
  char imagem[TAM_IMAGEM+1];	/**< Imagem da Obra. */
} Descritor;

/*! \brief Estrutura de um indice de descritores. */
typedef struct {
  Descritor *vetor;	/**< Vetor dinamico que contem os descritores do indice. */
  int tamanho;		/**< Tamanho do vetor e consequantemente do indice. */
  int alocado;		/**< Espaco alocado para o vetor do indice. Como nos demais indices, cresce dinamicamente com o tamanho do vetor. */
  int valorHash;		/**< Valor da funcao de espalhamento dos descritores do indice. */
} IndDesc;

/** 
 * @brief Estrutura para armazanar temporariamente as informacoes 
 * sobre os resultados de uma busca. 
 * 
 * Ela representa um buffer de resultados da busca, guardando o 
 * nrr de cada registro na base de dados, que obtem atraves
 * de uma pesquisa no indice primario. 
 */
typedef struct {
  int *nrrs; 				/**< Vetor de NRRs dos registros. */
  double *similaridades; 	/**< Vetor com os valores de similaridade com uma obra de referencia */
  int tamanho; 				/**< Tamanho dos vetores. */
  int alocado; 				/**< Espaco alocado pros vetores, aumenta sobre demanda. */
  char chave[TAM_TITULO]; 	/**< Uma string que indica a chave que resultou nestes resultados. */
} resultadosBusca;



#endif /*TIPOS_H_*/

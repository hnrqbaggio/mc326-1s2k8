/** @file descritor.h
 *  @brief Biblioteca que implementa as funcoes
 *         de manipulacao dos indices de descritores de imagens.
*/

#ifndef DESCRITOR_H_
#define DESCRITOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include "hash.h"
#include "libimg.h"
#include "catalogo.h"

/** 
 * @brief Inicializa um indice descritor.
 * 
 * Aloca memoria para a sua propria estrutura
 * e tambem para o vetor de descritores. Tambem inicializa
 * os demais campos do indice com valores apropriados.
 * 
 * return O Indice alocado com valores iniciais.
 */
IndDesc * inicializaDescritor();

/** 
 * @brief Constroi o indice descritor.
 *  
 * Tenta carregar o arquivo zero do indice e se falhar, 
 * constroi a partir da base de dados.
 * 
 * @param indDesc Indice descritor inicializado.
 * @param indPrim Indice primario. E atualizado caso o hash de 
 * alguma chave a ser inserida/carregada seja diferente do seu.
 * @param base O ponteiro para o arquivo da base de dados.
 * return void
*/
void constroiIndDesc(IndDesc *, IndPrim *indPrim, FILE *base);

/** 
 * @brief Troca o indice descritor atual por outro 
 *  no disco de acordo com o valor hash da string.
 * 
 *  @param indice O Indice descritor.
 *  @param valor Descritor cujo hash se deseja calcular.
 *  return O indice secundario atualizado.
*/
IndDesc * trocaIndDesc(IndDesc *ind, char valor);

/**
 * @brief Carrega um indice descritor a partir do arquivo em disco.
 * 
 * @param indice Indice descritor.
 * @param arqIndice Arquivo de indice descritor que fica no disco.
 * @return Indice descritor atualizado.
*/
IndDesc * carregaDescritor(IndDesc *indice, FILE *arqIndice);

/**
 * @brief Insere as informacoes do descritor da imagem no indice descritor.
 *
 * @param indDesc Indice descritor.
 * @param pk Chave primaria do descritor a ser inserido.
 * @param valor Valor do descritor.
 * @param imagem Nome da imagem do descritor.
 * @param simil Similaridade, iniciada como 0.
 * @return O Indice com o novo descritor inserido.
*/
IndDesc * insereDesc(IndDesc *indDesc, char *pk, char valor, char *imagem, double simil);

/** 
 * @brief Funcao usada para realocar espaco para o vetor de descritores do indice caso seja necessario.
 * 
 * Ela ira verificar se o numero de elementos eh
 * sufuciente para o espaco alocado para o vetor e reservar mais caso
 * seja necessario.
 *
 * @param indice O Indice a ser realocado.
 * @return O Indice, com seu vetor realocado ou nao. 
*/
IndDesc * realocaIndDesc(IndDesc *indice);

/** 
 * @brief Funcao usada filtrar as imagens de acordo com a similaridade.
 * 
 * A funcao calcula o descritor da imagem a ser buscada, abre o indice
 * descritor correspondente e filtra as imagens contidas no indice de acordo
 * com a similaridade calculada entre cada imagem e a imagem buscada.
 * 
 * @param indice Indice descritor.
 * @param imgRef Nome da imagem a ser buscada.
 * @return Indice descritor filtrado criado pela funcao que contem as imagens filtradas. 
*/
IndDesc * filtraInd(IndDesc *indice, char *imgRef);

/**
 * @brief Funcao que compara dois descritores pela similaridade.
 * 
 * @param a Um ponteiro para void que sera comparado (com um cast para Descritor*).
 * @param b O outro ponteiro usado na comparacao (tambem com cast).
 * @return Um inteiro que vale 1 se a similaridade a eh maior que o de b,
 *  0 se for igual, ou -1 caso b maior que a.
*/
int compareDesc(const void *a, const void *b);

/**
 * @brief Grava as informacoes do indice descritor no disco.
 * 
 * @param indice Indice descritor.
 * @return void.
*/
void gravaIndDesc(IndDesc *indice);

/**
 * @brief Ordena o indice descritor pela similaridade.
 * 
 * @param indice Indice descritor.
 * @return void.
*/
void ordenaIndDesc(IndDesc *indice);

#endif /*DESCRITOR_H_*/

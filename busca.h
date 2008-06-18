/** @file busca.h
 * @brief Biblioteca implementa as funcionalidades 
 * de busca na base de dados.
 */

#ifndef _BUSCA_H
#define _BUSCA_H

#include <stdio.h>
#include "catalogo.h"
#include "sk.h"
#include "descritor.h"
#include "menu.h"
#include "leitura.h"


/**
 * @brief Chama a funcao de busca para um determinado o indice primario, 
 * exibindo mensdagens para o usuario. Eh um pequeno adapter para evitar 
 * repeticao de codigo no main.
 * 
 * @param primario O indice primario atual.
 * @param base A base de dados.
 */
void buscaPrimario(Index * primario, FILE * base);

/**
 * @brief Analoga a buscaPrimario, chamando a funcao de busca
 * para um indice secundario, e exibindo mensagens para o usuario..
 * 
 * @param primario O indice primario atual.
 * @param secundario O indice secundario onde sera feita a busca,
 * @param base A base de dados.
 */
void buscaSecudario(Index * primario, Index * secundario, FILE * base);

/**
 * @brief Analoga a buscaPrimario e buscaSecundario, chamando a funcao de busca
 * para um indice de descritores, e exibindo mensagens para o usuario..
 * 
 * @param descritores O indice de descritores onde sera feita a busca,
 * @param primario O indice primario atual.
 * @param base A base de dados.
 */
void buscaDescritor(IndDesc * descritores, Index * primario, FILE * base);


/**
 * @brief Realiza uma busca por chave primaria na base de dados, armazenando 
 * numa estrutura de resposta o NRR do registo.
 * 
 * @param chave A chave de busca.
 * @param prim O indice primario (pose ser atualizado durante a operacao, devido ao hash).
 * @param base O ponteiro para a base de dados.
 * @param busca O ponteiro para a estrutura de resposta, que pode ser atulizada caso 
 * esta funcao esteja sendo usada pelas demais funcoes de busca.
 */
resultadosBusca * buscaPk(indexKey * chave, Index * prim, FILE * base, resultadosBusca * busca);

/**
 * @brief Realiza uma busca por palavra inteira na base de dados, encontrando 
 * a chave no indice secundario e passando a chave primarias associadas para a 
 * funcao que realiza busca no inidice primario.
 * 
 * @param chave A chave de busca.
 * @param prim O indice primario (pose ser atualizado durante a operacao, devido ao hash).
 * @param sec O indice secundario (tambem pode ser atualizado).
 * @param base O ponteiro para a base de dados.
 */
resultadosBusca * buscaSk(char *chave, Index *sec, Index *prim, FILE *base);


/**
 * @brief Realiza uma busca por similaridade entre as imagens na base de dados 
 * e uma imagem de referencia, passando a chave primarias associadas para a 
 * funcao que realiza busca no inidice primario.
 * 
 * @param arqImagem O nome da imagem de referencia.
 * @param descr O inidice de descritores (pose ser atualizado durante a operacao, devido ao hash).
 * @param prim O indice primario (tambem pode ser atualizado).
 * @param base O ponteiro para a base de dados.
 */
resultadosBusca * buscaPorConteudo(char * arqImagem, IndDesc * descr, Index * prim, FILE * base);

/** 
 * @brief Libera a memoria utilizada para armazenar os resultados de uma busca. 
 * @param result O ponteiro para a estrutura de busca que sera liberada. */
void liberaBusca(resultadosBusca * result);

#endif

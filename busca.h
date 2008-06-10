/** @file busca.h
 * @brief Esta biblioteca implementa as funcionalidades 
 * de busca na base de dados.
 */

#ifndef _BUSCA_H
#define _BUSCA_H

#include <stdio.h>
#include "catalogo.h"
#include "sk.h"
#include "descritor.h"
#include "menu.h"

/**
 * @brief Chama a funcao de busca para um determinado indice secundario, 
 * exibindo mensdagens para o usuario. Eh um pequeno adapter para evitar 
 * repeticao de codocgo no main.
 * 
 * @param primario O indice primario atual.
 * @param secundario O indice secundario onde sera feita a busca,
 * @base A base de dados.
 */
void buscaSecudario(IndPrim *primario, IndSec *secundario, FILE* base);

/**
 * @brief Realiza uma busca por palavra inteira na base de dados, produzindo
 * uma saida com uma lista de todas as obras que possuem a chave secundaria buscada.
 * 
 * @param chave A chave de busca.
 * @param prim O indice primario (pose ser atualizado durante a operacao, devido ao hash).
 * @param sec O indice secundario (tambem pode ser atualizado).
 * @param base O ponteiro para a base de dados.
 */
void buscaSk(char *chave, IndPrim *prim, IndSec *sec, FILE *base);


/**
 * @brief Realiza uma busca por conteudo nas imagens da base de dados, usando
 * a similaridade entre as mesmas e uma imagem de referencia.
 * 
 * @param arqImagem O nome da imagem de referencia.
 * @param descr O inidice de descritores (pose ser atualizado durante a operacao, devido ao hash).
 * @param prim O indice primario (tambem pode ser atualizado).
 * @param base O ponteiro para a base de dados.
 */
void buscaPorConteudo(char *arqImagem, IndDesc *descr, IndPrim *prim, FILE *base);

#endif

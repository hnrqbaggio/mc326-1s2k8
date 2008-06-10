/** @file busca.h
 * @brief Esta biblioteca implementa as funcionalidades 
 * de busca na base de dados utilizando indices secundarios.
 */
#ifndef _BUSCA_H
#define _BUSCA_H

#include <stdio.h>
#include "catalogo.h"
#include "sk.h"

/**
 * @brief Realiza uma busca por palavra inteira na base de dados, produzindo
 * uma saida com uma lista de todas as obras que possuem a chave secundaria buscada.
 * 
 * @param chave A chave de busca.
 * @param prim O indice primario (pose ser atualizado durante a operacao, devido ao hash).
 * @param sec O indice secundario (tambem pode ser atualizado).
 * @param base O ponteiro para a base de dados.
 * @param tipoCampo O tipo do campo a ser buscado.
 */
int buscaSk(char *chave, TIndice *indPrim, IndSec *indSecun, FILE *base, const int tipoCampo);

#endif

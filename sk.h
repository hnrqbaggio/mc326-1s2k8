/** \file sk.h
 *  \brief Biblioteca que implementa as funcoes
 *         de manipulacao dos indices secundarios.
 */

#ifndef _SK_H
#define _SK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tipos.h"
#include "hash.h"

/** @brief Constroi todos os indices secundarios de uma vez.*/
/** 
 * Essa operacao eh feita de uma soh vez para economizar acessos 
 * ao disco, percorrendo a base de dados no maximo uma unica vez.
 * 
 * Tenta carregar o arquivo zero de cada indice e se algum falhar, 
 * constroi todos a partir da base de dados.
 * 
 * @param indPrimario Eh o indice primario. 
 * Eh atualizado caso o hash de alguma chave a ser inserida/carregada 
 * seja diferente do seu.
 * @param base O ponteiro para o arquivo da base de dados.
 * @param indTitulo indice secundario de titulos.
 * @param indTipo indice secundario de tipos.
 * @param indAutor indice secundario de autores.
 * @param indAno indice secundario de anos.
 * @param avTitulo Avail List do indice de titulos.
 * @param avTipo Avail List do indice de tipos.
 * @param avAutor Avail List do indice de autores.
 * @param avAno Avail List do indice de ano.
 */
void constroiSecundarios(Index *indPrimario, FILE *base, Index *indTitulo, Index *indTipo, Index *indAutor, Index *indAno, availList *avTitulo, availList *avTipo, availList *avAutor, availList *avAno);

/**
 * @brief Insere as SKs de uma string relativa a um campo da obra
 * num indice secundario. Atualiza a lista invertida no BigFile.
 *
 * @param indice O indice secundario.
 * @param bigFile Um ponteiro para o BigFile do indice.
 * @param pk A chave primaria do registro.
 * @param campo A string do campo que ira gerar as chaves.
 * @param avail Um ponteiro para a Avail List do indice.
 *
 * @return O Indice com a nova chave inserida.
 */
Index * insereSk(Index *indice, FILE *bigFile, char *pk, char *campo, availList *avail);

/**
 * @brief Carrega um indice secundario a partir do arquivo em disco, especificado
 * pelo parametro passado.
 * 
 * @param indSk O ponteiro para o indice secundario vazio.
 * @param id O id do arquivo a ser carregado para a memoria.
 */
void carregaIndiceSecun(Index *indSk, int id);

/**
 * @brief Grava as informacoes do indice secundario no disco.
 * @param indice O indice secundario.
 */
void gravaIndiceSecun(Index *indice);


#endif

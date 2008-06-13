/** @file remove.h
 * @brief Biblioteca que trata a remocao de registros da base de dados e das implicacoes dessa remocao nos indices.
*/

#ifndef _REMOVE_H
#define _REMOVE_H

#include <stdio.h>
#include <string.h>
#include "catalogo.h"
#include "sk.h"

/** @brief Funcao que remove um registro do indice secundario e da base de dados usando a avail list da base.
 * 
 * @param chave String com a PK do registro a ser removido.
 * @param indPrim Indice primario.
 * @param base Base de dados.
 * @param avail Avail list da base.
 * @return Indice primario atualizado.
*/
IndPrim * removePk(char *chave, IndPrim *indPrim, FILE *base, availList *avail);

/** @brief Funcao q remove uma entrada de um indice secundario.
 * 
 * @param chave String com a SK.
 * @param indSecun Indice secundario.
 * @param pk Indice primario da obra removida.
 * @param avail Avail list do indice secundario.
 * @return Indice secundario atualizado.
*/
IndSec * removeSk(char *chave, IndSec *indSecun, char *pk, availList *avail);

IndDesc * removeDesc(char *pk, IndDesc *descritores);

#endif

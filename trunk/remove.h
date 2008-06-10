#ifndef _REMOVE_H
#define _REMOVE_H

#include <stdio.h>
#include <string.h>
#include "catalogo.h"
#include "sk.h"

/**
 * Biblioteca q trata a remocao de registros da base de dados e das implicacoes dessa remocao nos indices.
 */

/* Funcao que remove um registro do indice secundario e da base de dados usando a avail list da base.
 * Parametros: uma string com a PK do registro, um ponteiro pro indice primario, outro pra base de dados, e um pra avail list. */
IndPrim * removePk(char *, IndPrim *, FILE *, availList *);

/* Funcao q remove uma entrada de um indice secundario.
* Parametros: uma string com a SK, um ponteiro pro indice secundario, a string da pk e a avail list. */
IndSec * removeSk(char *, IndSec *, char *, availList *);

#endif

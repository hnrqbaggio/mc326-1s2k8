#ifndef _BUSCA_H
#define _BUSCA_H

#include <stdio.h>
#include "catalogo.h"
#include "sk.h"

/**
 * Esta biblioteca implementa as funcionalidades de busca na base de
 * dados.
 */

/**
 * Funcao que busca palavra passada por parametro nos indices secundarios.
 * Ela busca as PKs correspondentes a SK da palavra nos indices secundarios,
 * busca os nrr das PKs encontradas no indice primario e gera um arquivo html
 * com os dados das obras encontradas.
 * Recebe como parametros a palavra a ser buscada, o indice primario,
 * o indice secundario onde sera realizada a busca, a base de dados e um inteiro
 * que define o tipo do campo.
*/
void buscaSk(char *, TIndice *, IndSec *, FILE *, const int);

#endif

/** @file avail.h
 * @brief Bliblioteca que implementa as funcoes de avail list. 
 * 
 * funcoes que facilitam na remocao e insercao de obras aproveitando o espaço
 * previamente alocado.
 */

#ifndef _AVAIL_H
#define _AVAIL_H

#include <stdio.h>
#include "sk.h"

/** @brief Funcao que le o conteudo de um arquivo de avail list. 
 * 
 * @param campo O tipo de campo da obra ao qual a avail list se
 * se refere.
 * return Um inteiro que eh o valor do noh cabeca da avail list 
 */

int openAvail (const char *campo);

/** @brief Grava o valor de uma avail list no disco. 
 * @param avail O no cabeca da avail list.
 * @param campo O tipo de campo da obra ao qual a avail list se
 * se refere.
 */

void gravaAvail(availList avail, const char *campo );

#endif

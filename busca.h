#ifndef _BUSCA_H
#define _BUSCA_H

#include <stdio.h>
#include "catalogo.h"
#include "sk.h"

/**
 * Esta biblioteca implementa as funcionalidades de busca na base de
 * dados.
 */

TIndice * buscaMultipla(IndSec *, char *);
TIndice * intersection(TIndice **);

#endif

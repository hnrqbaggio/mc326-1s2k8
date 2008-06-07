#ifndef DESCRITOR_H_
#define DESCRITOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include "hash.h"
#include "libimg.h"
#include "catalogo.h"



IndDesc * inicializaDescritor();

void constroiIndDesc(IndDesc *, IndPrim *, FILE *);

IndDesc * trocaIndDesc(IndDesc *, char);

IndDesc * carregaDescritor(IndDesc *, FILE *);

IndDesc * insereDesc(IndDesc *, char *, char, char *, double);

IndDesc * realocaIndDesc(IndDesc *);

IndDesc * filtraInd(IndDesc *, char*);

int compareDesc(const void *, const void *);

void gravaIndDesc(IndDesc *);

void ordenaIndDesc(IndDesc *);

int compareDesc(const void *, const void *);

#endif /*DESCRITOR_H_*/

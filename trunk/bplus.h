#ifndef BPLUS_H_
#define BPLUS_H_
#include <stdio.h>

typedef struct {
	int numChaves; /**< Numero de chaves no noh. Usado para controlar operacoes da arvore. */
 	int chaves[B_ORDER-1] /**< vetor de strings que sao os delimitadores das chaves. */
	int filhos[B_ORDER]; /**< Vetor de ponteiros para os filhos do noh. */
	int level; /**< Nivel do noh - sua altura. A raiz tem level = altura - 1. */
	int left;
	int right;
} BTNode;

/* Funcoes */
/* Insercao. retorno: 1:ok, 0:repetido, -1:overflow do node */
int insert(int chave, int nodeId);

int bsearch();
int remove();

/* Quebra um noh, retorna o ID do novo. */
int split(BTNode *cheio, BTNode *novo);

int merge();

/* Aloca memoria. */
BTNode makeNode(void);

/* I/O */
void readNode(BTNode *node, int nodeId);
void writeNode(BTNode *node, int nodeId);
void leRegistro(); /* Pegar em catalogo.c */

int getId();

#endif /*BPLUS_H_*/

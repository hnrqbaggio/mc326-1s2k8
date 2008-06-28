#ifndef BTREE_H_
#define BTREE_H_

#include "bplus.h"


typedef struct {
	int root;
} BTree;

void  makeRoot();
BTree * makeTree();
void loadIndex(BTree *, FILE *);
void BTreeInsert(BTree * tree, int key, int nrr);

#endif /*BTREE_H_*/

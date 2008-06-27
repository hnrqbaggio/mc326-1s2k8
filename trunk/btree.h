#ifndef BTREE_H_
#define BTREE_H_

#include "bplus.h"


typedef struct {
	BTNode *root;
} BTree;

BTree * makeTree();
void loadIndex(BTree *, FILE *);
void BTreeInsert(BTree * tree, int key, int nrr);

#endif /*BTREE_H_*/

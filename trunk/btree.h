#ifndef BTREE_H_
#define BTREE_H_

#include "bplus.h"


typedef struct {
  int root;
  int tamBase;
} BTree;

void  makeRoot();
BTree * makeTree();
void loadIndex(BTree *, FILE *);
int BTreeInsert(BTree * tree, int key);
int BTreeRemove(BTree * tree, int key);

#endif /*BTREE_H_*/

#include "btree.h"

BTree * makeTree() {
  int i;
  BTree * temp = malloc(sizeof(BTree));
	
  temp->root = makeNode();
  temp->root->id = 0;
  temp->root->numChaves = 0; 
  temp->root->leaf = TRUE;
  temp->root->left = -1;
  temp->root->right = -1;

  for (i = 0; i < B_ORDER-1; i++) {
    temp->root->chaves[i] = 0;
    temp->root->filhos[i] = 0;
  }
  temp->root->filhos[i] = 0; /* O ponteiro extra. */
	
  return temp;
}

void BTreeInsert(BTree * tree, int key, int nrr) {
  int result;
  pk temp;
	
  temp.key = key;
  temp.pointer = nrr;
	
  result = insert(&temp, tree->root->id);
	
  if (result == OVERFLOW) {
		
#ifdef DEBUG 
    fprintf(stderr, "Root OverFlow\n");
#endif
		 
    rootOverflow(tree->root);
  }
	
}

void loadIndex(BTree *tree, FILE *base) {
  int temp;
  int key, nrr = 0;
	
  temp = readNode(tree->root, tree->root->id);
	
  if (!temp) { /* nao existe arquivo zero. */

    writeNode(tree->root); /* Escreve o root atual no seu arquivo. */
		
    while(!feof(base)) {
      fscanf(base, "%04d", &key);
      BTreeInsert(tree, key, nrr);
      nrr++;
    }
  }
}

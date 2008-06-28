#include "btree.h"

void makeRoot() {
  int i;
  BTNode *root;
  
  root = makeNode();
  root->id = 0;
  root->numChaves = 0; 
  root->leaf = TRUE;
  root->left = -1;
  root->right = -1;

  for (i = 0; i < B_ORDER; i++) {
    root->chaves[i] = -1;
    root->filhos[i] = -1;
  }
  root->filhos[i] = -1; /* O ponteiro extra. */
	
  writeNode(root);
  free(root);

}

BTree * makeTree() {
  BTree *tree = (BTree *)malloc(sizeof(BTree));
  tree->root = 0;

  return tree;  
}

void BTreeInsert(BTree * tree, int key, int nrr) {
  int result;
  pk temp;
	
  temp.key = key;
  temp.pointer = nrr;
	
  result = insert(&temp, tree->root);
	
  if (result == OVERFLOW) {
		
#ifdef DEBUG 
    fprintf(stderr, "Root OverFlow\n");
#endif
		 
    rootOverflow();
  }
	
}

void loadIndex(BTree *tree, FILE *base) {
  int temp;
  int key, nrr = 0;

  BTNode *root = makeNode();
	
  temp = readNode(root, tree->root);
	
  if (!temp) { /* nao existe arquivo zero. */

    makeRoot();
		
    while(!feof(base)) {
      fscanf(base, "%04d", &key);
      BTreeInsert(tree, key, nrr);
      nrr++;
    }
  }
}

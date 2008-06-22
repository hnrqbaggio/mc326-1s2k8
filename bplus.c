#include "bplus.h"

int insert(pk *key, int nodeId) {
	 
  int i = 0, j, idFilho, result, middle, tamanho;
  pk *middle;
  BTNode *node;

  node = makeNode();
  carrega(node, nodeId);
	 
  while (key > node->chaves[i] && i < node->numChaves) i++;
	 
  if (node->leaf == TRUE) { /* O Node eh uma folha. */

    if (node->chave[i] == pk->key) return FALSE; /* Repeticao. */	
    
    /* Abre espaco para a nova chave no vetor de chaves do noh. */
    j = node->numChaves;
    node->chaves[j] = node->chaves[--j];
    for(; j >= i; j--) {
      node->filhos[j] = node->filhos[j-1];
      node->chaves[j] = node->chaves[j-1];
    }
    node->chaves[i] = pk->key;
    node->filhos[i] = pk->pointer;

    (node->numChaves)++;
    	 	
  } else { /* O node eh pai de outro node. */
    
    idFilho = node->filhos[i];
    writeNode(node);
    free(node);

    result = insert(key, idFilho);
	 	
    if (result == OVERFLOW) { /* Overflow do filho. */

      node = makeNode();
      carrega(node, nodeId);

      r = rotation(idFilho, node->filhos[i-1], LEFT); /* Rotacao a esquerda. */

      if (r) return TRUE;
      else r = rotation(idFilho, node->filhos[i+1], RIGHT);

      if (r) reurn TRUE;

      else {/* Nenhum tem espaco. */
	middle = split(idFilho);

	j = node->numChaves;
	node->chaves[j] = node->chaves[--j];
	for(; j >= i; j--) {
	  node->filhos[j] = node->filhos[j-1];
	  node->chaves[j] = node->chaves[j-1];
	}

	node->chaves[i] = middle->key;
	node->filhos[i+1] = middle->pointer;
	
	(node->numChaves)++;

	free(middle);
      }
    }
  }
  tamanho = node->numCahves;
	 
  if (tamanho == B_ORDER) return OVERFLOW;
  else return result;	
}


/* Aloca memoria para um novo node. */
BTNode * makeNode(int level) {
  BTNode * resp = (BTNode *) malloc (sizeof(BTNode));
	
  resp->numChaves = 0;
  resp->level = level;
	
  return resp;
}

int createNewNode(BTNode **novo, int nivel, char * campo) {
  int id = getId(campo);
  *novo = makeNode(nivel);

  return id;
}

void loadNode(BTNode *node, int id, char *campo) {
  /* Leitura da pagina */
  FILE *pageFile;
  char pageFileName[TAM_NOME_ARQ];
	
  sprintf(pageFileName, "%s%d%s", campo, id, EXTENSAO_NODE);
  pageFile = fopen(pageFileName, "r");
	
  if (pageFile) fread(node, sizeof(BTNode), 1, pageFile); /* Por enquanto le como binario... */

}

void writeNode(BTNode *filho, int nodeId, char *campo) {
  FILE *pageFile;
  char pageFileName[TAM_NOME_ARQ];
	
  sprintf(pageFileName, "%s%d%s", campo, nodeId, EXTENSAO_NODE);
  pageFile = fopen(pageFileName, "w");
	
  fwrite(filho, sizeof(BTNode), 1, pageFile);
}

/* __________ Operacoes sobre folhas ___________ */

BTLeaf * makeLeaf(int id) {
  BTLeaf * folha = (BTLeaf *) malloc (sizeof(BTLeaf));
	
  folha->id = id;
  folha->numChaves = 0;
	
  return folha;
}

int createNewLeaf(BTLeaf *folha, char *campo) {
  int id = getId(campo);
	
  folha = makeLeaf(id);

  return id;
}

BTLeaf * loadLeaf(int id, char *campo) {
  BTLeaf * folha = makeLeaf(id);
  FILE * arquivo; /* Arquivo que contem a folha*/
  char nomeArq[TAM_NOME_ARQ];
	
  sprintf(nomeArq, "%s%d%s", campo, id, EXTENSAO_FOLHA);
  arquivo = fopen(nomeArq, "r");
	
  /* Le os dados da folha. */
  if (arquivo) fread(folha, sizeof(BTLeaf), 1, arquivo);
	
  return folha;
}

void writeLeaf(BTLeaf * folha, char * campo) {
  FILE * arquivo; /* Arquivo que contem a folha */
  char nomeArq[TAM_NOME_ARQ];
	
  sprintf(nomeArq, "%s%d%s", campo, folha->id, EXTENSAO_FOLHA);
  arquivo = fopen(nomeArq, "w");
	
  fwrite(folha, sizeof(BTLeaf), 1, arquivo);
	
  return;
}

int insertInLeaf(char *key, BTLeaf *leaf) {
	
  if (strcmp(leaf->chave, key) < 0) { /* Key eh maior q a maior chave no indice. */
    strcpy(leaf->chave, key);
  }
	
  return 1; /* por enquanto. */
}

int getId(char *campo) {
  int id = 0; /* O ID zero eh reservado para a primeira folha da arvore. */
  FILE *idFile;
  char arq[TAM_NOME_ARQ];
	
  sprintf(arq, "%s%s", campo, EXTENSAO_ID);
  idFile = fopen(arq, "r");
	
  if (idFile) { /* Se o arquivo jah existe, le e fecha. */
    fread(&id, sizeof(int), 1, idFile);
    fclose(idFile);
  }
	
  id++;
  idFile = fopen(arq, "w");
  fwrite(&id, sizeof(int), 1, idFile); /* Atualiza o arquivo. */
	
  return id;
}

void rootOverFlow(Index *indice) {
  int id1, id2;
  BTNode *newRoot, *novoIrmao;
	
  id1 = createNewNode(&newRoot, indice->root->level + 1, indice->tipoCampo); 
  id2 = createNewNode(&novoIrmao, indice->root->level, indice->tipoCampo);
	
  split(indice->root, novoIrmao);
   
  /* A chave media do root antigo vira a chave do root novo. */
  strcpy(newRoot->chaves[0], novoIrmao->chaves[novoIrmao->numChaves]);
  newRoot->filhos[0] = id2; 
  newRoot->filhos[1] = indice->rootId;
  (newRoot->numChaves)++;
	      	
  writeNode(indice->root, indice->rootId, indice->tipoCampo);
  writeNode(novoIrmao, id2, indice->tipoCampo);
      	
  indice->root = newRoot;
  /** @bug LEAK! */
}

int search(char *key, BTNode *node, Index *indice) {
  /* Busca na BTree
   * 
   * caso particular root->numChaves == 0
   * 	return -1;
   * 
   * acho o indice do filho que contem a chave nao menor que key
   * se folha retorno filho[i]
   * senao carrego o filhe e chamo a recursao pro filho.
   * 
   */
	 
  int i = 0, id;
  BTNode * filho;
	 
  if (node->numChaves == 0) return 0; /* Noh vazio, certamente eh um ROOT no inicio do programa. */
	 
  while (strcmp(node->chaves[i], key) < 0 && i < node->numChaves) i++;
	 
  if (node->level == 1) { /* Pai de folha. */
	 	
    id = node->filhos[i];
	 	
  } else {
	 	
    filho = makeNode(node->level - 1);
    loadNode(filho, node->filhos[i], indice->tipoCampo); 	
	 	
    /* Recursividade. */
    id = search(key, filho, indice);
  }
	 
  return id;
}

void split(BTNode *node1, BTNode *node2) {

  int meio = node1->numChaves / 2 +1, j;
  BTNode * swap;
	
  /* Copia metade dos dados de um indice para outro. */
  for(j = meio; j < node1->numChaves; j++) {
    strcpy(node2->chaves[j], node1->chaves[meio-j]);
    node2->filhos[j+1] = node1->filhos[meio-j-1];
  }
	 		
  node1->numChaves = meio-1;
  node1->numChaves = j;
	 		
  /* Faz uma troca de indices, para manter a ordenacao e evitar muita copia de dados. */
  swap = node2; node2 = node1; node1 = swap;
}

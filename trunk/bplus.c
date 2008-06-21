#include "bplus.h"

int insert(char *key, BTNode *node, Index *indice) {

  /* Busca a chave no vetor de separadores. */
  /* while chave > sep[i], i++
   * 
   * se nevel == 1;
   *  carrega folha;
   * 	temp = insertInLeaf(key);
   * 
   * 	if (overflow(indice))
   * 		criar nova folha;
   * 		indice = indice->carrega(indice);
   * 		cria novo indice;
   * 		copia metade do indice atual pro novo;
   * 		salva ambos os indices;
   * 		abre espaco;
   * 		insere key em seps[i];
   * 		insere id da nova folha em filhos[i];
   * 
   * senao
   * 	aloca novo noh filho = carrega o filho[i];
   *  temp = insert(key, filho, indice);
   * 
   * 	se overflow(filho)
   * 		cria novo filho;
   * 		copio metade dos seps e ids pro novo;
   * 		abre espaco
   * 		insere key em seps[i];
   * 		insere id do novo filho em seps[i];
   * 
   * if overflow return -1
   * else return temp
   * 
   */
	 
  int i = 0, j, temp, meio, id;
  BTNode *filho, *novoFilho;
  Index * novoIndice, * swap;
	 
  while (strcmp(key, node->chaves[i]) > 0 && i < node->numChaves) i++;
	 
  if (node->level == 1) { /* O Node eh pai de uma folha. */
	 	
    if(node->chaves[0][0] == '\0') {
      strcpy(node->chaves[0], key); /* Caso particular */
      (node->numChaves)++;
    }
	 	
    if(i >= node->numChaves) strcpy(node->chaves[i-1], key); 
	 	
    if (indice->tamanho == BLOCK_SIZE) { /* Overflow do indice. */
	 		
      novoIndice = makeIndex(indice->tipoCampo); 
      meio = indice->tamanho / 2;
	 		
      /* Copia metade dos dados de um indice para outro. */
      for(j = meio; j < indice->tamanho; j++) {
	strcpy(novoIndice->vetor[j].key, indice->vetor[meio-j].key);
	novoIndice->vetor[j].nrr =  indice->vetor[meio-j].nrr;
      }
	 		
      /* Atualiza tamanhos e ID. */
      novoIndice->tamanho = meio;
      indice->tamanho -= meio;
      novoIndice->tamFile = indice->tamFile;
      novoIndice->id = getId(indice->tipoCampo);
	 		
      /* Faz uma troca de indices, para manter a ordenacao e evitar muita copia de dados. */
      swap = novoIndice; novoIndice = indice; indice = swap;
	 		
      /* Grava os indices nos arquivos. */
      indice->grava(indice);
      novoIndice->grava(novoIndice);
	 		
      for(j = node->numChaves; j > i; j--) {
	node->filhos[j] = node->filhos[j-1];
	strcpy(node->chaves[j-1], node->chaves[j-2]);
      }
      node->filhos[j] = node->filhos[j-1];
	 		
      strcpy(node->chaves[i], key);
      node->filhos[i] = novoIndice->id;

    }
	 	
  } else { /* O node eh pai de outro node. */
	 	
    /* Aloca espaco para o node. */
    filho = makeNode(node->level - 1);
	 	
    /* Carrega os dados da pagina para o node. */
    loadNode(filho, node->filhos[i], indice->tipoCampo);
	 	
    /* Chama a insercao recursivamente. */
    temp = insert(key, filho, indice);
	 	
    if (temp == -1) { /* Overflow do filho. */
	 	
      /* Aloca memoria para um novo node e retorna o ID para ele. */
      id = createNewNode(&novoFilho, node->level - 1, indice->tipoCampo);
	 		
      /* Divide as chaves entre os nos. */
      split(filho, novoFilho);
	 		
      /* Abre espaco para a nova chave no vetor de chaves do noh. */
      for(j = node->numChaves; j > i; j--) {
	node->filhos[j] = node->filhos[j-1];
	strcpy(node->chaves[j-1], node->chaves[j-2]);
      }
      node->filhos[j] = node->filhos[j-1];
	 		
      /* A chave media do filho quebrado eh inserida neste noh. */
      strcpy(node->chaves[i], novoFilho->chaves[novoFilho->numChaves]);
      node->filhos[i] = id;
	 		
      /* Salva ambos os filhos nas suas paginas. */
      writeNode(filho, node->filhos[i+1], indice->tipoCampo);
      writeNode(novoFilho, id, indice->tipoCampo);
	 		
      /* libera a memoria dos filhos. */
      free(filho);
      free(novoFilho);
    }
	 	
  }
	 
  if (node->numChaves == B_ORDER) return -1; /* Overflow */
  else return temp; /* Insercao bem sucedida eh 1, caso contrario eh zero. */
	
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

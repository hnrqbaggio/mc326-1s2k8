#include "bplus.h"

int insert(pk *key, int nodeId) {
	 
  int i = 0, j, idFilho, result, tamanho;
  pk middle;
  BTNode *node;

  node = makeNode();
  readNode(node, nodeId);
	 
  while (key->key > node->chaves[i] && i < node->numChaves) i++;
	 
  if (node->leaf == TRUE) { /* O Node eh uma folha. */

    if (node->chaves[i] == key->key) return FALSE; /* Repeticao. */	
    
    /* Abre espaco para a nova chave no vetor de chaves do noh. */
    j = node->numChaves;
    node->chaves[j] = node->chaves[--j];
    for(; j >= i; j--) {
      node->filhos[j] = node->filhos[j-1];
      node->chaves[j] = node->chaves[j-1];
    }
    node->chaves[i] = key->key;
    node->filhos[i] = key->pointer;

    (node->numChaves)++;
    	 	
  } else { /* O node eh pai de outro node. */
    
    idFilho = node->filhos[i];
    writeNode(node);
    free(node);

    result = insert(key, idFilho);
	 	
    if (result == OVERFLOW) { /* Overflow do filho. */

      node = makeNode();
      readNode(node, nodeId);

      result = rotation(idFilho, node->filhos[i-1], LEFT); /* Rotacao a esquerda. */

      if (result) return TRUE;
      else result = rotation(idFilho, node->filhos[i+1], RIGHT);

      if (result) return TRUE;

      else {/* Nenhum tem espaco. */
      	middle = split(idFilho);
      
      	j = node->numChaves;
      	node->chaves[j] = node->chaves[--j];
      	for(; j >= i; j--) {
      	  node->filhos[j] = node->filhos[j-1];
      	  node->chaves[j] = node->chaves[j-1];
        }
      
      	node->chaves[i] = middle.key;
      	node->filhos[i+1] = middle.pointer;
      	
      	(node->numChaves)++;
      }
    }
  }
  tamanho = node->numChaves;
	 
  if (tamanho == B_ORDER) return OVERFLOW;
  else return result;	
}

/* Aloca memoria para um novo node. */
BTNode * makeNode() {
  BTNode * resp = (BTNode *) malloc (sizeof(BTNode));
	
  resp->numChaves = 0;
	
  return resp;
}

/**
 * @brief Carrega um arquivo da arvore para memoria (BTNode).
 * 
 * @param 
 * @param 
*/
void readNode(BTNode *node, int nodeId) {
  
  FILE *file;
  char nomeArq[TAM_NOME_ARQ];
  
  sprintf(nomeArq, "%d%s", nodeId, EXTENSAO_NODE);
  file = fopen(nomeArq, "r");
  
  if (file) { /*Se o arquivo existe*/
    
    /*Leitura dos campos do arquivo*/
     
    
  }
}

/*Grava os dados do no em memoria para o arquivo*/
void writeNode(BTNode *node) {
  
  FILE *file;
  char nomeArq[TAM_NOME_ARQ];
  
  sprintf(nomeArq, "%d%s", node->id, EXTENSAO_NODE);
  file = fopen(nomeArq, "w");
  
  /*Grvacao dos dados*/
  
}

/*Pega o id do arquivo da arvore a ser gravado. */
int getId() {
  int id = 0; /* O ID zero eh reservado para a primeira folha da arvore. */
  FILE *idFile;
  
  idFile = fopen(ARQUIVO_ID, "r");
  
  if (idFile) { /* Se o arquivo jah existe, le e fecha. */
    fread(&id, sizeof(int), 1, idFile);
    fclose(idFile);
  }
  
  id++;
  idFile = fopen(ARQUIVO_ID, "w");
  fwrite(&id, sizeof(int), 1, idFile); /* Atualiza o arquivo. */
  fclose(idFile);
  
  return id;
}

/*Faz split do filho que teve overflow*/
pk split(int nodeId) {

  pk retorno;
  int meio, j;
  BTNode *node = makeNode();
  BTNode *new = makeNode();
  
  /*Carrego o filho que tem overflow*/
  readNode(node, nodeId);
  
  meio = node->numChaves / 2 +1;
  
  /* Copia metade dos dados de um indice para outro. */
  for(j = meio; j < node->numChaves; j++) {
    new->chaves[j-meio] = node->chaves[j];
    new->filhos[j-meio+1] = node->filhos[j-1];
  }
      
  node->numChaves = meio-1;
  new->numChaves = j;
  
  /*Atualizo os apontadores e retorno pk, que contem a chave a ser inserida no pai.*/
  retorno.key = new->chaves[0];
  retorno.pointer = new->id;
  
  /*Gravo os novos arquivos na base, libero memoria.*/
  writeNode(new);
  writeNode(node);
  free(node);
  free(new);
  
  return retorno;
}

/*Rotacao de acordo com o parametro LEFT/RIGHT */
int rotation(int idFilho, int idIrmao, const int tipo) {
  
  return 0;
}


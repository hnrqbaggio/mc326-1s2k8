#include "bplus.h"

int insert(pk *key, int nodeId) {
  int i = 0, j, idFilho, result = FALSE, tamanho;
  pk middle;
  BTNode *node;
  
#ifdef DEBUG
  fprintf(stderr, "Inserção em Node %d\n", nodeId);
#endif

  node = makeNode();
  readNode(node, nodeId);

  while (key->key > node->chaves[i] && i < node->numChaves) i++;
	 
  if (node->leaf == TRUE) { /* O Node eh uma folha. */

    if (node->chaves[i] == key->key) return result; /* Repeticao. */	
    
    /* Abre espaco para a nova chave no vetor de chaves do noh. */
    j = node->numChaves;
    node->filhos[j+1] = node->filhos[j];
    for(; j >= i; j--) {
      node->filhos[j] = node->filhos[j-1];
      node->chaves[j] = node->chaves[j-1];
    }
    node->chaves[i] = key->key;
    node->filhos[i] = key->pointer;

    tamanho = ++(node->numChaves);

    writeNode(node);
    result = TRUE;
    	 	
  } else { /* O node eh pai de outro node. */
    
    idFilho = node->filhos[i];
    writeNode(node);
    free(node);

    result = insert(key, idFilho);
	 	
    if (result == OVERFLOW) { /* Overflow do filho. */

      node = makeNode();
      readNode(node, nodeId);

      if (i > 0 && (result = rotation(idFilho, node->filhos[i-1], LEFT)) && result != -1) { 

	node->chaves[i-1] = result;

      } else if (i < node->numChaves && (result = rotation(idFilho, node->filhos[i+1], RIGHT)) && result != -1) {

	node->chaves[i] = result;

      } else {/* Nenhum tem espaco. */
      	middle = split(idFilho);
      
      	j = node->numChaves;
	node->filhos[j+1] = node->filhos[j];
	for(; j > i; j--) {
      	  node->filhos[j] = node->filhos[j-1];
      	  node->chaves[j] = node->chaves[j-1];
        }
      
      	node->chaves[j] = middle.key;
      	node->filhos[j+1] = middle.pointer;
      	
      	tamanho = ++(node->numChaves);
      }
      writeNode(node);
      result = TRUE;
    }
  }
	 
  if (tamanho == B_ORDER) return OVERFLOW;
  else return result;	
}

/* Aloca memoria para um novo node. */
BTNode * makeNode() {
  BTNode * resp = (BTNode *) malloc (sizeof(BTNode));
	
  resp->numChaves = 0;

  resp->left = -1;
  resp->right = -1;
	
  return resp;
}

/**
 * @brief Carrega um arquivo da arvore para memoria (BTNode).
 * 
 * @param node Onde ser a armazenado o arquivo lido.
 * @param nodeId Id do arquivo a ser carregado
*/
int readNode(BTNode *node, int nodeId) {
  
  FILE *file;
  char nomeArq[TAM_NOME_ARQ];
  int i;
 
  /*Colocando id do arquivo na estrutura*/
  node->id = nodeId;
  
  sprintf(nomeArq, "%d%s", nodeId, EXTENSAO_NODE);
  file = fopen(nomeArq, "r");
  
  if (file) { /*Se o arquivo existe*/
    
    /*Leitura dos campos do arquivo*/
    
    /*Leitura se e folha ou no*/
    fscanf(file, "%d", &node->leaf);
    
    /*Leitura do numero de chaves*/
    fscanf(file, "%d", &node->numChaves);
    
    /*leitura das chaves*/
    for ( i = 0; i < node->numChaves; ++i) {
      fscanf(file, "%d", &node->chaves[i]);
    }
    
    /*Leitura dos apontadores ou nrrs, caso seja folha*/
    for ( i = 0; i <= node->numChaves; ++i) {
      fscanf(file, "%d", &node->filhos[i]);
    }
    
    /*Leitura dos irmaos da esquerda e da direita (-1 caso nao seja folha)*/
    fscanf(file, "%d", &node->left);
    fscanf(file, "%d", &node->right);
    
    fclose(file); 
    return 1;
    
  } else {
    return 0; /* Nao existe o arquivo. */
  }

}

/*Grava os dados do no em memoria para o arquivo*/
void writeNode(BTNode *node) {
  
  FILE *file;
  char nomeArq[TAM_NOME_ARQ];
  int i;
  
  sprintf(nomeArq, "%d%s", node->id, EXTENSAO_NODE);
  file = fopen(nomeArq, "w");
  
  /*Gravacao dos dados*/
  
  /*Gravacao se e folha ou no*/
  fprintf(file, "%d\n", node->leaf);

  /*Gravacao do numero de chaves*/
  fprintf(file, "%d\n", node->numChaves);

  /*Gravacao das chaves*/
  for (i = 0; i < node->numChaves; ++i) {
    fprintf(file, "%d ", node->chaves[i]);
  }

  fprintf(file, "\n");

  /*Gravacao dos apontadores ou nrrs, caso seja folha*/
  for (i = 0; i <= node->numChaves; ++i) {
    fprintf(file, "%d ", node->filhos[i]);
  }

  fprintf(file, "\n");

  /*Gravacao dos irmaos da esquerda e da direita (-1 caso nao seja folha)*/
  fprintf(file, "%d\n", node->left);
  fprintf(file, "%d\n", node->right);
  
  fclose(file);
}

/*Pega o id do arquivo da arvore a ser gravado. */
int getId() {
  int id = 0; 
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
  BTNode *right = makeNode();
  
#ifdef DEBUG
  fprintf(stderr, "Split node %d\n", nodeId);
#endif

  /*Carrego o filho que tem overflow*/
  readNode(node, nodeId);
  readNode(right, node->right);
  
  meio = node->numChaves / 2 + 1;
  
  new->id = getId();
  /* Copia metade dos dados de um indice para outro. */
  if(node->leaf) { /* Folha */

    for(j = meio; j < node->numChaves; j++) {
      new->chaves[j-meio] = node->chaves[j];
      new->filhos[j-meio] = node->filhos[j];
    }

    new->leaf = TRUE;
    node->numChaves = meio;
    new->numChaves = j-meio;
  
  } else { /* Tronco */

    for(j = meio; j < node->numChaves; j++) {
      new->chaves[j-meio] = node->chaves[j];
      new->filhos[j-meio] = node->filhos[j];
    }
    new->filhos[j-meio] = node->filhos[j];

    new->leaf = FALSE;

    node->numChaves = meio-1;
    new->numChaves = j-meio;

  }
  
  /*Atualizo os apontadores e retorno pk, que contem a chave a ser inserida no pai.*/
  retorno.key = node->chaves[node->numChaves];
  retorno.pointer = new->id;

  /* Atualiza ponteiros */
  node->right = new->id;
  new->left = node->id;
  new->right = right->id;
  right->left = new->id;
  
  /*Gravo os novos arquivos na base, libero memoria.*/
  writeNode(new);
  writeNode(node);
  free(node);
  free(new);
  
  return retorno;
}

/*Rotacao de acordo com o parametro LEFT/RIGHT */
int rotation(int idFilho, int idIrmao, const int tipo) {

  int j, chave;
  BTNode *filho, *irmao;
  irmao = makeNode();
  readNode(irmao, idIrmao);

  /*Caso o irmao esteja cheio, retorna*/
  if (irmao->leaf != TRUE || irmao->numChaves >= B_ORDER - 1) return -1;

#ifdef DEBUG
  fprintf(stderr, "Rotation!\n");
#endif

  filho = makeNode();
  readNode(filho, idFilho);

  if (tipo == LEFT) { /*Rotacao no irmao da esquerda*/
   
    irmao->chaves[irmao->numChaves] = filho->chaves[0];
    irmao->filhos[irmao->numChaves] = filho->filhos[0];

    for(j = 0; j < filho->numChaves; j++) {
      filho->filhos[j] = filho->filhos[j+1];
      filho->chaves[j] = filho->chaves[j+1];
    }

    chave = filho->chaves[0];


  } else {
   
    /*Abre espaco para que a chave a ser inserida no irmao seja a primeira da folha*/
    for(j = irmao->numChaves; j > 0; j--) {
      irmao->filhos[j] = irmao->filhos[j-1];
      irmao->chaves[j] = irmao->chaves[j-1];
    }
    irmao->chaves[0] = filho->chaves[filho->numChaves-1];
    irmao->filhos[0] = filho->filhos[filho->numChaves-1];

    chave = irmao->chaves[0];
  }

  (filho->numChaves)--;
  (irmao->numChaves)++;

  writeNode(filho); writeNode(irmao);

  free(irmao); free(filho);

  /*ROTACAO CONCLUIDA COM SUCESSO. PRECISA AGORA ATUALIZAR O PAI, QUE CHAMOU A ROTACAO DOS FILHOS   */
 
  return chave;
}

void rootOverflow() {
  pk middle;
  BTNode * root;

  root = makeNode();
  readNode(root, 0);
		
  root->id = getId(); /* Pega um novo ID para o antigo root. */
  writeNode(root);
	
  middle = split(root->id);

  root->filhos[0] = root->id;
  root->chaves[0] = middle.key;
  root->filhos[1] = middle.pointer;
	
  root->numChaves = 1;
  root->leaf = FALSE;

  root->id = 0;

  writeNode(root);
  free(root);
	
}

/**
 * @brief Funcao recursiva de busca na arvore.
 * 
 * @param key A chave a ser buscada.
 * @param node O no atual a ser buscado.
 * return O ponteiro do arquivo de indice a ser aberto.
*/

int search(BTNode *node, int procura){
  int i = 0, resposta;

#ifdef DEBUG
  fprintf(stderr, "Busca em Arvore \n");
#endif

  printf("%d\n" , i);
  while (procura > node->chaves[i] && i < node->numChaves){ 
    i++;
    printf("%d\n" ,i);
  }
  printf("%d" , i);
  if (node->leaf == TRUE) { /* O Node eh uma folha. */

    if (node->chaves[i] == procura){
      
      resposta = node->filhos[i];
      printf("Busca concluida com sucesso!\n");
      return resposta; /*Encontrou o resultado*/

    }else{

      printf("O numero %i nao foi encontrado\n", procura);
      return 0;/*Nao encontrou o resultado esperado*/
    
    }
    	 	
  } else { /* O node eh pai de outro node. */
    
    resposta = search(&(node->filhos[i]), procura); 	
    return resposta;

  }
}

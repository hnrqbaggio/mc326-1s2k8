#ifndef BPLUS_H_
#define BPLUS_H_
#include <stdio.h>
#include <stdlib.h>

#define DEBUG

/**
 * Definicoes para facilitar a vizualizacao do codigo da arvore B+
*/
#define TRUE 1
#define FALSE 0
#define OVERFLOW -1
#define UNDERFLOW -1
#define RIGHT 1
#define LEFT 2

/**
 * Ordem da arvore b+.
*/
#define B_ORDER 5
#define MINIMO (B_ORDEM/2)

/**
 * Preenchimento minimo do no da arvore.
*/
#define MINIMO B_ORDER/2

/**
 * Tamanho maximo do nome do arquivo.
*/
#define TAM_NOME_ARQ 60

/**
 * Extencao do arquivo reservado para gravar o id do ultimo arquivo gravado.
*/
#define ARQUIVO_ID "id.bplus"

/**
 * Extencao do arquivo de no da arvore.
*/
#define EXTENSAO_NODE ".bplus"

typedef struct {
  int numChaves; /**< Numero de chaves no noh. Usado para controlar operacoes da arvore. */
  int chaves[B_ORDER]; /**< vetor de inteiros que sao os delimitadores das chaves. */
  int filhos[B_ORDER+1]; /**< Vetor de ponteiros para os filhos do noh. */
  int leaf; /**< Nivel do noh - sua altura. A raiz tem level = altura - 1. */
  int left; /**< Apontador para esquerda. */
  int right;/**< Apontador para direita.*/
  int id; /**<Id do arquivo para ser gravado em disco. */
} BTNode;

typedef struct {
  int key; /**<Inteiro que e a pk do indice.*/
  int pointer; /**<Inteiro usado como o nrr do indice primario. */
} pk;


/* Funcoes */

/**
 * @brief Funcao de insercao.
 * 
 * Funcao que busca recursivamente o local a ser inserido.
 * 
 * @param key A chave a ser inserida.
 * @param nodeId O no a ser aberto para a insercao.
 * @return  1:ok, 0:repetido, -1:overflow do node 
*/
int insert(pk *key, int nodeId);

/**
 * @brief Aloca memoria para no da arvore.
 * 
 * @return O no alocado em memoria, inicializado com o nivel.
*/
BTNode * makeNode();

/* I/O */

/**
 * @brief Le um arquivo da arvore, no ou folha.
 * 
 * @param node O no em memoria onde sera armazenado.
 * @param nodeId O id do arquivo a ser carregado.
*/
int readNode(BTNode *node, int nodeId);

/**
 * @brief Grava um no da arvore para o disco.
 * 
 * @param node O no a ser gravado.
*/
void writeNode(BTNode *node);

/**
 * @brief Le do arquivo o inteiro que sera o nome do arquivo a ser gravado na arvore.
 * 
 * @return O numero do novo arquivo a ser gravado.
*/
int getId();

/**
 * @brief Carrega um arquivo que possui overflow, e faz split. 
 * 
 * @param nodeId O id do arquivo a ser carregado.
 * @return Pk contendo o middle a ser inserido no pai.*/
pk split(int nodeId);

/**
 * @brief Funcao que faz rotacao no no/folha da arvore.
 * 
 * @param id O id do arquivo a ser rotacionado.
 * @param idIrmao O id do arquivo para onde sera transferido os dados de id.
 * @param tipo LEFT/RIGHT Define se a rotacao e para esquerda ou direita. 
 * @return -1 caso nao ocorra rotacao, caso contrario, retorna um inteiro
 * que e a chave a ser atualizada.
*/
int rotation(int id, int idIrmao, const int tipo);

/**
 * @brief Funcao que faz rotacao no no/folha da arvore para a funcao de remocao.
 * 
 * @param idFilho O arquivo a receber uma chave da rotacao.
 * @param idIrmao O arquivo a fornecer, caso possivel, uma chave para idFilho.
 * @param tipo LEFT/RIGHT Define se a rotacao e para esquerda ou direita. 
 * @return -1 caso nao ocorra rotacao, caso contrario, retorna um inteiro
 * que e a chave a ser atualizada.
*/
int removeRotation(int idFilho, int idIrmao, const int tipo);

void rootOverflow();

void leRegistro(); /* Pegar em catalogo.c */


/**
 * @brief Funcao recursiva de busca na arvore.
 * 
 * @param key A chave a ser buscada.
 * @param node O no atual a ser buscado.
 * return O ponteiro do arquivo de indice a ser aberto.
*/
int search(BTNode *node, int procura);

int remove(pk *key, int nodeId);

int merge();


#endif /*BPLUS_H_*/

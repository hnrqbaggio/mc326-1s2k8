#ifndef BPLUS_H_
#define BPLUS_H_

/* Operacoes na Arvore B+
 * Insert - Inserir uma chave numa folha
 * Remove - Remover uma chave de uma folha
 * Search - Encontrar a folha que contem uma chave
 * 
 * Operacoes sobre os nos
 * Split - Quebra um noh em dois, quando falta espaco
 * Merge - Junta dois nos
 * 
 * Operacoes sobre folhas
 * Insert - Inserir a chave no arquivo de indice
 * Remove
 * MakeLeaf - Cria uma nova folha, alocando espaco para ela
 * 
 * */

/* == Insercao em um noh ==
 * 
 * Procura pela CHAVE - busca sequencial ou naum pelo vetor de keys
 *
 * Se o nivel eh 1
 * 	insere na folha filha
 * Senao
 * 	insere no noh filho (recursao)
 * 
 * Se houve overflow do filho
 * 	Insere uma nova key que eh igual a CHAVE, antes da key do filho que teve overflow.
 * 		(isso implica em mover os ponteiros também ¬¬)
 * 	Se o filho for um noh
 *  	Cria um novo filho, e transfere a primeira metade do do filho 
 *   	com over pro novo. Associa esse novo filho a nova chave inserida.
 * 	Se o filho for uma folha
 * 		cria uma nova folha, e copia os primeiros registros pra um novo indice, 
 * 		associado a nova folha, entao salva ambos os indices.
 * 
 * Se inseriu normalmente, retorne 1
 * Se a chave jah existia, retorne 0
 * Se deu overflow do noh (nao do filho), retorne -1
 * 
 * IMPORTANTE - OVERFLOW DO ROOT
 * Cria um novo ROOT com a maior chave do atual
 * Cria um novo noh e coloca o atual ROOT e o novo noh 
 * 	como filhos do novo ROOT.
 * Salva os dois nos e tambem o novo ROOT.
 * Retorna -1 (isso faz o main recarregar o root, que vai tah atualizado)
 */
 
 /* == Insercao numa folha ==
  * Recebe uma string, a CHAVE
  * Carrega o indice da memoria pro arquivo
  * Insere normalmente no indice
  * Salva o indice no arquivo
  * 
  * Se deu overflow do indice, retorne -1
  * Se inseriu normalmente, retorne 1
  * Se a chave jah existia, retorne 0
  */

#include <stdio.h>
#include <string.g>
#include "tipos.h"

BTNode * makeNode();
BTLeaf * makeLeaf();

int insert(char *key, BTNode *node, Index *indice);
int insertInLeaf(char *key, BTLeaf *leaf, Index *indice);

void merge(BTNode *);
void split(BTNode *);


#endif /*BPLUS_H_*/

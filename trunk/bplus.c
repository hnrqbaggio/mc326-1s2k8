#include "bplus.h"

int insertInLeaf(char *key, BTLeaf *leaf) {
	FILE *arqIndice;
	
	
	
}

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
	 
	 int i = 0, temp, meio;
	 BTLeaf *folha, *novaFolha;
	 BTNode *filho, *novoFilho;
	 Index * novoIndice, * swap;
	 
	 while (strcmp(chave, node->chaves[i]) < 0) i++;
	 
	 if (node->level == 1) {
	 	folha = openLeaf(node->filhos[i], indice);
	 	temp = insertInLeaf(key, folha);
	 	
	 	if (indice->tamanho == BLOCK_SIZE) {
	 		novaFolha = makeLeaf();
	 		indice = makeIndex(indice->tipoCampo);
	 		meio = indice->tamanho / 2;
	 		
	 		for(j = meio; j < indice->tamanho; j++) {
	 			strcpy(novoIndice->vetor[meio-j].key, indice->vetor[j].key);
	 			novoIndice->vetor[meio-j].nrr =  indice->vetor[j].nrr;
	 		}
	 		
	 		novoIndice->tamanho = meio;
	 		indice->tamanho -= meio;
	 		novoIndice->tamFile = indice->tamFile;
	 		novoIndice->id = novaFolha->id;
	 		
	 		swap = novoIndice; novoIndice = indice; indice = swap;
	 		
	 		indice->grava(indice);
	 		novoIndice->grava(novoIndice);
	 		
	 		for(j = node->numChaves; j > i;) {
	 			node->filhos[j] = node->filhos[j--];
	 			node->chaves[j] = node->chaves[j--];
	 		}
	 		node->filhos[j+1] = node->filhos[j];
	 		
	 		strcpy(node->chaves[i], key);
	 		node->filhos[i] = novaFolha->id;
	 	}
	 	
	 } else {
	 	
	 }
	 
	
}
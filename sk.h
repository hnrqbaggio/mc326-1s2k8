#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Constantes usadas para determinar o tipo do campo para o indice
 */
#define TITULO 0
#define TIPO 1
#define AUTOR 2
#define ANO 3

/** Constante usada para indicar o fim de uma lista invertida. */
#define FIM_DE_LISTA -1

/**
 * Estrutura que representa um chave secundaria.
 * 
 * key eh o vetor que contem a string que eh a chave propriamente
 * dita. next eh um apontador para a proxima chave no indice, e lenght
 * eh o tamanho da string.
*/
typedef struct _sk{
  char key[TAM_TITULO];
  int next;
  int lenght;
} Sk;

/**
 * Estrutura que representa um indice secundario, formado por um vetor
 * de chaves secundarias e quatro inteiros que indicam o numero de
 * elementos no indice, o espaco alocado na memoria para este indice,
 * o tamanho da parte do indice no disco, e o noh cabeca da avail list
 * desse indice, que sera usado na operacao de insercao e remocao de
 * chaves.
 */
typedef struct _vetorsk{
  Sk *vetor;
  int tamanho;
  int alocado;
  int tamDisco;
} IndSec;

/**
 * Este inteiro será usado para representar a cabeca de uma avail
 * list, que sera um alista invertida de posicoes livres no disco.
 */
typedef int availList;

/**
 * Constroi um indice secundario.  Se existe o arquivo de indice, ele
 * carrega parte de estrutura pra memoria e deixa a outra no disco.
 * Caso nao exista o indice, ele ira constri-lo a partir da base de
 * dados.
 *
 * Para isso, ela chama as funcoes auxiliares carregaSk e criaSk.
 * 
 * Parametros: O indice primario, a base de dados e uma constante que
 * indica o tipo de campo cujo indice sera gerado.
 *
 * Retorno: um ponteiro pra um vetor de sks.
 */
IndSec * geraSk(TIndice *, FILE *, availList*, const int);

/**
 * Carrega um indice secundario a partir do arquivo em disco.
 * Armazena a parte que ficara na RAM em um vetor de SKs e retorna o
 * ponteiro para a estrutura que contem esse vetor e intormacaoes
 * sobre o seu tamanho.
 *
 * Parametro: o ponteiro pra o arquivo de indice.
 */
IndSec * carregaSk(FILE *, availList* );

/**
 * Cria o indice secundario a partir da base de dados, carregando as
 * informacoes de cada registro.
 *
 * Essa funcao aloca espaco pro vetor de SKs. Ha uma funcao especifica
 * para liberar esse espaco.
 * 
 * Parametros: o indice primario, a base de dados e uma constante que
 * indica o tipo de campo que formara o indice.
 *
 * Retorna o ponteiro para a estrutura que representa o vetor de SKs.
 */
IndSec * criaSk(TIndice *, FILE *, availList *, const int);

/**
 * Insere uma SK no vetor que contem o indice secundario pra um
 * determinado campo, e atualiza a lista invertida no disco.
 *
 * Parametros: o vetor de SKs, um ponteiro pro arquivop de indice, uma
 * string com a chave primaria do registro, um string com o token e
 * uma referencia ao valor do noh cabeca da avail list do arquivo de
 * indice.
 *
 * Retorno: O vetor de SKs atualizado.
 */
IndSec * insereSk(IndSec *indSecun, FILE *fsk, char *pk, char *campo, availList *avail);

/**
 * Funcao usada para realocar espaco para o vetor da SK caso seja
 * necessario. Ela ira verificar se o numero de elementos eh
 * sufuciente para o espaco alocado para o vetor e reservar mais caso
 * seja necessario.
 *
 * Parametros: o vetor de SKs.
 *
 * Retorno: O vetor de SKs, realocado ou nao. 
 */
IndSec * realocaIndSec(IndSec *);

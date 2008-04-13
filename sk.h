#include <stdio.h>
#include <catalogo.h>

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
  int head;
} IndSec;

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
IndSec * geraSk(TIndice *, FILE *, const int).

/**
 * Carrega um indice secundario a partir do arquivo em disco.
 * Armazena a parte que ficara na RAM em um vetor de SKs e retorna o
 * ponteiro para a estrutura que contem esse vetor e intormacaoes
 * sobre o seu tamanho.
 *
 * Parametro: o ponteiro pra o arquivo de indice.
 */
IndSec * carregaSk(FILE *).

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
IndSec * criaSk(TIndice *, FILE *, const int).

/**
 * Insere uma SK no vetor que contem o indice secundario pra um
 * determinado campo, e atualiza a lista invertida no disco.
 *
 * Parametros: o vetor de SKs e uma string que contem o valor do campo do registro.
 *
 * Retorno: O vetor de SKs atualizado.
 */
IndSec * insereSk(SkVetor *, char *).

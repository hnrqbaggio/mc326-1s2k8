#ifndef _SK_H
#define _SK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tipos.h"
#include "hash.h"

IndSec * inicializaSecundario(int);

/**
 * Constroi todos os indices secundarios.  Tenta carregar o arquivo zero 
 * de cada indice e se algum falhar, constroi todos a partir da base de dados.
 * 
 * Parametros: O indice primario e a base de dados, 
 * os quatro indices secundarios e suas avail lists
 *
 * Retorno: um ponteiro pra um vetor de sks.
 */
void constroiSecundarios(IndicePrim *, FILE *, IndSec *, IndSec *, IndSec *, IndSec *, availList *, availList *, availList *, availList *);

IndSec * trocaIndSec(IndSec *, char *);

/**
 * Carrega um indice secundario a partir do arquivo em disco.
 * Armazena a parte que ficara na RAM em um vetor de SKs e retorna o
 * ponteiro para a estrutura que contem esse vetor e intormacaoes
 * sobre o seu tamanho.
 *
 * Parametro: o ponteiro pra o arquivo de indice.
 */
IndSec * carregaSk(FILE *);

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
IndSec * criaSk(IndicePrim *, FILE *, availList *, const int);

/**
 * Insere uma SK no vetor que contem o indice secundario pra um
 * determinado campo, e atualiza a lista invertida no disco.
 *
 * Parametros: o vetor de SKs, um ponteiro pro arquivo de chaves primarias,
 * uma string com a chave primaria do registro, um string com o valor do 
 * campo relativo ao indice e uma referencia ao valor do noh cabeca da 
 * avail list do arquivo de indice.
 *
 * Retorno: O vetor de SKs atualizado.
 */
IndSec * insereSk(IndSec *, FILE *, char *, char *, availList *);

/**
 * Grava o indice secundário da RAM para o arquivo no final deste, e 
 * grava também o tamanho do arquivo que sempre esta em disco.
 * Libera a memoria utilizada nesses e fecha o arquivo de SK.
 *
 * Parametros: o vetor de SKs.
 *
 * Retorno: void 
*/
void gravaIndSk(IndSec *);

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

/**
 *comenta aki porra
*/
int compareSk(const void *, const void *);

#endif

/** \file sk.h
 *  \brief Biblioteca que implementa as funcoes
 *         de manipulacao dos indices secundarios.
 */

#ifndef _SK_H
#define _SK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tipos.h"
#include "hash.h"

/** @brief Inicializa um indice secundario,
 * alocando memoria para a sua propria estrutura
 * e tamabem para o vetor de SKs. Tambem inicializa
 * os demais campos do indice com valores apropriados.
 * 
 * @param campo Uma string que determina o campo da obra
 * ao qual o indice se refere.
 * 
 * return O Indice alocado com valores iniciais.
 */
IndSec * inicializaSecundario(char *campo);

/** @brief Constroi todos os indices secundarios de uma vez.*/
/** 
 * Essa operacao eh feita de uma soh vez para economizar acessos 
 * ao disco, percorrendo a base de dados no maximo uma unica vez.
 * 
 * Tenta carregar o arquivo zero de cada indice e se algum falhar, 
 * constroi todos a partir da base de dados.
 * 
 * @param indPrimario Eh o indice primario. 
 * Eh atualizado caso o hash de alguma chave a ser inserida/carregada 
 * seja diferente do seu.
 * @param base O ponteiro para o arquivo da base de dados.
 * @param indTitulo indice secundario de titulos.
 * @param indTipo indice secundario de tipos.
 * @param indAutor indice secundario de autores.
 * @param indAno indice secundario de anos.
 * @param avTitulo Avail List do indice de titulos.
 * @param avTipo Avail List do indice de tipos.
 * @param avAutor Avail List do indice de autores.
 * @param avAno Avail List do indice de ano.
 */
void constroiSecundarios(IndPrim *indPrimario, FILE *base, IndSec *indTitulo, IndSec *indTipo, IndSec *indAutor, IndSec *indAno, availList *avTitulo, availList *avTipo, availList *avAutor, availList *avAno);
/** @brief Troca o indice secundario atual por outro 
 *  no disco de acordo com o valor hash da string.
 * 
 *  @param indice O Indice secundario.
 *  @param chave A chave secundaria cujo hash se deseja calcular.
 *  return O indice secundario atualizado. */
IndSec * trocaIndSec(IndSec *indice, char *chave);

/**
 * @brief Carrega um indice secundario a partir do arquivo em disco.
 * @param indice O ponteiro para o indice secundario vazio.
 * @param bigFile O BigFile associado ao campo da obra ao qual 
 * 					o indice se refere, formado pela lista invertida
 * 					de chaves primarias cujos registros contem a SK.
 * @return O ponteiro para o indice contendo os registros.
 */
IndSec * carregaSk(IndSec *indice, FILE *bigFile);

/**
 * @brief Insere as SKs de uma string relativa a um campo da obra
 * num indice secundario. Atualiza a lista invertida no BigFile.
 *
 * @param indice O indice secundario.
 * @param bigFile Um ponteiro para o BigFile do indice.
 * @param pk A chave primaria do registro.
 * @param campo A string do campo que ira gerar as chaves.
 * @param avail Um ponteiro para a Avail List do indice.
 *
 * @return O Indice com a nova chave inserida.
 */
IndSec * insereSk(IndSec *indice, FILE *bigFile, char *pk, char *campo, availList *avail);

/**
 * @brief Grava as informacoes do indice secundario no disco.
 * @param indice O indice secundario.
 * @return N�o tem.
*/
void gravaIndSk(IndSec *indice);

/** @brief Funcao usada para realocar espaco para o vetor da SKs do indice caso seja necessario. */
/** 
 * Ela ira verificar se o numero de elementos eh
 * sufuciente para o espaco alocado para o vetor e reservar mais caso
 * seja necessario.
 *
 * @param indice O Indice a ser realocado.
 *
 * @return O Indice, com seu vetor realocado ou nao. 
 */
IndSec * realocaIndSec(IndSec *indice);

/**
 * @brief Funcao que compara duas chaves secundarias.
 * @param a Um ponteiro para void que sera comparado (com um cast para Sk*).
 * @param b O outro ponteiro usado na comparacao (tambem com cast).
 * @return Um inteiro que vale -1 se o campo key da chave a eh 
 * alfabeticamente menor que o da chave b, 1 se for maior, ou 0 se forem iguais.
*/
int compareSk(const void *a, const void *b);

#endif

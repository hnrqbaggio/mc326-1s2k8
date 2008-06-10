/** @file catalogo.h
 * @brief Biblioteca que implementa as funcoes relacionadas a base de dados e ao indice primario. 
 */

#ifndef _CATALOGO_H
#define _CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** @{
 * @name Definicoes de tamanhos de campos e do tamanho do registro 
 */

#define TAM_TITULO 200  /**< Tamanho do campo titulo da obra. */
#define TAM_TIPO 100	/**< Tamanho do campo tipo da obra. */
#define TAM_AUTOR 125	/**< Tamanho do campo autor da obra. */
#define TAM_ANO 4       /**< Tamanho do campo ano da obra. */
#define TAM_VALOR 12    /**< Tamanho do campo valor da obra. */
#define TAM_IMAGEM 9	/**< Tamanho do campo imagem da obra. */
#define TAM_REG (TAM_TITULO+TAM_TIPO+TAM_AUTOR+TAM_ANO+TAM_VALOR+TAM_IMAGEM)/**< Tamanho total de um registro. */

/** @} */

/** @{
 * @name Definicoes para os nomes dos arquivos
 */
#define ARQ_BASE ("/tmp/base00.dat")       /**< Base de dados. */
#define ARQ_HTML ("/tmp/base24.html")      /**< Html para saida dos resultados de busca. */
#define ARQ_PK ("primario.pi")         /**< Arquivo que contem os registros do indice primario. */
#define ARQ_AVAIL_BASE ("base24.av")  /**< Avail list da base de dados. */
/** @} */

/** 
 * Constante para fazer reallocs mais eficientes. A primeira alocacao
 * sera feita com esse valor, e dobrando sempre que atingir o maximo 
 */
#define VETOR_MIN 20

/** @{
 * @name Constantes para gravacao de campos numericos.
 */
/** Define para a mascara usada para ler e gravar inteiros. */
#define FORMATO_INT ("%08d")
/** Define para o numero de digitos dos inteiros. */
#define TAM_NUMERO 8
/** @} */

/** @{
 * @name Constante usadas como parametros na funcao geraHtml.
 */
#define ALL 0  /**< Adiciona tanto o cabecalho quanto o fim do html. Usado quando e uma consulta de um registro especifico.*/
#define HEAD 1 /**< Deve ser inserido um cabecalho no html */
#define END 2  /**< Deve fechar as tags do html: </table></body></html> */
#define MEIO 3 /**< Diferencia os registros que nao sao nem inicio nem fim da listagem, logo, devem apenas ser inseridos */
/** @} */
 
/** @brief Estrutura usada para representar uma obra de arte.*/
typedef struct {
  char titulo[TAM_TITULO + 1];   /**< Titulo da obra de arte. */
  char tipo[TAM_TIPO + 1];       /**< Tipo da obra. */
  char autor[TAM_AUTOR + 1];     /**< Nome do autor. */
  char ano[TAM_ANO + 1];         /**< Ano de publicacao. */
  char valor[TAM_VALOR + 1];     /**< Valor da Obra. */
  char imagem[TAM_IMAGEM + 1];   /**< Imagem correspondente. */
} TObra;

/** @brief Estrutura usada para representar uma chave primaria. */
typedef struct {
  char pk[TAM_TITULO + 1]; /**< String usada para armazenar a chave primaria propriamente dita. */
  int nrr; /**< Numero relativo do registro na base de dados. */
} ElementoIndice;

/** @brief Estrutura que representa o indice primario. */
typedef struct {
  ElementoIndice * vetor;    /**< Vetor eh um vetor dinamico que ira armazenar os elementos de indice.*/
  int tamanho;   /**< Eh o numero de elementos validos, assim, indica tambem a proxima posicao livre do vetor. */
  int alocado;   /**< Espaco reservado na memoria para o vetor. Aumanta conforme o tamanho do indice se iguala a ele. */
} TIndice;

/**
 * Este inteiro será usado para representar a cabeca de uma avail
 * list, que sera um alista invertida de posicoes livres no disco.
 */
typedef int availList;


/** @brief Abre a base de dados.
 *  
 * Abre um arquivo que sera a base de dados do programa e retorna um
 * ponteiro pra esse arquivo.
 * @param nome Nome do arquivo no disco.
 * @return Arquivo da base de dados.
 */
FILE * abreCatalogo(char *nome);

/** @brief Fecha o arquivo da base de dados.
 * 
 * @param arq Arquivo a ser fechado.
 * @return void
*/
void fechaCatalogo(FILE *arq);

/** brief Grava uma obra na base de dados.
 * 
 * @param obra Obra a ser gravada.
 * @param arq Base de dados.
 * @param avail Avail list da base de dados.
 * @return O nrr da obra inserida.
 */
int gravaObra(TObra obra, FILE *arq, availList *avail);

/** @brief Carrega o indice do arquivo para a memoria ou cria o indice caso o
 * arquivo nao exista.
 * 
 * @param base Base de dados.
 * @param indice Indice primario inicializado.
 * @param avail Avail list da base.
 * @return Indice primario.
 */
TIndice * carregaIndice(FILE *base, TIndice *indice, availList *avail);

/** @brief Realiza a ordenacao do indice primario utilizando qsort.
 * 
 * @param indice Indice primario.
 * @return void.
*/
void ordenaIndice(TIndice *indice);

/** @brief Grava o indice primario para o arquivo.
 * @param indice Indice primario.
 * @return void.
*/
void gravaIndice(TIndice *indice);

/** @brief Realiza uma consulta na base usando a chave primaria. 
 * 
 * @param chave Titulo a ser buscado no indice primario.
 * @param base Base de dados.
 * @param indice Indice primario.
 * @param reg Estrutura de obra de arte, utilizada para possivel remocao da obra consultada.
 * @return Inteiro que retorna se a obra buscada foi encontrada ou nao. 
*/
int consulta(ElementoIndice *chave, FILE *base, TIndice *indice, TObra *reg);

/** @brief Lista todos os registros da base de dados. 
 * 
 * @param base Base de dados.
 * @param indice Indice primario, que sera percorrido para listar as obras ordenadas.
 * @return void.
 */
void listaBase(FILE *base, TIndice *indice);

/** @brief Funcao de comparacao para as funcoes qsort e bsearch da stdlib.h.
 * 
 * Foi possivel usar uma unica funcao pois para a bsearch passamos nao
 * uma string, mas um elemento de indice, que eh do mesmo tipo que os
 * elementos do vetor no qual a busca eh feita. Isso permitiu usar uma
 * funcao de comparacao com ambos os parametros iguais, como ocorre
 * com a qsort.  
 * Os parametros da funcao recebem um cast para
 * ElementoIndice * e tem seus campos pk copiadaos para strings
 * auxiliares, as quais sao comparadas usando strcmp.
 * 
 * @param a Parametro a ser comparado.
 * @param b Parametro a ser comparado.
 * @return int Retorna se os parametros sao iguais ou nao.
 */
int compare(const void *a, const void *b);

/** @brief Funcao usada para realocar espaco para o vetor do indice caso seja necessario. 
 * 
 * Ela verifica se o numero de elementos e sufuciente para o espaco alocado para o vetor 
 * e reservar mais caso seja necessario.
 *
 * @param ind Indice Primario.
 * @return Indice Primario.  
 */
TIndice * realocaIndice(TIndice *ind);

/** @brief Gera cabecalho do arquivo html.
 * 
 * @param b Arquivo html.
 * @return Arquivo html.
 */
FILE * headHtml(FILE *b);

/** @brief Gera o final do html.
 * 
 * @param b Arquivo html.
 * @return Arquivo html.
 */
FILE * endHtml(FILE *b);

/** @brief Insere uma obra na tabela de obras de arte do arquivo HTML.
 * 
 * @param b Arquivo html.
 * @param valores Obra de arte a ser mostrada no arquivo.
 * @return Arquivo html.
 */
FILE * preencheHtml(FILE *b, TObra valores);

/** @brief Transforma para maiuscula a string passada como parametro.
 * 
 * @param chave String a ser mudada para maiuscula.
 * @return void.
*/
void maiuscula(char *chave);

#endif

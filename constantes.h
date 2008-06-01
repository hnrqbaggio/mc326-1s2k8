#ifndef CONSTANTES_H_
#define CONSTANTES_H_

/* Definicoes de tamanhos de campos e do tamanho do registro */
#define TAM_TITULO 200
#define TAM_TIPO 100
#define TAM_AUTOR 125
#define TAM_ANO 4
#define TAM_VALOR 12
#define TAM_IMAGEM 9
#define TAM_REG (TAM_TITULO+TAM_TIPO+TAM_AUTOR+TAM_ANO+TAM_VALOR+TAM_IMAGEM)

/* Defines para os nomes dos arquivos.*/
#define TAM_NOME_ARQ 256
#define ARQ_BASE ("/tmp/base00.dat")       /** Base de dados */
#define ARQ_HTML ("/tmp/base24.html")      /** Html para saida dos resultados de busca */
#define ARQ_PK ("primario")                /** Arquivo que contem os registros do indice primario */
#define ARQ_AVAIL_BASE ("base24.av")       /** Avail list da base de dados */
#define EXTENSAO_PK (".chp")
#define EXTENSAO_SK (".chs")

/** 
 * Constante para fazer reallocs mais eficientes. A primeira alocacao
 * sera feita com esse valor, e dobrando sempre que atingir o maximo 
 */
#define VETOR_MIN 20

/** Define para a mascara usada para ler e gravar inteiros. */
#define FORMATO_INT ("%08d")

/** Define para o numero de digitos dos inteiros. */
#define TAM_NUMERO 8

/*__________Constantes usadas como parametros na funcao geraHtml_____________*/

/**
 * Adiciona tanto o cabecalho quanto o fim do html. 
 * Usado quando eh uma consulta de um registro especifico. 
 */
#define ALL 0 
#define HEAD 1 /** Deve ser inserido um cabecalho no html */
#define END 2 /** Deve fechar as tags do html: </table></body></html> */

/** 
 * Diferencia os registros que nao sao nem inicio nem 
 * fim da listagem, logo, devem apenas ser inseridos 
 */
#define MEIO 3 

/* Nomes dos arquivos de indices secundarios. */
#define ARQ_IS_TITULO ("titulo")
#define ARQ_IS_TIPO ("tipo")
#define ARQ_IS_AUTOR ("autor")
#define ARQ_IS_ANO ("ano")

/* Nomes dos arquivos de avail list. */
#define ARQ_AVAIL_TITULO ("titulo.av")
#define ARQ_AVAIL_TIPO ("tipo.av")
#define ARQ_AVAIL_AUTOR ("autor.av")
#define ARQ_AVAIL_ANO ("ano.av")

/** Constantes usadas para determinar o tipo do campo para o indice. */
#define TITULO 0
#define TIPO 1
#define AUTOR 2
#define ANO 3

/** Constante usada para indicar o fim de uma lista invertida. */
#define FIM_DE_LISTA -1

#define H 53

/** 
 * Mensagem de erro enviada ao usuario caso uma entrada seja grande
 * demais para o campo da obra, ou esteja fora do padrao
 * estabelecido. 
 */
#define MensagemErro() (printf("Entrada invalida. Digite novamente.\n"))

/** 
 * Pseudo-funcao usada para limpar o buffer do teclado e descartar o
 * excesso de dados digitado pelo usuario numa entrada.  
 * Nota: A funcao __fpurge pode ser incompativel com Windows (Testado no LMS).
 */
#define LimpaBuffer() (__fpurge(stdin))

#endif /*CONSTANTES_H_*/
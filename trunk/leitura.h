#ifndef _LEITURA_H
#define _LEITURA_H

#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>

/* 
 * Mensagem de erro enviada ao usuario caso uma entrada seja grande
 * demais para o campo da obra, ou esteja fora do padrao
 * estabelecido. 
 */
#define MensagemErro() (printf("Entrada invalida. Digite novamente.\n"))

/* 
 * Pseudo-funcao usada para limpar o buffer do teclado e descartar o
 * excesso de dados digitado pelo usuario numa entrada.  
 * Nota: A funcao __fpurge pode ser incompativel com Windows (Testado no LMS).
 */
#define LimpaBuffer() (__fpurge(stdin))

/*** Funcoes de leitura das entradas do usuario ***/
/* 
 * Funcao responsavel pela leitura e tratamento da entrada do usuario.
 * Recebe o indice primario para verificar se a obra ja existe e atualizar este.
 * Tambem recebe o valor do hash do atual indice primario.
 * Retorna a obra a ser inserida na base.
 */
TObra * leObra(IndicePrim *, TObra *, int *);

/*** Funcoes de leitura das entradas do usuario ***/
/* 
 * Essa funcao principal para insercao de uma nova obra. Utiliza
 * funcoes auxiliares que tratam tipos diferentes de dados e possiveis
 * entradas invalidas. Ao final de uma entrada do usuario, ele grava
 * os dados na base e atualiza o vetor de busca na memoria.
 */
void insereObra(FILE *, IndicePrim *);

/* 
 * Esta funcao realiza a leitura dos campos do tipo texto: Titulo,
 * Tipo e Autor.  
 *
 * Seus parametros sao um ponteiro para o campo, o
 * tamanho do mesmo, e uma mensagem que sera exibida na tela,
 * indicando qual campo esta sendo lido no momento. 
 */
void leTexto(char *, int, char *);

/* 
 * Esta funcao eh analoga a leTexto, difeindo somente pelo fato de
 * fazer a leitura dos campos numericos.
 */
void leNumeros(char *, int, char *);

/* 
 * Tambem eh analoga as duas anteriores, sendo usada para ler a
 * entrada do campo Imagem.
 */
void leImagem(char *, int, char *);

/* 
 * Preenche uma string com espacos em branco. os paramentros sao o
 * ponteiro da string e o tamanho da mesma.  Sera usada para completar
 * os campos da estrutura que representa a obra e assim manter o
 * tamanho dos campos fixos quando forem salvos na base.
 */
void preencher(char *, int);

#endif

/** @file leitura.h
 * @brief Biblioteca que implementa as funcoes relacionadas ao tratamento da entrada do usuario e 
 * insercao de obra de arte na base de dados. 
*/

#ifndef _LEITURA_H
#define _LEITURA_H

#include <stdio.h>
#include <string.h>
#include "catalogo.h"
#include "sk.h"

/** @brief Funcao responsavel pela leitura e tratamento da entrada do usuario.
 * 
 * @param ind Indice primario, para verificar se a obra ja existe e atualizar este.
 * @param obra Obra a ser retornada para insercao dos indices secundarios.
 * @return int Inteiro: se a insercao foi feita com sucesso (1) ou  nao (0).
 */
TObra * leObra(IndPrim *ind, TObra *obra);

/** @brief Leitura dos campos do tipo texto: Titulo,Tipo e Autor.  
 *
 * @param campo Onde sera armazenado a entrada do usuario.
 * @param tamanho Tamanho do campo a ser lido.
 * @param msg Mensagem de erro de acordo com o campo.
 * @return void. 
*/
void leTexto(char *campo, int tamanho, char *msg);

/** @brief Leitura dos campos numericos: Ano e Valor.
 * 
 * @param campo Onde sera armazenado a entrada do usuario.
 * @param tamanho Tamanho do campo a ser lido.
 * @param msg Mensagem de erro de acordo com o campo.
 * @return void.
*/
void leNumeros(char *campo, int tamanho, char *msg);

/** @brief Leitura do nome da Imagem.
 * 
 * @param campo Onde sera armazenado a entrada do usuario.
 * @param tamanho Tamanho do campo a ser lido.
 * @param msg Mensagem de erro de acordo com o campo.
 * @return void.
*/
void leImagem(char *campo, int tamanho, char *msg);

/** @brief Preenche uma string com espacos em branco. 
 * 
 * @param campo String a ser preenchida.
 * @param tamanho Tamanho do campo.
 * @return void.
 */
void preencher(char *campo, int tamanho);

/** 
 * Mensagem de erro enviada ao usuario caso uma entrada seja grande
 * demais para o campo da obra, ou esteja fora do padrao
 * estabelecido. 
 */
#define MensagemErro() (printf("Entrada invalida. Digite novamente.\n"))

/** 
 * Pseudo-funcao usada para limpar o buffer do teclado e descartar o
 * excesso de dados digitado pelo usuario numa entrada.  
 * Nota: A funcao __fpurge eh incompativel com Windows (Testado no LMS).
 */
#ifdef IS_WINDOWS
	#define LimpaBuffer() (fflush(stdin))
#else
	#include <stdio_ext.h>
	#define LimpaBuffer() (__fpurge(stdin))
#endif



#endif

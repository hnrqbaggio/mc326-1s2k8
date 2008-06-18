/** \file hash.h
 *  \brief Biblioteca que implementa as funcoes
 *         de espalhamento para os indices.
 */

#ifndef HASH_H_
#define HASH_H_

#include <string.h>
#include <ctype.h>
#include "constantes.h"

/** 
 * @brief Funcao de espalhamento dos descritores.
 * @param descritor Caracter descritor de uma imagem.
 * @return O valor da funcao de hash do @c descritor. 
*/
int hashDescritor(char descritor);

#endif /*HASH_H_*/

/* MC326 - Estrutura de Arquivos
 * TP2 - Catalogo de Obras de Arte
 * Grupo 24 */

/* 
 * Este programa implementa a gravacao dos dados de obras de arte
 * fornecidos pelo usuario num arquivo base.dat Tambem possibilita a
 * consulta de obras de arte atraves de uma pk, gerando o resultado em
 * html.
 */

#include <stdio.h>
#include "catalogo.h"
#include "leitura.h"
#include "sk.h"
#include "menu.h"
#include "remove.h"
#include "avail.h"
#include "busca.h"

int main(int argc, char **argv){

  int ent, option, end, resultado;
  TObra obra, obra2, consultaObra;
  char temp[TAM_TITULO];
 
  /* Ponteiro pra base de dados. */
  FILE *arq;
  /* Ponteiro utilizado para abrir os arquivos de indice secundario*/
  FILE *fsk;

  /* Variavais de indice primario e sua avail list da base de dados. */
  TIndice *ind;
  ElementoIndice *elem;
  availList availBase = FIM_DE_LISTA;

  /* Variaveis de indice secundario e respectivas avail lists. */
  IndSec *secTitulo, *secAutor, *secTipo, *secAno;
  availList availTitulo, availTipo, availAno, availAutor;

  /*Abre a availList da base, a base e o indice primario*/
  availBase = openAvail(ARQ_AVAIL_BASE);
  arq = abreCatalogo(ARQ_BASE);
  ind = carregaIndice(arq, ind, &availBase);

  /*Abre os indices secundarios*/
  secTitulo =  geraSk(ind, arq, &availTitulo, TITULO);
  secTipo =  geraSk(ind, arq, &availTipo, TIPO);
  secAutor =  geraSk(ind, arq, &availAutor, AUTOR);
  secAno =  geraSk(ind, arq, &availAno, ANO);

  /* Carrega as avail lists dos arquivos. */
  availTitulo = openAvail(ARQ_AVAIL_TITULO);
  availTipo = openAvail(ARQ_AVAIL_TIPO);
  availAutor = openAvail(ARQ_AVAIL_AUTOR);
  availAno = openAvail(ARQ_AVAIL_ANO);

  elem = (ElementoIndice *) malloc(sizeof(ElementoIndice));

  /*Entrada do programa*/
  printf("|------------------------------------|\n");
  printf("|Catalogo de Obras de Arte - Grupo 24|\n");
  printf("|------------------------------------|\n");

  /*Looping do menu do programa*/
  do {
    /*Imprime o menu principal e le a entrada do usuario*/
    ent = geraMenu();
		
    switch (ent) {
    case 1:/*Inserir nova obra*/
      /*Looping de insercao*/
      do {
        /*leitura da obra a ser inserida e gravacao no indice primario*/
        if(leObra(ind, &obra)) {/*Insercao feita com sucesso na PK*/
          /*Insercao nos indices secundarios*/
          /*titulo*/
          fsk = fopen(ARQ_IS_TITULO,"r+");
          strcpy(obra2.titulo, obra.titulo);
          secTitulo = insereSk(secTitulo, fsk, obra2.titulo, obra.titulo, &availTitulo);
          fclose(fsk);
          /*tipo*/
          fsk = fopen(ARQ_IS_TIPO,"r+");
          strcpy(obra2.tipo, obra.tipo);
          secTipo = insereSk(secTipo, fsk, obra2.titulo, obra.tipo, &availTipo);
          fclose(fsk);
          /*autor*/
          fsk = fopen(ARQ_IS_AUTOR,"r+");
          strcpy(obra2.autor, obra.autor);
          secAutor = insereSk(secAutor, fsk, obra2.titulo, obra.autor, &availAutor);
          fclose(fsk);
          /*ano*/
          fsk = fopen(ARQ_IS_ANO,"r+");
          strcpy(obra2.ano, obra.ano);
          secAno = insereSk(secAno, fsk, obra2.titulo, obra.ano, &availAno);
          fclose(fsk);
          /*Copia os parametros que faltam para gravar*/
          strcpy(obra2.valor, obra.valor);
          strcpy(obra2.imagem, obra.imagem);
          /*Grava a obra inserida na base de dados*/
          end = gravaObra(obra2, arq, &availBase);
          /*Atualizo o nrr na pk*/
          ind->vetor[ind->tamanho-1].nrr = end;
          /*Notificacao de insere*/
          option = geraNotificaInsere();
        
        } else {/*Obra ja existente no catalogo*/
          option = geraNotificaErroInsere();
        }
      } while (option == 1);
      /*Ordeno o indice*/
      ordenaIndice(ind);
      break;

    case 2:
      do {
        /*Imprime o menu de busca*/
        option = geraMenuBusca();

        switch (option) {
        /*Somente uma palavra por vez, por enquanto*/
        case 1:/*Busca pelo titulo*/
          printf("Digite uma palavra:\n");
          scanf("%s", temp);
          buscaSk(temp, ind, secTitulo, arq, TITULO);
          break;

        case 2:/*Busca pelo tipo*/
          printf("Digite uma palavra:\n");
          scanf("%s", temp);
          buscaSk(temp, ind, secTipo, arq, TIPO);
          break;

        case 3:/*Busca pelo autor*/
          printf("Digite uma palavra:\n");
          scanf("%s", temp);
          buscaSk(temp, ind, secAutor, arq, AUTOR);
          break;

        case 4:/*Busca por ano*/
          printf("Digite uma palavra:\n");
          scanf("%s", temp);
          buscaSk(temp, ind, secAno, arq, ANO);
          break;

        case 5:/*Busca por PK*/
          leTexto(elem->pk, sizeof(elem->pk), "Digite a PK da Obra: ");
          preencher(elem->pk, sizeof(elem->pk));
          elem->nrr = -1;

          consulta(elem, arq, ind, &consultaObra);
          break; 
        case 0:/*Menu anterior*/
          break;

        default:
          printf("\n*** Opcao invalida *** \n");
          break;
        }
        
      } while (option != 0);
      break;

    case 3:
      /*Mostra a base inteira em ordem ASCII, no html*/
      listaBase(arq, ind);

    case 4: /* Remocao */
      /******BUSCA**********/
      option = geraMenuBusca();
      switch (option) {
        /*Somente uma palavra por vez, por enquanto*/
      case 1:/*Busca pelo titulo*/
        printf("Digite uma palavra:\n");
        scanf("%s", temp);
        resultado = buscaSk(temp, ind, secTitulo, arq, TITULO);
        break;

      case 2:/*Busca pelo tipo*/
        printf("Digite uma palavra:\n");
        scanf("%s", temp);
        resultado = buscaSk(temp, ind, secTipo, arq, TIPO);
        break;

      case 3:/*Busca pelo autor*/
        printf("Digite uma palavra:\n");
        scanf("%s", temp);
        resultado = buscaSk(temp, ind, secAutor, arq, AUTOR);
        break;

      case 4:/*Busca por ano*/
        printf("Digite uma palavra:\n");
        scanf("%s", temp);
        resultado = buscaSk(temp, ind, secAno, arq, ANO);
        break;

      case 5:/*Busca por PK*/
        leTexto(elem->pk, sizeof(elem->pk), "Digite a PK da Obra: ");
        preencher(elem->pk, sizeof(elem->pk));
        elem->nrr = -1;
        resultado = consulta(elem, arq, ind, &consultaObra);
        break; 

      case 0:
        resultado = 0;
        break;

      default:
        printf("\n*** Opcao invalida *** \n");
        resultado = 0;
        break;
      }
      if(resultado) {/*Se a busca retornou algo*/
        leTexto(elem->pk, sizeof(elem->pk), "Digite a PK da obra a ser removida: ");
        preencher(elem->pk, sizeof(elem->pk));
        elem->nrr = -1;
  
        /*Faz a pesquisa da pk e mostra no html*/
        if(consulta(elem, arq, ind, &consultaObra) == 1) {
          /*Somente se consulta retornou verdadeiro*/
          ind = removePk(elem->pk, ind, arq, &availBase);
  
          /*Remove todas as Sks */
          strcpy(temp, consultaObra.titulo);
          secTitulo = removeSk(temp, secTitulo, elem->pk, TITULO, &availTitulo);
  
          strcpy(temp, consultaObra.tipo);
          secTipo = removeSk(temp, secTipo, elem->pk, TIPO, &availTipo);
  
          strcpy(temp, consultaObra.autor);
          secAutor = removeSk(temp, secAutor, elem->pk, AUTOR, &availAutor);
  
          strcpy(temp, consultaObra.ano);
          secAno = removeSk(temp, secAno, elem->pk, ANO, &availAno);
        }
      }
      break;

    case 0:/*Sair do programa*/
      /*fecha todos os arquivos abertos e libera memoria para sair do programa*/
      fechaCatalogo(arq);
      gravaIndice(ind);

      /* Grava os indices nos arquivos. */
      gravaIndSk(secTitulo, TITULO);
      gravaIndSk(secTipo, TIPO);
      gravaIndSk(secAutor, AUTOR);
      gravaIndSk(secAno, ANO);

      /* Grava as avail lists. */
      gravaAvail(availBase, ARQ_AVAIL_BASE);
      gravaAvail(availTitulo, ARQ_AVAIL_TITULO);
      gravaAvail(availTipo, ARQ_AVAIL_TIPO);
      gravaAvail(availAutor, ARQ_AVAIL_AUTOR);
      gravaAvail(availAno, ARQ_AVAIL_ANO);

      free(elem);
      break;
	
    default:
      printf("\n*** Opcao invalida *** \n");
      break;	
    }
  }
  while (ent != 0); 

  return 0;
}

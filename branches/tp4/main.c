/* MC326 - Estrutura de Arquivos
 * Catalogo de Obras de Arte
 * Grupo 24 */

/* 
 * Este programa implementa a gravacao dos dados de obras de arte
 * fornecidos pelo usuario num arquivo base.dat Tambem possibilita a
 * consulta de obras de arte atraves de uma pk, gerando o resultado em
 * html.
 */

#include <stdio.h>
#include "leitura.h"
#include "menu.h"
#include "remove.h"
#include "busca.h"

int main(int argc, char **argv){

  int ent, option, end;
  TObra obra, obra2, *rem;
  resultadosBusca *remove = NULL;
  char nome[TAM_NOME_ARQ+10];
 
  /* Ponteiro pra base de dados. */
  FILE *arq;
  
  /* Ponteiro utilizado para abrir os arquivos de indice secundario*/
  FILE *bigfile;

  /* Variavais de indice primario e sua avail list da base de dados. */
  IndPrim *ind;
  Pk *elem;
  availList availBase = FIM_DE_LISTA;

  /* Variaveis de indice secundario e respectivas avail lists. */
  IndSec *secTitulo, *secAutor, *secTipo, *secAno;
  availList availTitulo, availTipo, availAno, availAutor;
  
  /* Indice de descritores. */
  IndDesc *indDescritor;

  /*Abre a availList da base, a base e o indice primario*/
  availBase = openAvail(ARQ_AVAIL_BASE);
  arq = abreCatalogo(ARQ_BASE);
  ind = iniciaPk(arq, ind);
  
  /* Atribui valores convenientes os campos dos indices e aloca espaco pros mesmos. */
  secTitulo = inicializaSecundario(ARQ_IS_TITULO);
  secTipo   = inicializaSecundario(ARQ_IS_TIPO);
  secAutor  = inicializaSecundario(ARQ_IS_AUTOR);
  secAno    = inicializaSecundario(ARQ_IS_ANO);
   
  /* Carrega as avail lists dos arquivos. */
  availTitulo = openAvail(ARQ_AVAIL_TITULO);
  availTipo   = openAvail(ARQ_AVAIL_TIPO);
  availAutor  = openAvail(ARQ_AVAIL_AUTOR);
  availAno    = openAvail(ARQ_AVAIL_ANO);
  
  indDescritor = inicializaDescritor();
  
  /*Abre os indices secundarios*/
  constroiSecundarios(ind, arq, secTitulo, secTipo, secAutor, secAno, &availTitulo, &availTipo, &availAutor, &availAno);

  /* Abre o indice de descritores. */
  constroiIndDesc(indDescritor, ind, arq);
	
  elem = (Pk *) malloc(sizeof(Pk));

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
        obra = *(leObra(ind, &obra));
        
	/*Grava a obra inserida na base de dados*/
        end = gravaObra(obra, arq, &availBase, ind);
        
        /*Obra2 sera inserida na base. obra sera usada na insereSk*/
        strcpy(obra2.titulo, obra.titulo);
        /*Passo todas as strings para maiuscula para nao ocorrer conflito nas pesquisas*/
        maiuscula(obra2.titulo);
        maiuscula(obra.titulo);
        maiuscula(obra.tipo);
        maiuscula(obra.autor);
        maiuscula(obra.ano);
        
        /*Insercao nos indices secundarios*/
        /*titulo*/
        sprintf(nome, "%s%s", secTitulo->tipoCampo, EXTENSAO_PK);
        bigfile = fopen(nome,"r+");

        secTitulo = insereSk(secTitulo, bigfile, obra2.titulo, obra.titulo, &availTitulo);
        fclose(bigfile);
        
        /* tipo */
        sprintf(nome, "%s%s", secTipo->tipoCampo, EXTENSAO_PK);
        bigfile = fopen(nome,"r+");

        secTipo = insereSk(secTipo, bigfile, obra2.titulo, obra.tipo, &availTipo);
        fclose(bigfile);
        
        /* autor */
        sprintf(nome, "%s%s", secAutor->tipoCampo, EXTENSAO_PK);
        bigfile = fopen(nome,"r+");

        secAutor = insereSk(secAutor, bigfile, obra2.titulo, obra.autor, &availAutor);
        fclose(bigfile);
        
        /* ano */
        sprintf(nome, "%s%s", secAno->tipoCampo, EXTENSAO_PK);
        bigfile = fopen(nome,"r+");

        secAno = insereSk(secAno, bigfile, obra2.titulo, obra.ano, &availAno);
        fclose(bigfile);

        /* Notificacao de insere */
        option = geraNotificaInsere();

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
	  buscaSecudario(ind, secTitulo, arq);
          break;

        case 2:/*Busca pelo tipo*/
	  buscaSecudario(ind, secTipo, arq);
          break;

        case 3:/*Busca pelo autor*/
	  buscaSecudario(ind, secAutor, arq);
          break;

        case 4:/*Busca por ano*/
	  buscaSecudario(ind, secAno, arq);
          break;

        case 5:/*Busca por PK*/
	  buscaPrimario(ind, arq);
          break;
          
	case 6: /* Busca por conteudo. */
	  buscaDescritor(indDescritor, ind, arq);
	  break;
        		 
        case 0:/*Menu anterior*/
          break;

        default:
          printf("\n*** Opcao invalida *** \n");
          break;
        }
      } while (option != 0);
      break;

    case 3: /* Remocao */
      leTexto(elem->pk, sizeof(elem->pk), "Digite a PK da Obra: ");
      preencher(elem->pk, sizeof(elem->pk));
      elem->nrr = -1;

      /*Faz a pesquisa da pk e mostra no html*/
      remove = buscaPk(elem, arq, ind, remove);
     
      /*Se encontrou obra de arte*/
      if(remove != NULL) {
        
        leRegistro(rem, remove->obras[0].nrr, arq);
        
        /*Remove do indice primario e da base de dados*/
        ind = removePk(rem->titulo, ind, arq, &availBase);

        /*Remove todas as Sks */
        secTitulo = removeSk(rem->titulo, secTitulo, rem->titulo, &availTitulo);
        secTipo   = removeSk(rem->tipo,   secTipo,   rem->titulo, &availTipo);
        secAutor  = removeSk(rem->autor,  secAutor,  rem->titulo, &availAutor);
        secAno    = removeSk(rem->ano,    secAno,    rem->titulo, &availAno);

	/* Remove do indice de descritores. */
	indDescritor = removeDesc(elem->pk, indDescritor);
      }

      /*Libera remove, que e alocado dentro da funcao consulta*/
      liberaBusca(remove);

      break;

    case 0:/*Sair do programa*/
    
      /*fecha todos os arquivos abertos e libera memoria para sair do programa*/
      fechaCatalogo(arq);
      gravaPk(ind);

      /* Troco os indices secundarios para o de indice de valor 0, pois este deve ter
       * o tamanho do arquivo de indices secundarios atualizado, jah que eh o primeiro
       * a ser carregado no programa. */
      trocaIndSec(secTitulo, "\0");
      trocaIndSec(secTipo, "\0");
      trocaIndSec(secAutor, "\0");
      trocaIndSec(secAno, "\0");
      
      /*Grava os indice secundarios*/
      gravaIndSk(secTitulo);
      gravaIndSk(secTipo);
      gravaIndSk(secAutor);
      gravaIndSk(secAno);

      /* Grava as avail lists. */
      gravaAvail(availBase, ARQ_AVAIL_BASE);
      gravaAvail(availTitulo, ARQ_AVAIL_TITULO);
      gravaAvail(availTipo, ARQ_AVAIL_TIPO);
      gravaAvail(availAutor, ARQ_AVAIL_AUTOR);
      gravaAvail(availAno, ARQ_AVAIL_ANO);
      
      /*Libera os indices e seus vetores*/
      liberaIndices(ind, secTitulo,secTipo, secAutor, secAno, indDescritor);

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

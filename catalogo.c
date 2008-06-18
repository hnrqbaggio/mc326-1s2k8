#include "catalogo.h"


/*** Funcoes de maniluacao da base de dados. ***/
/* Funcao que abre o arquivo com as obras e amazena num ponterio. */
FILE * abreCatalogo(char *nome) {
  FILE *arq;
  arq = fopen(nome, "r+");

  if (!arq) arq = fopen(nome, "w+"); /* cria o arquivo! */

  return arq;
}


/* Fecha o catalogo apontado pelo parametro. */
void fechaCatalogo(FILE *arq) {
  fclose(arq);
}


/* Esta funcao grava os dados da obra no arquivo .dat ela recebe como
 * parametros a struct TObra e o arquivo de saida, a avail da base de dados e
 * o indice primario, que contem o tamanho da base.
 * Retorna o endereco da obra inserida ou o tamanho da base.
 */
int gravaObra(TObra obra, FILE *arq, availList *avail, Index *indice){
  
  int end, prox = 0;
  
  /*Caso e o final do arquivo*/
  if(*avail == -1) {
    fseek(arq, (indice->tamFile *(int)TAM_REG), SEEK_SET);
  	
    fprintf(arq, "%s", obra.titulo);
    fprintf(arq, "%s", obra.tipo);
    fprintf(arq, "%s", obra.autor);
    fprintf(arq, "%s", obra.ano);
    fprintf(arq, "%s", obra.valor);
    fprintf(arq, "%s", obra.imagem);
    
    /*Atualizo o nrr*/
    indice->vetor[indice->tamanho-1].nrr  =  indice->tamFile;  
    /*Atualizo tamanho da base*/
    indice->tamFile++;
    
    return indice->tamFile;
    
  } else { 
    /*Pega o valor da avail, e percorre a base com fseek para inserir na posicao.*/
    fseek(arq, (*avail*(int)TAM_REG), SEEK_SET);
  	
    /*Leio o next da avail list*/
    fscanf(arq, FORMATO_INT, &prox);
  	
    end = *avail;
    *avail = prox;
    
    /*Volta para sobrescrever o antigo next da avail*/
    fseek(arq, -TAM_NUMERO, SEEK_CUR);
    
    /*Gravacao da obra*/
    fprintf(arq, "%s", obra.titulo);
    fprintf(arq, "%s", obra.tipo);
    fprintf(arq, "%s", obra.autor);
    fprintf(arq, "%s", obra.ano);
    fprintf(arq, "%s", obra.valor);
    fprintf(arq, "%s", obra.imagem);
    
    /*Atualizo o nrr*/
    indice->vetor[indice->tamanho-1].nrr  =  end; 

    return end;
  }
}


/*** Funcoes de manipulacao do indice primario. ***/
/* funcao que carrega o indice do arquivo ou monta-o a partir da base */
Index * iniciaPk(FILE *base, Index *indice) {
	
  FILE *arq_ind; /* arquivo onde estao os indices, caso exista */
  char pkAux[TAM_TITULO+1], nome[TAM_NOME_ARQ];
  int offset, id;
  int *tam, position = 0;      
  /* apontador pro numero de elementos do vetor de
     indice. Pra facilitar a leitura do codigo. */

  /*Inicializando indice*/
  indice = (Index *) malloc(sizeof(Index));
  indice->vetor = (indexKey *) malloc(sizeof(indexKey) * VETOR_MIN);
  indice->alocado = VETOR_MIN;
  indice->tamanho = 0;
  indice->id = 0;

  /*Abro o indicePk-hash = 0*/
  sprintf(nome, "%s%d%s", ARQ_PK, indice->id, EXTENSAO_PK);
  arq_ind = fopen(nome, "r");
  
  tam = &(indice->tamanho);

  if (arq_ind != NULL) { /* existe o arquivo */
  	
	fprintf(stdout, "Carregando indice primario... ");
    /*Pego o tamanho da base*/
    fscanf(arq_ind, FORMATO_INT, &(indice->tamFile));
		
    while(fgets(indice->vetor[(*tam)].key, TAM_TITULO+1, arq_ind)) {
      (*tam)++;
      realocaIndice(indice);
      fscanf(arq_ind, "%d", &(indice->vetor[*tam-1].nrr)); /* le o nrr do registro */
    }
    fclose(arq_ind);
    
  } else { /* vai ter que gerar a partir da base */
   
   fprintf(stdout, "Construindo indice primario... ");
   
    while(fgets(pkAux, TAM_TITULO+1, base)) {
    	
      
      /*Abre o indice relativo a pkAux*/
      
      
      /***********************************/
      
                /*B+*/
      /**************************************/
      trocaIndice(indice, id);
		  
      /*Copio a pk para o indice primario*/
      strcpy(indice->vetor[(*tam)].key, pkAux);
      indice->vetor[*tam].nrr = position;
      (*tam)++;
      indice->tamFile++;
      realocaIndice(indice);
		
      /* Posiciona o cursor do arquivo no campo Titulo do proximo registro. */
      /* Usamos apenar tamanho e nao tamanho-1 pois iniciamos o nrr em zero. */
      offset = TAM_REG * (++position);
      fseek(base, offset, SEEK_SET);
    }

    /* Como o indice neste caso nao esta ordenado, precisamos ordena-lo */
    qsort(indice->vetor, *tam, sizeof(indexKey), compare);
  }

	fprintf(stdout, "OK\n");
  
  return indice;

}


/* Gravacao do indice primario, com o nome especificado pelo id contido neste */
void gravaIndicePrim(Index *indice) {
  FILE *ind;
  int i;
  char nome[TAM_NOME_ARQ+10];

  sprintf(nome, "%s%d%s", ARQ_PK, indice->id, EXTENSAO_PK);

  ind = fopen(nome, "w");
	
  /* Gravo o tamanho da base. */
  fprintf(ind, FORMATO_INT, indice->tamFile);
  	
  for (i = 0; i < indice->tamanho; i++) {
    fprintf(ind, "%s", indice->vetor[i].key);  /* grava a chave primaria   */
    fprintf(ind, FORMATO_INT, indice->vetor[i].nrr); /* grava o nrr do registro. */
  }

  fclose(ind);
}

/*Abre o arquivo correspondente ao id e carrega-o para a memoria.*/
void carregaIndicePrim(Index *indice, int id) {
  
  char nome[TAM_NOME_ARQ+10];
  int *tam = &(indice->tamanho);
  FILE *arqInd;
  
  /*Abro o indicePk-id*/
  sprintf(nome, "%s%d%s", ARQ_PK, indice->id, EXTENSAO_PK);
  arqInd = fopen(nome, "r");
  
  if (arqInd) {
    /*Pego o tamanho da base*/
    fscanf(arqInd, FORMATO_INT, &(indice->tamFile));
    
    /*Salvo no indice primario*/
    while(fgets(indice->vetor[(*tam)].key, TAM_TITULO+1, arqInd)) {
      (*tam)++;
      realocaIndice(indice);
      fscanf(arqInd, FORMATO_INT, &(indice->vetor[*tam-1].nrr)); /* le o nrr do registro */
    }
    fclose(arqInd); 
  }
}

/*Inicia um indice com valores padrao de acordo com o tipo deste.*/
Index * makeIndice(char *tipoIndice) {
  
  Index * indice = (Index *) malloc(sizeof(Index));
  
  indice->vetor = (indexKey *) malloc(sizeof(indexKey) * VETOR_MIN);
  indice->tamanho = 0;
  indice->alocado = VETOR_MIN;
  indice->tamFile = 0;
  indice->id = 0;
  indice->root = 0;
  indice->carrega = 0;/*Depende do tipoIndice*/
  indice->grava = 0;/*Depende do tipoIndice*/
  strcpy(indice->tipoCampo, tipoIndice);
  
  return indice;
}


/*  Realiza a ordenacao do indice usando qsort. */
void ordenaIndice(Index *indice) {
  qsort(indice->vetor, indice->tamanho, sizeof(indexKey), compare);
}

/* Realoca espaco para o vetor caso seja necessario. */
void realocaIndice(Index *indice) {

  if (indice->tamanho == indice->alocado) {
    indice->alocado *= 2; 
    indice->vetor = (indexKey *) realloc(indice->vetor, sizeof(indexKey) * indice->alocado);
  }
}


/*Troca o indice de acordo com a id passada como parametro.*/
void trocaIndice(Index *indice, int id) {
	
  int tamFile;
	
  /*Caso o arquivo a ser aberto e o que esta na memoria*/
  if (id == indice->id) return;
	
  tamFile = indice->tamFile;
	
  /*Gravo o indice*/
  indice->grava(indice);
  	
  /*Carrego o novo indice*/
  indice->tamanho = 0;
  indice->carrega(indice, id);
	
  /*Atualizo o tamanho da base*/
  indice->tamFile = tamFile;
}


/*Funcao que libera os mallocs/reallocs utilizados*/
void liberaIndices(Index * indPrim, Index *indTitulo,
		   Index *indTipo, Index*indAutor, Index *indAno, IndDesc *indDescritor) {
	
  /*Libero os vetores alocados em cada indice*/
  free(indPrim->vetor);
  free(indTitulo->vetor);
  free(indTipo->vetor);
  free(indAutor->vetor);
  free(indAno->vetor);
  free(indDescritor->vetor);
	
  /*Libero os indices*/
  free(indPrim);
  free(indTitulo);
  free(indTipo);
  free(indAutor);
  free(indAno);
  free(indDescritor);

}


/* Funcao de comparacao */
/* 
 * Apesar das funcoes pedidas pelo qsort e pela bsearch serem
 * diferentes, conseguimos usar esta mesma funcao para ambas.
  
 * Veja a definicao desta funcao ou o relatorio do lab pra mais
 * detalhes.
 */
int compare(const void *a, const void *b) {
  char *str1, *str2;

  str1 = (*(indexKey *)a).key;
  str2 = (*(indexKey *)b).key;
  
  return strcasecmp(str1, str2);
}


/*Le um registro da base de dados de acordo com o nrr.*/
void leRegistro(TObra *obra, int nrr, FILE *base) {
	
	fseek(base, TAM_REG * nrr, SEEK_SET);
	
	/* Le cada campo da obra. */
    fgets(obra->titulo, TAM_TITULO + 1, base);
    fgets(obra->tipo,   TAM_TIPO + 1,   base);
    fgets(obra->autor,  TAM_AUTOR + 1,  base); 
    fgets(obra->ano,    TAM_ANO + 1,    base);
    fgets(obra->valor,  TAM_VALOR + 1,  base);
    fgets(obra->imagem, TAM_IMAGEM + 1, base);
}

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
int gravaObra(TObra obra, FILE *arq, availList *avail, IndicePrim * indice ){
  
  int end, prox = 0;
  
  /*Caso e o final do arquivo*/
  if(*avail == -1) {
  	fseek(arq, (indice->tamBase *(int)TAM_REG), SEEK_SET);
  	
    fprintf(arq, "%s", obra.titulo);
    fprintf(arq, "%s", obra.tipo);
    fprintf(arq, "%s", obra.autor);
    fprintf(arq, "%s", obra.ano);
    fprintf(arq, "%s", obra.valor);
    fprintf(arq, "%s", obra.imagem);
    
    /*Atualizo o nrr*/
    indice->vetor[indice->tamanho-1].nrr  =  indice->tamBase;  
    /*Atualizo tamanho da base*/
    indice->tamBase++;
    
    return indice->tamBase;
    
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
IndicePrim * iniciaPk(FILE *base, IndicePrim *indice) {
	
  FILE *arq_ind; /* arquivo onde estao os indices, caso exista */
  char pkAux[TAM_TITULO+1], nome[TAM_NOME_ARQ];
  int offset;
  int *tam, position = 0;      
  /* apontador pro numero de elementos do vetor de
     indice. Pra facilitar a leitura do codigo. */

  /*Inicializando indice*/
  indice = (IndicePrim *) malloc(sizeof(IndicePrim));
  indice->vetor = (Pk *) malloc(sizeof(Pk) * VETOR_MIN);
  indice->alocado = VETOR_MIN;
  indice->tamanho = 0;
  indice->valorHash = 0;

  /*Abro o indicePk-hash = 0*/
  sprintf(nome, "%s%d%s", ARQ_PK, indice->valorHash, EXTENSAO_PK);
  arq_ind = fopen(nome, "r");
  
  tam = &(indice->tamanho);

  if (arq_ind != NULL) { /* existe o arquivo */

	/*Pego o tamanho da base*/
	fscanf(arq_ind, FORMATO_INT, &(indice->tamBase));
		
    while(fgets(indice->vetor[(*tam)].pk, TAM_TITULO+1, arq_ind)) {
      (*tam)++;
      indice = realocaIndPrim(indice);
      fscanf(arq_ind, "%d", &(indice->vetor[*tam-1].nrr)); /* le o nrr do registro */
    }
    fclose(arq_ind);
    
  } else { /* vai ter que gerar a partir da base */
   
    while(fgets(pkAux, TAM_TITULO+1, base)) {
    	
    	
      
		maiuscula(pkAux);
		/*Abre o indice relativo a pkAux*/
		indice = trocaIndPrim(indice, pkAux);
		  
		/*Copio a pk para o indice primario*/
		strcpy(indice->vetor[(*tam)].pk, pkAux);
		indice->vetor[*tam].nrr = position;
		(*tam)++;
		indice->tamBase++;
		indice = realocaIndPrim(indice);
		
		/* Posiciona o cursor do arquivo no campo Titulo do proximo registro. */
		/* Usamos apenar tamanho e nao tamanho-1 pois iniciamos o nrr em zero. */
		offset = TAM_REG * (++position);
		fseek(base, offset, SEEK_SET);
    }

    /* Como o indice neste caso nao esta ordenado, precisamos ordena-lo */
    qsort(indice->vetor, *tam, sizeof(Pk), compare);
  }
 
  return indice;
}

/*  Realiza a ordenacao do indice usando qsort. */
void ordenaIndice(IndicePrim *indice) {
  qsort(indice->vetor, indice->tamanho, sizeof(Pk), compare);
}

/* Gravacao do indice, ordenado por pk, no arquivo */
IndicePrim * gravaPk(IndicePrim *indice) {
  FILE *ind;
  int i;
  char nome[TAM_NOME_ARQ+10];

  sprintf(nome, "%s%d%s", ARQ_PK, indice->valorHash, EXTENSAO_PK);

  ind = fopen(nome, "w");
	
	/* Gravo o tamanho da base. */
  	fprintf(ind, FORMATO_INT, indice->tamBase);
  	
  for (i = 0; i < indice->tamanho; i++) {
    fprintf(ind, "%s", indice->vetor[i].pk);  /* grava a chave primaria   */
    fprintf(ind, FORMATO_INT, indice->vetor[i].nrr); /* grava o nrr do registro. */
  }

  fclose(ind);
  
  return indice;
}

/* Consulta de uma obra na base. 
   Chave já vem preenchido. */
TObra * consulta(Pk *chave, FILE *base, IndicePrim *indice) {
  
  Pk *temp;
  int retorno;
  TObra * reg = (TObra *) malloc(sizeof(TObra));
  
  /*Abro o indice relativo a pk a ser buscada*/
  maiuscula(chave->pk);
  indice = trocaIndPrim(indice, chave->pk);
  
  temp = (Pk *) bsearch(chave, indice->vetor, indice->tamanho, sizeof(Pk), compare);

  if (temp) { /* registro encontrado */
    FILE *saida;

    saida = fopen(ARQ_HTML, "w");
    /*Inicio do HTML*/
    headHtml(saida);
    
    retorno = 1;/*Encontrou PK. Consulta retorna 1*/
    fseek(base, TAM_REG * (temp->nrr), SEEK_SET);

    /* leitura do registro */
    fgets(reg->titulo, TAM_TITULO + 1, base);
    fgets(reg->tipo, TAM_TIPO + 1, base);
    fgets(reg->autor, TAM_AUTOR + 1, base); 
    fgets(reg->ano, TAM_ANO + 1, base);
    fgets(reg->valor, TAM_VALOR + 1, base);
    fgets(reg->imagem, TAM_IMAGEM + 1, base);

    /* passagem do resultado pra função que gera a saida em html */
    preencheHtml(saida, *reg);

    if (saida) {
      printf("\n--------------------------------------------------\n");
      printf("Registro encontrado e saida gerada com sucesso.\n");
      printf("A consulta esta disponivel no arquivo %s.\n", ARQ_HTML);
      printf("--------------------------------------------------\n");				
    } else {
      printf("\n--------------------\n");
      printf("Erro ao gerar saida!\n");
      printf("--------------------\n");
    }
		/*Fim do HTML*/
    endHtml(saida);
    
    fclose(saida);
		
  } else {/*Registro nao encontrado*/
    retorno = 0;
    printf("\n-----------------------\n");
    printf("Registro não encontrado.\n");
    printf("-----------------------\n");
    reg = NULL;
  }

  return reg;
}

/* 
 * Realiza a listagem as obras na base, e envia os resultados pra um
 * arquivo em html. 
 */
void listaBase(FILE *base, IndicePrim *indice) {
  int i, j;
  TObra reg;
  FILE *saida;

  saida = fopen(ARQ_HTML, "w");

  /*Inicio do HTML*/
  headHtml(saida);
  
  /*Percorre todos os arquivos de indice primario*/
	for(j=0; j<= H; j++) {
		/*Gravo indice primario*/
		gravaPk(indice);
	  	
	  	indice->valorHash = j;
		indice->tamanho = 0;
		abrePk(indice);
	  	
	  	/*Percorre o indice*/
	  	for (i = 0; i < indice->tamanho; i++) {
			fseek(base, TAM_REG*(indice->vetor[i].nrr), SEEK_SET);
		
		   /* leitura do registro */
		   fgets(reg.titulo, TAM_TITULO+1, base);
		   fgets(reg.tipo, TAM_TIPO+1, base);
		   fgets(reg.autor, TAM_AUTOR+1, base);
		   fgets(reg.ano, TAM_ANO+1, base);
		   fgets(reg.valor, TAM_VALOR+1, base);
		   fgets(reg.imagem, TAM_IMAGEM+1, base);
		
		   preencheHtml(saida, reg);
		}
	}
	
  /*Final do HTML*/
  endHtml(saida);
  
  printf("\n--------------------------------------------------\n");
  printf("A consulta esta disponivel no arquivo %s.\n", ARQ_HTML);
  printf("--------------------------------------------------\n")	;

  fclose(saida);
}

/* Funcao de comparacao */
/* 
 * Apesar das duncoes pedidas pelo qsort e pela bsearch serem
 * diferentes, conseguimos usar esta mesma funcao para ambas.
  
 * Veja a definicao desta funcao ou o relatorio do lab pra mais
 * detalhes.
 */
int compare(const void *a, const void *b) {
  char *str1, *str2;
  int i;

  str1 = (*(Pk *)a).pk;
  str2 = (*(Pk *)b).pk;

  /* Copia os valores dos parametros, convertendo pra maiuscula */
  for (i = 0; i <= TAM_TITULO; i++) {
    str1[i] = toupper(str1[i]);
    str2[i] = toupper(str2[i]);
  }

  str1[i] = '\0';
  str2[i] = '\0';
 
  return strcmp(str1, str2);
}

/* Realoca espaco para o vetor caso seja necessario. */
IndicePrim * realocaIndPrim(IndicePrim *ind) {

  if (ind->tamanho == ind->alocado) {
    ind->alocado *= 2;
    ind->vetor = (Pk *) realloc(ind->vetor, sizeof(Pk) * ind->alocado);
  }

  return ind;
}

/*Gera cabecalho do HTML*/
FILE * headHtml(FILE *b) {
  /* Inicio do arquivo HTML. Insere o cabecalho da tabela. */
  fprintf(b, "%s", "<html><head><title>Consulta do catalogo de obras de arte</title></head>");
  fprintf(b, "%s","<body><table border=\"1\" width=\"800\" font= 'Arial'><tr><th colspan ='3' align=\"center\"><font size='6' color='red'><b>Consulta do catalogo de obras de arte</b></th></tr>");

  return b;
}

/*Gera o final do HTML*/
FILE * endHtml(FILE *b) {
  
  fprintf(b, "%s", "</table></body></html>");
  return b;
}
/*Preenche o HTML*/
FILE * preencheHtml(FILE *b, TObra valores) {
  
  char aux[TAM_IMAGEM + 2];
  int j,i = 0;
  int ponto = 0;
  
  /*Preenchimento da tabela*/
  fprintf(b, "%s", "<tr height=\"8\"></tr>");
  fprintf(b, "%s","<tr><td nowrap width=\"200\"><b>TITULO DA OBRA</b></td><td nowrap width=\"400\">");
  fprintf(b, "%s", valores.titulo);	
  fprintf(b, "%s","</td><td nowrap width=\"200\" align=\"center\"><b>IMAGEM</b></td></tr>");
  fprintf(b, "%s","<tr><td><b>TIPO DA OBRA</b></td><td>");
  fprintf(b, "%s", valores.tipo);
  fprintf(b, "%s","</td><td rowspan=\"4\" align=\"center\">");	

  /* 
  * Este loop insere o ponto no nome do arquivo da imagem. Ele
  * verifica se ateh as tres ultimas posicoes do vetor ha somente
  * digitos numericos e insere o ponto assim que essa condicao
  * torna-se falsa. 
  */

  for(i = 0, j = 0; i < TAM_IMAGEM; i++, j++)
  {
    aux[j] = valores.imagem[i];
    if ( (valores.imagem[i] < '0' || valores.imagem[i] > '9') && ponto == 0)
    {
      aux[j] = '.';
      --i;
      ponto = 1;
    }
  }
  aux[j] = '\0';

  fprintf(b, "%s", "<a href=\"img/");
  fprintf(b, "%s", aux);
  fprintf(b, "%s", "\"><img src=\"img/");
  fprintf(b, "%s", aux);
  fprintf(b, "%s","\" \"width=\"180\" height=\"110\" alt=\"Clique na imagem para visualizar em tamanho original\"></a></td></tr>");
  fprintf(b, "%s","<tr><td><b>AUTOR</b></td><td>");
  fprintf(b, "%s", valores.autor);
  fprintf(b, "%s","</td></tr><tr><td><b>ANO</b></td><td>");
  fprintf(b, "%s", valores.ano);
  fprintf(b, "%s","</td></tr><tr><td><b>VALOR</b></td><td>");
  fprintf(b, "%s", valores.valor);
  fprintf(b, "%s", "</td><tr>");
  
  return b;
}

/*Abre o arquivo correspondente ao valor de hash ja atualizado no proprio indice*/
IndicePrim * abrePk(IndicePrim *indice) {
  
  char nome[TAM_NOME_ARQ+10];
  int *tam = &(indice->tamanho);
  FILE *arqInd;
  
  /*Abro o indicePk-hash*/
  sprintf(nome, "%s%d%s", ARQ_PK, indice->valorHash, EXTENSAO_PK);
  arqInd = fopen(nome, "r");
  
  if (arqInd) {
		/*Pego o tamanho da base*/
		fscanf(arqInd, FORMATO_INT, &(indice->tamBase));
		
	  /*Salvo no indice primario*/
	  while(fgets(indice->vetor[(*tam)].pk, TAM_TITULO+1, arqInd)) {
	    (*tam)++;
	    indice = realocaIndPrim(indice);
	    fscanf(arqInd, FORMATO_INT, &(indice->vetor[*tam-1].nrr)); /* le o nrr do registro */
	  }
	  fclose(arqInd);
  
  }
  
  return indice;
  
}

/*Troca os indices primarios*/
IndicePrim * trocaIndPrim(IndicePrim * indice, char *chave) {
	
	int hashChave, tamBase;
	
	hashChave = hashFunction(chave);
	
	/*Caso o hash e o mesmo do indice aberto*/
	if (hashChave == indice->valorHash) return indice;
	
	tamBase = indice->tamBase;
	
	/*Gravo indice primario*/
	indice = gravaPk(indice);
	
	/*Troca o o valor do Hash*/
	indice->valorHash = hashChave;
	
	/*Abro o novo indice*/
	indice->tamanho = 0;
	abrePk(indice);
	
	/*Atualizo o tamanho da base*/
	indice->tamBase = tamBase;
	
	return indice;
}

/*Transforma para maiuscula a string passada como parametro*/
void maiuscula(char *chave) {

	int i;
	
	/* Copia os valores dos parametros, convertendo pra maiuscula */
  	for (i = 0; i < strlen(chave); i++) chave[i] = toupper(chave[i]);
  	chave[i] = '\0';
  	
}

/*Funcao que libera os mallocs/reallocs utilizados*/
void liberaIndices(IndicePrim * indPrim, IndSec *indTitulo,
IndSec *indTipo, IndSec *indAutor, IndSec *indAno) {
	
	/*Libero os vetores alocados em cada indice*/
	free(indPrim->vetor);
	free(indTitulo->vetor);
	free(indTipo->vetor);
	free(indAutor->vetor);
	free(indAno->vetor);
	
	/*Libero os indices*/
	free(indPrim);
	free(indTitulo);
	free(indTipo);
	free(indAutor);
	free(indAno);
	
	return;
}

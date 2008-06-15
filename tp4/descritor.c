#include "descritor.h"

IndDesc * inicializaDescritor() {
	
	IndDesc * d = (IndDesc *) malloc (sizeof(IndDesc));
	d->vetor = (Descritor *) malloc (sizeof(Descritor) * VETOR_MIN);
	d->alocado = VETOR_MIN;
	d->tamanho = 0;
	d->valorHash = 0;
	
	return d;
}

void constroiIndDesc(IndDesc * indDesc, IndPrim *indPrim, FILE *base) {
	
	char nomeArq[TAM_NOME_ARQ+10], imagem[TAM_NOME_ARQ+10], valor;
	FILE *arqDesc;
	TObra obra;
	int i, j, cont;
	
	sprintf(nomeArq, "%s%d%s", ARQ_DESCRITOR, indDesc->valorHash, EXTENSAO_DESC);
	
	arqDesc = fopen(nomeArq, "r");
	
	if (arqDesc) {
		
		fprintf(stdout, "Carregando indices de descritores das imagens... ");
		/* Carrega os descritores a partir do arquivo. */
		indDesc = carregaDescritor(indDesc, arqDesc);
		
		/* Atualiza o valor hash dos indices. */
		indDesc->valorHash = 0;
		
		fclose(arqDesc);
		
		fprintf(stdout, "OK\n");
		
	} else {
		
		fprintf(stdout, "Criando indices de descritores das imagens.\n");
		fprintf(stdout, "Isso pode demorar um pouco, dependendo do tamanho da sua base de dados.\n");
		
		cont = 0;
		for(j=0; j<= H; j++) {
		
		/*Gravo indice primario*/
		gravaPk(indPrim);
		
	  	indPrim->valorHash = j;
	
		/*Abro o novo indice*/
		indPrim->tamanho = 0;
		abrePk(indPrim);
		
			/*Percorre os indices primarios*/
			for (i = 0; i < indPrim->tamanho; ++i) {
				
				/* Le o registro. */
				leRegistro(&obra, indPrim->vetor[i].nrr, base);
				fprintf(stdout, "Processando registro %d -> Imagem %s\n", ++cont, obra.imagem);
				
				/*Coloca tudo em maiuscula para nao 
				 * occorer discrepancia entre os dados buscados*/
				
				
				/* Calcula o descritor da imagem lida da base. */
				sprintf(imagem, "%s%s", "img/", obra.imagem);
				valor = CalculaDescritor(imagem);
				
				/* Verifica se qual indice de descritores ela pertence. */
				indDesc = trocaIndDesc(indDesc, valor);
				
				/* Faz a inserção do descritor no indice. */
				indDesc = insereDesc(indDesc, indPrim->vetor[i].pk, valor, obra.imagem, 0);
				
			}
		}
	}
}

IndDesc * carregaDescritor(IndDesc *indice, FILE *arqIndice){

  char pk[TAM_TITULO+1], imagem[TAM_IMAGEM+1];
  char valorDesc;
	
  /*Enquanto nao chega ao final do arquivo, leio pk, valor do descritor e nome da imagem*/
  while (fgets(pk, TAM_TITULO+1, arqIndice)) {
  	
    fscanf(arqIndice, "%c", &(valorDesc));
    fgets(imagem, TAM_IMAGEM+1, arqIndice);
    
    indice = insereDesc(indice, pk, valorDesc, imagem, 0); /* Insere o descritor no indice, a similaridade aqui eh irrelevante. */

  }

  return indice;
}

void gravaIndDesc(IndDesc *ind) {

  FILE *arq;
  int i = 0;
  char nomeArq[TAM_NOME_ARQ+10];
  
  sprintf(nomeArq, "%s%d%s", ARQ_DESCRITOR, ind->valorHash, EXTENSAO_DESC);
  arq = fopen(nomeArq,"w");
  
  for (i = 0; i < ind->tamanho; i++) {
    fprintf(arq, "%s", ind->vetor[i].pk);                       /* grava a pk. */
    fprintf(arq, "%c", ind->vetor[i].valorDescritor);    /* grava o valor do descritor. */
    fprintf(arq, "%s", ind->vetor[i].imagem);
  }

  fclose(arq);
  
  return;
}

IndDesc * trocaIndDesc(IndDesc *ind, char valor) {
	
  	int hashDesc;
  	char nome[TAM_NOME_ARQ+10];
  	FILE *arq;

  	hashDesc = hashDescritor(valor);

  	if (hashDesc == ind->valorHash) return ind;

  	gravaIndDesc(ind); /* Tem que trocar de indice: salva o atual e carrega outro. */
	
  	sprintf(nome, "%s%d%s", ARQ_DESCRITOR, hashDesc, EXTENSAO_DESC);
  	arq = fopen(nome, "r");

	if (arq) {
		ind->tamanho = 0;
  		ind = carregaDescritor(ind, arq);
  		fclose(arq);
  		
	} else {
		ind->tamanho = 0; /* Se o arquivo não existe, o indice eh vazio. */
	}

	/* Atualiza o valor hash do indice. */
  	ind->valorHash = hashDesc;

  	return ind;
}

IndDesc * insereDesc(IndDesc *indDesc, char *pk, char valor, char * imagem, double simil) {
	int i = indDesc->tamanho;
	
	strcpy(indDesc->vetor[i].pk, pk);
	strcpy(indDesc->vetor[i].imagem, imagem);
	indDesc->vetor[i].valorDescritor = valor;
	indDesc->vetor[i].similaridade = simil;
	
	(indDesc->tamanho++);
	
   /*Verifica se precisa ser realocado memoria*/
   indDesc = realocaIndDesc(indDesc);	
	
	return indDesc;
}

IndDesc * realocaIndDesc(IndDesc * indice) {
	
	if (indice->tamanho == indice->alocado) {
    indice->alocado = 2*(indice->alocado);
    indice->vetor = (Descritor *) realloc(indice->vetor, sizeof(Descritor) * (indice->alocado));
  }
  return indice;
  
}

IndDesc * filtraInd(IndDesc *indice, char* imgRef) {
	IndDesc *filtrado;
	char temp, referencia, imagem[TAM_NOME_ARQ];
	int i;
	double simil;

	printf("\nCriando indice de similaridade");
	
	filtrado = inicializaDescritor();
	referencia = CalculaDescritor(imgRef);

	indice = trocaIndDesc(indice, referencia);
	
	for (i = 0; i < indice->tamanho; ++i) {
	  
	  fflush(stdout);
	  printf(".");
		
		/* Calcula o XOR entre o valor do descritor atual e o de referencia */
		temp = indice->vetor[i].valorDescritor ^ referencia;
		
		/* Cado o hash do XOR seja menor que dois, insere o descritor no indice filtrado. */
		if (hashDescritor(temp) <= 2) {
			
			sprintf(imagem, "%s%s", "img/", indice->vetor[i].imagem );
			
			simil = ComputaSimilaridade(imagem, imgRef);
			insereDesc(filtrado, indice->vetor[i].pk, indice->vetor[i].valorDescritor, indice->vetor[i].imagem, simil);		
		}
	}

	printf("\n");

	/* Ja ordena o indice por similaridade com o descritor de referencia. */
	ordenaIndDesc(filtrado);
		
	return filtrado;
}

/* Ordena um indice por similardade*/
void ordenaIndDesc(IndDesc *indice) {
	qsort(indice->vetor, indice->tamanho, sizeof(Descritor), compareDesc);
}

/* Funcao de comparacao utilizada pela bsearch. */
int compareDesc(const void *a, const void *b) {

  Descritor *a2 = (Descritor *)a;
  Descritor *b2 = (Descritor *)b;
  
  	if (a2->similaridade > b2->similaridade) 
  		return 1;
  	else if (a2->similaridade == b2->similaridade)
  		return 0;
  	else /* (a2->similaridade < b2->similaridade) */
  		return -1;
  
}


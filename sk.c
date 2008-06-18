#include "sk.h"
#include "catalogo.h"

void constroiSecundarios(Index *indPrim, FILE *base, 
Index *titulo, Index*tipo, Index *autor, Index *ano, 
availList * avTitulo, availList * avTipo, availList * avAutor, availList * avAno) {
	
	int i, j, id;
	char nomeTitulo[TAM_NOME_ARQ+10], nomeTipo[TAM_NOME_ARQ+10], nomeAutor[TAM_NOME_ARQ+10], nomeAno[TAM_NOME_ARQ+10];
	char pkAux[TAM_TITULO+1];
	FILE *arqTitulo, *arqTipo, *arqAutor, *arqAno;         /* Arquivos de chaves secundarias. */
	FILE *arqPkTitulo, *arqPkTipo, *arqPkAutor, *arqPkAno; /* Arquivos de chaves primarias. */
	TObra obra;
	
	if (arqTitulo && arqTipo && arqAutor && arqAno) { /* Se existem todos os arquivos. */
		
		fprintf(stdout, "Carregando indices secundários... ");
		
		/* Carrega chaves do arquivo. */
	titulo->carrega(titulo, 0);
    tipo->carrega(tipo, 0);
    autor->carrega(autor, 0);
    ano->carrega(ano, 0);
		
		/* Atualiza o valor dos ids dos indices. */
		titulo->id = 0;
		tipo->id   = 0;
		autor->id  = 0;
		ano->id    = 0;
		
		fclose(arqTitulo);
		fclose(arqTipo);
		fclose(arqAutor);
		fclose(arqAno);
		
	} else { /* Vai precisar criar todos a partir da base de dados. */
		
		fprintf(stdout, "Gerando indices secundários... ");
		
		sprintf(nomeTitulo, "%s%s", titulo->tipoCampo, EXTENSAO_PK);
		sprintf(nomeTipo,   "%s%s", tipo->tipoCampo,   EXTENSAO_PK);
		sprintf(nomeAutor,  "%s%s", autor->tipoCampo,  EXTENSAO_PK);
		sprintf(nomeAno,    "%s%s", ano->tipoCampo,    EXTENSAO_PK);
		
		arqPkTitulo = fopen(nomeTitulo, "w");
		arqPkTipo   = fopen(nomeTipo,   "w");
		arqPkAutor  = fopen(nomeAutor,  "w");
		arqPkAno    = fopen(nomeAno,    "w");
		
		for(j=0; j<= H; j++) {
		
		/*Gravo indice primario*/
		indPrim->grava(indPrim);
		
	  indPrim->id = j;
	
		/*Abro o novo indice*/
		indPrim->tamanho = 0;
		indPrim->carrega(indPrim, id);
		
			/*Percorre os indices primarios*/
			for (i = 0; i < indPrim->tamanho; ++i) {
				
				/* Le o registro. */
				leRegistro(&obra, indPrim->vetor[i].nrr, base);				
				
				strcpy(pkAux, indPrim->vetor[i].key);
				/* Para cada indice, faz a inserção das chaves. */
				titulo = insereSk(titulo, arqPkTitulo, pkAux, obra.titulo, avTitulo);
				tipo   = insereSk(tipo,   arqPkTipo,   pkAux, obra.tipo,   avTipo);
				autor  = insereSk(autor,  arqPkAutor,  pkAux, obra.autor,  avAutor);
				ano    = insereSk(ano,    arqPkAno,    pkAux, obra.ano,    avAno);
				
			}
		}
		
		fclose(arqPkTitulo);
		fclose(arqPkTipo);
		fclose(arqPkAutor);
		fclose(arqPkAno);
		

	}
	
	fprintf(stdout, "OK\n");
	
}

Index * insereSk(Index *indSecun, FILE *fsk, char *pk, char *campo, availList *avail) {
	
  indexKey * sk = (indexKey *) malloc(sizeof(indexKey)), *sk2;
  indexKey * result;
  char * token;
  int tam, offset, temp, tamToken, id;

  /* Quebra a string em varios tokens */
  token = strtok(campo, " ,.-");

  while (token) { /* Realiza a insercao para cada novo token existente na string. */

	/* Realiza o busca na arvore */
  
  /*********************************************/
  /*        B+        */
  /*********************************/
  trocaIndice(indSecun, id);
	
   /* Inicializa os valores num elemento que usarei como chave da
      busca binaria no indice. */
   strcpy(sk->key, token);
   sk->nrr = -1;
   tamToken = strlen(token);

   result = (indexKey*) bsearch(sk, indSecun->vetor, indSecun->tamanho, sizeof(indexKey), compare);

   /* Mais um ponteiro pra regiao atual, pra poder liberar depois. */
   sk2 = sk; 

   if (result) { /* Jah existe uma ocorrencia da SK, atualiza a lista invertida. */
 
      /*free(sk); Libera o espaco que naum sera mais usado. */      
      sk = result; /*Aponta para SK encontrada*/

    } else { /* Nao ha nenhuma ocorrencia da SK, insere um novo elemento no vetor do indice */
			
      /*Verifica se nao precisa ser realocado espaco*/
      realocaIndice(indSecun);
			
      /*Insere a SK no final do vetor*/
      tam = indSecun->tamanho;
      strcpy(indSecun->vetor[tam].key, sk->key);
      indSecun->vetor[tam].nrr = sk->nrr;

      sk = &(indSecun->vetor[tam]);
			
      /*Atualiza o tamanho do vetor*/
      (indSecun->tamanho)++;
    }

    /* Insercao da chave primaria na parte em disco, usando a avail
       list. */
    
    if (*avail != -1) { /* Ira inserir num 'buraco' do arquivo. */

      temp = *avail; /* Pega o inicio da avail list. */

      offset = temp * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO; 
      fseek(fsk, offset, SEEK_SET); /* Posiciona o cursor pra gravar a nova PK */
      fprintf(fsk, "%s", pk);
      fscanf(fsk, FORMATO_INT, avail); /* Pega o proximo elemento da avail list. */

      /* Vai inserir o novo elemento na lista da SK. */
      fseek(fsk, -1*TAM_NUMERO, SEEK_CUR); 
      fprintf(fsk, FORMATO_INT, sk->nrr);
      sk->nrr = temp;

    } else {
      /* Calcula onde termina a parte do indice que fica no disco.
       * tamFile indica o numero de PKs no disco, o acrescimo do
       * tamanho numero eh devido ao cabecalho do arquivo. */
      offset = indSecun->tamFile * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;

      /* Insere a nova PK no fim da parte em disco. Com o proximo
	 elemento sendo o primeiro da lista da qual sk eh a cabeca. */
			fseek(fsk, offset, SEEK_SET); 
      fprintf(fsk, "%s", pk);
      fprintf(fsk, FORMATO_INT, sk->nrr);
      sk->nrr = indSecun->tamFile;

      (indSecun->tamFile)++;
    }
    
    sk = sk2;
    token = strtok(NULL, " ,.-"); /* Pega um novo token na string, pra fazer uma nova SK. */
  }

  /*Ordena o vetor de SKs*/
  qsort(indSecun->vetor, indSecun->tamanho, sizeof(indexKey), compare);

  free(sk);
	
  return indSecun;
}

/*Abre o arquivo correspondente ao id e carrega-o para a memoria.*/
void carregaIndiceSecun(Index *indSk, int id){

  int tamSk = 0;
  int tamKey = 0;
  FILE *arqSk;
  char nome[TAM_NOME_ARQ];
  indexKey *sk = (indexKey *) malloc(sizeof(indexKey));
  
  sprintf(nome, "%s%d%s", indSk->tipoCampo, id, EXTENSAO_SK);
  arqSk = fopen(nome, "r");
  
  /*Se o arquivo existe*/
  if(arqSk) {
    /* Le o tamanho do arquivo que contem as chaves primarias do indice secundario. */
    fscanf(arqSk, FORMATO_INT, &(indSk->tamFile));
    
    /*Enquanto nao chega ao final do arquivo, leio tamanho, key e next*/
    while (fscanf(arqSk, FORMATO_INT, &(tamKey)) != EOF){
      fgets(sk->key, 1 + tamKey, arqSk);
      fscanf(arqSk, FORMATO_INT, &(sk->nrr));
    
      /*Gravo no vetor de Sk e atualizo o tamanho deste*/
      strcpy(indSk->vetor[tamSk].key, sk->key);
      indSk->vetor[tamSk].nrr = sk->nrr;
      tamSk++;
      indSk->tamanho = tamSk;

      /*Verifica se precisa ser realocado memoria*/
      realocaIndice(indSk); 
    }
  }
  free(sk);
}

void gravaIndiceSecun(Index *sec) {

  FILE *fsk;
  int i = 0, tam;
  char nomeArq[TAM_NOME_ARQ+10];
  
  /*Tamanho da PK gravada no disco*/
  tam = TAM_TITULO + TAM_NUMERO; 
  
  sprintf(nomeArq, "%s%d%s", sec->tipoCampo, sec->id, EXTENSAO_SK);
  fsk = fopen(nomeArq,"w");
  
  /* Gravo o tamanho do arquivo de chaves primarias. */
  fprintf(fsk, FORMATO_INT, sec->tamFile);

  for (i = 0; i < sec->tamanho; i++) {
    fprintf(fsk, FORMATO_INT, (int) sizeof(sec->vetor[i].key));  /* grava o tamanho da sk */
    fprintf(fsk, "%s", sec->vetor[i].key);            /* grava a key */
    fprintf(fsk, FORMATO_INT, sec->vetor[i].nrr);    /* grava o proximo elemento na lista invertida */
  }
  
  fclose(fsk);
}

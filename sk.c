#include "sk.h"
#include "catalogo.h"

IndSec * inicializaSecundario(char *tipoCampo) {
	
	IndSec * indice = (IndSec *) malloc(sizeof(IndSec));
	
	indice->vetor = (Sk *) malloc(sizeof(Sk) * VETOR_MIN);
	indice->tamanho = 0;
	indice->alocado = VETOR_MIN;
	indice->tamDisco = 0;
	indice->valorHash = 0;
	strcpy(indice->tipoCampo, tipoCampo);
	
	return indice;
}

void constroiSecundarios(IndPrim *indPrim, FILE *base, 
IndSec *titulo, IndSec*tipo, IndSec *autor, IndSec *ano, 
availList * avTitulo, availList * avTipo, availList * avAutor, availList * avAno) {
	
	int i, j;
	char nomeTitulo[TAM_NOME_ARQ+10], nomeTipo[TAM_NOME_ARQ+10], nomeAutor[TAM_NOME_ARQ+10], nomeAno[TAM_NOME_ARQ+10];
	char pkAux[TAM_TITULO+1];
	FILE *arqTitulo, *arqTipo, *arqAutor, *arqAno;         /* Arquivos de chaves secundarias. */
	FILE *arqPkTitulo, *arqPkTipo, *arqPkAutor, *arqPkAno; /* Arquivos de chaves primarias. */
	TObra obra;
	
	/* Tenta abrir os arquivos de indice de hash iguais a zero. */
	sprintf(nomeTitulo, "%s%d%s", titulo->tipoCampo, titulo->valorHash, EXTENSAO_SK);
	sprintf(nomeTipo,   "%s%d%s", tipo->tipoCampo,   tipo->valorHash,   EXTENSAO_SK);
	sprintf(nomeAutor,  "%s%d%s", autor->tipoCampo,  autor->valorHash,  EXTENSAO_SK);
	sprintf(nomeAno,    "%s%d%s", ano->tipoCampo,    ano->valorHash,    EXTENSAO_SK);
	
	arqTitulo = fopen(nomeTitulo, "r");
	arqTipo   = fopen(nomeTipo,   "r");
	arqAutor  = fopen(nomeAutor,  "r");
	arqAno    = fopen(nomeAno,    "r");
	
	if (arqTitulo && arqTipo && arqAutor && arqAno) { /* Se existem todos os arquivos. */
		
		fprintf(stdout, "Carregando indices secundários. ");
		
		/* Carrega chaves do arquivo. */
		titulo = carregaSk(titulo, arqTitulo);
		tipo   = carregaSk(tipo,   arqTipo);
		autor  = carregaSk(autor,  arqAutor);
		ano    = carregaSk(ano,    arqAno);
		
		/* Atualiza o valor hash dos indices. */
		titulo->valorHash = 0;
		tipo->valorHash   = 0;
		autor->valorHash  = 0;
		ano->valorHash    = 0;
		
		fclose(arqTitulo);
		fclose(arqTipo);
		fclose(arqAutor);
		fclose(arqAno);
		
	} else { /* Vai precisar criar todos a partir da base de dados. */
		
		fprintf(stdout, "Gerando indices secundários. ");
		
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
		gravaPk(indPrim);
		
	  	indPrim->valorHash = j;
	
		/*Abro o novo indice*/
		indPrim->tamanho = 0;
		abrePk(indPrim);
		
			/*Percorre os indices primarios*/
			for (i = 0; i < indPrim->tamanho; ++i) {
				
				/* Le o registro. */
				leRegistro(&obra, indPrim->vetor[i].nrr, base);				
				
				/*Coloca tudo em maiuscula para nao 
				 * occorer discrepancia entre os dados buscados*/
				maiuscula(obra.titulo);
				maiuscula(obra.tipo);
				maiuscula(obra.autor);
				maiuscula(indPrim->vetor[i].pk);
				strcpy(pkAux, indPrim->vetor[i].pk);
				
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
	
	fprintf(stdout, "OK.\n");
	
}

IndSec * carregaSk(IndSec *indSk, FILE *arqSk){

  int tamSk = 0;
  Sk *sk = (Sk *) malloc(sizeof(Sk));

	/* Le o tamanho do arquivo que contem as chaves primarias do indice secundario. */
	fscanf(arqSk, FORMATO_INT, &(indSk->tamDisco));
	
  /*Enquanto nao chega ao final do arquivo, leio tamanho, key e next*/
  while (fscanf(arqSk, FORMATO_INT, &(sk->lenght)) != EOF){
    fgets(sk->key, 1 + sk->lenght, arqSk);
    fscanf(arqSk, FORMATO_INT, &(sk->next));
		
    /*Gravo no vetor de Sk e atualizo o tamanho deste*/
    strcpy(indSk->vetor[tamSk].key, sk->key);
    indSk->vetor[tamSk].next = sk->next;
    indSk->vetor[tamSk].lenght = sk->lenght;
    tamSk++;
    indSk->tamanho = tamSk;

    /*Verifica se precisa ser realocado memoria*/
    indSk = realocaIndSec(indSk);	
  }

  free(sk);
  return indSk;
}

IndSec * criaSk(IndPrim *indPrim, FILE *base, availList *avail, const int tipoCampo) {

  int i, tam;
  int offset = 0, offset_ext; /* Deslocamentos no arquivo */
  char  campo[TAM_TITULO+1];
  IndSec * secundario = (IndSec *) malloc(sizeof(IndSec)); /* O indice secundario */
  FILE *fsk;

  /* Como vamos criar o indice, a lista invertida eh vazia. */
  *avail = -1;

  /*Inicializando o vetor de SKs*/ 
  secundario->vetor = (Sk *) malloc(sizeof(Sk) * VETOR_MIN);
  secundario->alocado = VETOR_MIN;
  secundario->tamanho = 0;
  secundario->tamDisco = 0;
  
  switch (tipoCampo){
  case TITULO: /* Campo a ser lido eh o titulo. */
    tam = TAM_TITULO;
    offset_ext = 0;
    fsk = fopen(ARQ_IS_TITULO,"w");
    break;
  case TIPO: /* Campo Tipo */
    tam = TAM_TIPO;
    offset_ext = TAM_TITULO;
    fsk = fopen(ARQ_IS_TIPO,"w");
    break;
  case AUTOR: /* Campo Autor */
    tam = TAM_AUTOR;
    offset_ext = TAM_TITULO + TAM_TIPO;
    fsk = fopen(ARQ_IS_AUTOR,"w");
    break;
  case ANO: /* Campo Ano */
    tam = TAM_ANO;
    offset_ext = TAM_TITULO + TAM_TIPO + TAM_AUTOR;
    fsk = fopen(ARQ_IS_ANO,"w");
    break;
  }

  /* Criando o arquivo com os campo do cabecalho zerado. */ 
  fprintf(fsk, FORMATO_INT, 0);

  for (i = 0; i < indPrim->tamanho; i++) {
    offset = indPrim->vetor[i].nrr * TAM_REG;

    fseek(base, offset, SEEK_SET); 
    fseek(base, offset_ext, SEEK_CUR);
    fgets(campo, tam + 1, base); /* Le o campo do registro */

    /* Insere a SK relativa ao token, sendo q neste caso a avail list eh vazia. */
    secundario = insereSk(secundario, fsk, indPrim->vetor[i].pk, campo, avail);
 
  }
  fclose(fsk);
  return secundario;
}

IndSec * insereSk(IndSec *indSecun, FILE *fsk, char *pk, char *campo, availList *avail) {
	
  Sk * sk = (Sk *) malloc(sizeof(Sk)), *sk2;
  Sk * result;
  char * token;
  int tam, offset, temp;

  /* Quebra a string em varios tokens */
  token = strtok(campo, " ");

  while (token) { /* Realiza a insercao para cada novo token existente na string. */

	/* Realiza o Hash do token. */
	indSecun = trocaIndSec(indSecun, token);
	
   /* Inicializa os valores num elemento que usarei como chave da
      busca binaria no indice. */
   strcpy(sk->key, token);
   sk->next = -1;
   sk->lenght = strlen(token);

   result = (Sk*) bsearch(sk, indSecun->vetor, indSecun->tamanho, sizeof(Sk), compareSk);

   /* Mais um ponteiro pra regiao atual, pra poder liberar depois. */
   sk2 = sk; 

   if (result) { /* Jah existe uma ocorrencia da SK, atualiza a lista invertida. */
 
      /*free(sk); Libera o espaco que naum sera mais usado. */      
      sk = result; /*Aponta para SK encontrada*/

    } else { /* Nao ha nenhuma ocorrencia da SK, insere um novo elemento no vetor do indice */
			
      /*Verifica se nao precisa ser realocado espaco*/
      indSecun = realocaIndSec(indSecun);
			
      /*Insere a SK no final do vetor*/
      tam = indSecun->tamanho;
      strcpy(indSecun->vetor[tam].key, sk->key);
      indSecun->vetor[tam].next = sk->next;
      indSecun->vetor[tam].lenght = sk->lenght;

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
      fprintf(fsk, FORMATO_INT, sk->next);
      sk->next = temp;

    } else {
      /* Calcula onde termina a parte do indice que fica no disco.
       * tamDisco indica o numero de PKs no disco, o acrescimo do
       * tamanho numero eh devido ao cabecalho do arquivo. */
      offset = indSecun->tamDisco * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;

      /* Insere a nova PK no fim da parte em disco. Com o proximo
	 elemento sendo o primeiro da lista da qual sk eh a cabeca. */
			fseek(fsk, offset, SEEK_SET); 
      fprintf(fsk, "%s", pk);
      fprintf(fsk, FORMATO_INT, sk->next);
      sk->next = indSecun->tamDisco;

      (indSecun->tamDisco)++;
    }
    
    sk = sk2;
    token = strtok(NULL, " "); /* Pega um novo token na string, pra fazer uma nova SK. */
  }

  /*Ordena o vetor de SKs*/
  qsort(indSecun->vetor, indSecun->tamanho, sizeof(Sk), compareSk);

  free(sk);
	
  return indSecun;
}

void gravaIndSk(IndSec *sec) {

  FILE *fsk;
  int i = 0, tam;
  char nomeArq[TAM_NOME_ARQ+10];
  
  /*Tamanho da PK gravada no disco*/
  tam = TAM_TITULO + TAM_NUMERO; 
  
  sprintf(nomeArq, "%s%d%s", sec->tipoCampo, sec->valorHash, EXTENSAO_SK);
  fsk = fopen(nomeArq,"w");
  
  /* Gravo o tamanho do arquivo de chaves primarias. */
  fprintf(fsk, FORMATO_INT, sec->tamDisco);

  for (i = 0; i < sec->tamanho; i++) {
    fprintf(fsk, FORMATO_INT, sec->vetor[i].lenght);  /* grava o tamanho da sk */
    fprintf(fsk, "%s", sec->vetor[i].key);            /* grava a key */
    fprintf(fsk, FORMATO_INT, sec->vetor[i].next);    /* grava o proximo elemento na lista invertida */
  }

  fclose(fsk);
  
  return;
}


IndSec * trocaIndSec(IndSec *indSecun, char *chave) {
	
  	int hashChave, tamanho = indSecun->tamDisco;
  	char nome[TAM_NOME_ARQ+10];
  	FILE *ind;

  	hashChave = hashFunction(chave);

  	if (hashChave == indSecun->valorHash) return indSecun;

  	gravaIndSk(indSecun); /* Tem que trocar de indice: salva o atual e carrega outro. */
	
  	sprintf(nome, "%s%d%s", indSecun->tipoCampo, hashChave, EXTENSAO_SK);
  	ind = fopen(nome, "r");

	if (ind) {
  		indSecun = carregaSk(indSecun, ind);
	
  		fclose(ind);
  		
	} else {
		indSecun->tamanho = 0; /* Se o arquivo não existe, o indice eh vazio. */
	}

	/* Atualiza o valor hash do indice. */
  	indSecun->valorHash = hashChave;
  	indSecun->tamDisco = tamanho;

  	return indSecun;
}




/* Realoca espaco para o vetor caso seja necessario. */
IndSec * realocaIndSec(IndSec *sec) {

  if (sec->tamanho == sec->alocado) {
    sec->alocado = 2*(sec->alocado);
    sec->vetor = (Sk *) realloc(sec->vetor, sizeof(Sk) * sec->alocado);
  }
  return sec;

}

/* Funcao de comparacao utilizada pela bsearch. */
int compareSk(const void *a, const void *b) {

  Sk *a2 = (Sk *)a;
  Sk *b2 = (Sk *)b;
  int i, x, y;

  /* Calculo do tamanho das strings. */
  x = a2->lenght;
  y = b2->lenght;

  /* Copia os valores dos parametros, convertendo pra maiuscula */
  for (i = 0; i <= x; i++) a2->key[i] = toupper(a2->key[i]);
  a2->key[i] = '\0';

  for (i = 0; i <= y; i++) b2->key[i] = toupper(b2->key[i]);
  b2->key[i] = '\0';
 
  return strcmp(a2->key, b2->key);

}


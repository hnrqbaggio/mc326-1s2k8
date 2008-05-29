#include "catalogo.h"
#include "leitura.h"
#include "sk.h"

/* Constroi um indice secundario.  Se existe o arquivo de indice, ele 
carrega parte de estrutura pra memoria e deixa a outra no disco.
 Caso nao exista o indice, ele ira constri-lo a partir da base de
 dados.*/

IndSec * geraSk(IndicePrim *indPrim, FILE *base, availList *avail, const int tipoCampo){

  IndSec *indSk;
  FILE *fsk;

  switch (tipoCampo){
  case TITULO: /* Campo a ser lido eh o titulo. */
    fsk = fopen(ARQ_IS_TITULO,"r");
    if (fsk == NULL){
      indSk = criaSk(indPrim, base, avail, tipoCampo);
    } else {
      indSk = carregaSk(fsk, avail);
    }
    break;
  case TIPO: /* Campo Tipo */
    fsk = fopen(ARQ_IS_TIPO,"r");
    if (fsk == NULL){
      indSk = criaSk(indPrim, base, avail, tipoCampo);
    } else {
      indSk = carregaSk(fsk, avail);
    }
    break;
  case AUTOR: /* Campo Autor */
    fsk = fopen(ARQ_IS_AUTOR,"r");
    if (fsk == NULL){
      indSk =  criaSk(indPrim, base, avail, tipoCampo);
    } else {
      indSk = carregaSk(fsk, avail);
    }
    break;
  case ANO: /* Campo Ano */
    fsk = fopen(ARQ_IS_ANO,"r");
    if (fsk == NULL){
      indSk = criaSk(indPrim, base, avail, tipoCampo);
    } else {
      indSk = carregaSk(fsk, avail);
    }
    break;
  }

  if (fsk) fclose(fsk);
  return indSk;
}


/* Funcao que,caso exista o arquivo de SKs, carrega a parte que deve
  ficar na RAM e no no cabeca da AVAIL LIST da parte que fica no
  disco */
IndSec * carregaSk(FILE *arqSk){
  /*Declaracao de variaveis*/
  IndSec *indSk;
  int tamSk = 0;
  Sk *sk = (Sk *) malloc(sizeof(Sk));

  /*Inicializando o vetor de SKs*/
  indSk = (IndSec *)malloc(sizeof(IndSec)); 
  indSk->vetor = (Sk *) malloc(sizeof(Sk) * VETOR_MIN);
  indSk->alocado = VETOR_MIN;
  indSk->tamanho = 0;

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

IndSec * criaSk(IndicePrim *indPrim, FILE *base, availList *avail, const int tipoCampo) {

  int i, tam;
  int offset = 0, offset_ext; /* Deslocamentos no arquivo */
  char  campo[TAM_TITULO+1];
  IndSec * secundario = (IndSec *)malloc(sizeof(IndSec)); /* O indice secundario */
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

    /* Inicializo os valores num elemento que usarei como chave da
       busca binaria no indice. */
    strcpy(sk->key, token);
    sk->next = -1;
    sk->lenght = strlen(token);

    result = (Sk*) bsearch(sk, indSecun->vetor, indSecun->tamanho, sizeof(Sk), compareSk);

    /* Insercao da SK no indice na RAM */
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

void gravaIndSk(IndSec *sec, const int tipoCampo) {

  FILE *fsk;
  int i = 0, tam;
  /*Tamanho da PK gravada no disco*/
  tam = TAM_TITULO + TAM_NUMERO;
	
  /*Define o tipo de campo e abre o arquivo correspondente*/
  switch (tipoCampo){
  case TITULO: /* Campo a ser lido eh o titulo. */
    fsk = fopen(ARQ_IS_TITULO,"r+");
    break;
  case TIPO: /* Campo Tipo */
    fsk = fopen(ARQ_IS_TIPO,"r+");
    break;
  case AUTOR: /* Campo Autor */
    fsk = fopen(ARQ_IS_AUTOR,"r+");
    break;
  case ANO: /* Campo Ano */
    fsk = fopen(ARQ_IS_ANO,"r+");
    break;
  }

  /*Gravo o tamanho da parte que esta no disco*/
  fprintf(fsk, FORMATO_INT, sec->tamDisco);

  /*Pulo para o fim do arquivo e gravo as SKs*/
  fseek(fsk, sec->tamDisco * tam, SEEK_CUR);
  for (i = 0; i < sec->tamanho; i++) {
    fprintf(fsk, FORMATO_INT, sec->vetor[i].lenght);  /* grava o tamanho da sk */
    fprintf(fsk, "%s", sec->vetor[i].key); /* grava a key */
    fprintf(fsk, FORMATO_INT, sec->vetor[i].next);/* grava o proximo elemento na lista invertida */
  }

  /* Libera memoria da estrutura E dos seus campos. */
  free(sec->vetor);
  free(sec);
  fclose(fsk);
	
  return;
}


IndSec * carregaIndice(IndSec *indSecun, char *chave, int *atual, const int tipoCampo) {
  int valorHash; 
  char *nome, campo;
  FILE *ind;

  valorHash = hashFunction(chave);

  if (valorHash == atual) return indSecun;

  gravaIndSk(indSecun, tipoCampo, atual); /* O par tipoCampo e atual
					     determinam unicamente o
					     arquivo onde salvar. */
  free(IndSecun);

  switch (tipoIndice) {
  case TITULO: /* Campo a ser lido eh o titulo. */
    strcpy(campo, ARQ_IS_TITULO); 
    break;
  case TIPO: /* Campo Tipo */
    strcpy(campo, ARQ_IS_TIPO);
    break;
  case AUTOR: /* Campo Autor */
     strcpy(campo, ARQ_IS_AUTOR)
    break;
  case ANO: /* Campo Ano */
    strcpy(campo, ARQ_IS_ANO);
    break;
  }

  sprintf(nome, "%d%s", valorHash, campo);
  *atual = valorHash;
  ind = fopen(nome, "r+");

  IndSecun = NULL;
  indSecun = carregaSk(ind);

  return indSecun;
}




/* Realoca espaco para o vetor caso seja necessario. */
IndSec * realocaIndSec(IndSec *sec) {

  if (sec->tamanho == sec->alocado) {
    sec->alocado *= 2;
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


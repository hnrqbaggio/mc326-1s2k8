#include "catalogo.h"
#include "leitura.h"
#include "sk.h"

/* Constroi um indice secundario.  Se existe o arquivo de indice, ele 
carrega parte de estrutura pra memoria e deixa a outra no disco.
 Caso nao exista o indice, ele ira constri-lo a partir da base de
 dados.*/

/*Tetando fazer funfa pra comecar a debugar*/
IndSec * geraSk(TIndice *indPrim, FILE *base, const int tipoCampo){

	/*IndSec *indSk;*/
	FILE *fsk;

	switch (tipoCampo){
  case 0: /* Campo a ser lido eh o titulo. */
    fsk = fopen("titulo.sk","r");
		if (fsk){
			return  criaSk(indPrim, base, tipoCampo);
		} else {
			return carregaSk(fsk);
		}
    break;
  case 1: /* Campo Tipo */
    fsk = fopen("tipo.sk","r");
		if (fsk == NULL){
			return  criaSk(indPrim, base, tipoCampo);
		} else {
			return carregaSk(fsk);
		}
    break;
  case 2: /* Campo Autor */
    fsk = fopen("autor.sk","r");
		if (fsk == NULL){
			return  criaSk(indPrim, base, tipoCampo);
		} else {
			return carregaSk(fsk);
		}
    break;
  case 3: /* Campo Ano */
    fsk = fopen("ano.sk","r");
		if (fsk == NULL){
			return  criaSk(indPrim, base, tipoCampo);
		} else {
			return carregaSk(fsk);
		}
    break;
  }
	/*return fsk;*/
}


/* Funcao que,caso exista o arquivo de SKs, carrega a parte que deve
  ficar na RAM e no no cabeca da AVAIL LIST da parte que fica no
  disco */
IndSec * carregaSk(FILE *arqSk){
  /*Declaracao de variaveis*/
  IndSec *sk;
  int tamSk = 0;
	
  /*Inicializando o vetor de SKs*/
  sk = (IndSec *)malloc(sizeof(IndSec)); 
  sk->vetor = (Sk *) malloc(sizeof(Sk) * VETOR_MIN);
  sk->alocado = VETOR_MIN;
  sk->tamanho = 0;
	
  /*Leitura do tamanho do SK que fica no disco*/
  fscanf(arqSk, "%d", sk->tamDisco);

  /*Posiciona o ponteiro do arquivo para o inicio da parte que deve ficar na RAM*/
  fseek(arqSk, sk->tamDisco, SEEK_SET);

  /*Enquanto nao chega ao final do arquivo, */
  while (fscanf(arqSk, "%d", sk->vetor[tamSk].lenght) ){
    fgets(sk->vetor[tamSk].key, sk->vetor[tamSk].lenght, arqSk);
    tamSk++;
    fscanf(arqSk, "%d", sk->vetor[tamSk].next);
    sk = realocaIndSec(sk);
  }
  return sk;
}

IndSec * criaSk(TIndice *indPrim, FILE *base, const int tipoCampo) {
  int i, tam;
  int offset = 0, offset_ext; /* Deslocamentos no arquivo */
  char * campo;
  IndSec * secundario = NULL; /* O indice secundario */
	FILE *fsk = NULL;

  switch (tipoCampo){
  case 0: /* Campo a ser lido eh o titulo. */
    tam = TAM_TITULO;
    offset_ext = 0;
    fsk = fopen("titulo.sk","w");
    break;
  case 1: /* Campo Tipo */
    tam = TAM_TIPO;
    offset_ext = TAM_TITULO;
    fsk = fopen("tipo.sk","w");
    break;
  case 2: /* Campo Autor */
    tam = TAM_AUTOR;
    offset_ext = TAM_TITULO + TAM_TIPO;
    fsk = fopen("autor.sk","w");
    break;
  case 3: /* Campo Ano */
    tam = TAM_ANO;
    offset_ext = TAM_TITULO + TAM_TIPO + TAM_AUTOR;
    fsk = fopen("ano.sk","w");
    break;
  }
  
  fprintf(fsk, "%d%d", 0, 0); /* Criando o arquivo com os campos do cabecalho zerados. */
  fclose(fsk);

  for (i = 0; i < indPrim->tamanho; i++) {
    offset = indPrim->vetor[i].nrr * TAM_REG;

    fseek(base, offset, SEEK_SET); 
    fseek(base, offset_ext, SEEK_CUR);
    fgets(campo, tam, base); /* Le o campo do registro */

    /* Insere a SK relativa ao token, sendo q neste caso a avail list eh vazia. */
    secundario = insereSk(secundario, fsk, indPrim->vetor[i].pk, campo, FIM_DE_LISTA);
 
  }

  return secundario;
}

IndSec * insereSk(IndSec *indSecun, FILE *fsk, char *pk, char *campo, int *avail) {
  Sk * temp = (Sk *) malloc(sizeof(Sk));
  Sk * result;
  char * token;
  int tam, offset;

  /* Quebra a string em varios tokens */
  token = strtok(campo, " ");

  while (token) { /* Realiza a insercao para cada novo token existente na string. */

    /* Inicializo os valores num elemento que usarei como chave da
       busca binaria no indice. */
    strcpy(temp->key, token);
    temp->next = -1;
    temp->lenght = strlen(token) + 2*sizeof(int);

    result = (Sk*) bsearch(temp, indSecun->vetor, indSecun->tamanho, sizeof(Sk), compare);

    if (result) { /* Jah existe uma ocorrencia da SK, atualiza a lista invertida. */
      
      temp = result; /*Aponta para SK encontrada*/

    } else { /* Nao ha nenhuma ocorrencia da SK, insere um novo elemento no vetor do indice */
			
      /*Verifica se nao precisa ser realocado espaco*/
      indSecun = realocaIndSec(indSecun);
			
      /*Insere a SK no final do vetor*/
      tam = indSecun->tamanho;
      strcpy(indSecun->vetor[tam].key, temp->key);
      indSecun->vetor[tam].next = temp->next;
      indSecun->vetor[tam].lenght = temp->lenght;
			
      /*Atualiza o tamanho do vetor*/
      (indSecun->tamanho)++;
    }

    /* Insercao da chave primaria na parte em disco, usando a avail
       list. */
    
    if (*avail != -1) { /* Ira inserir num 'buraco' do arquivo. */
      offset = *avail + 2*sizeof(int);

      temp->next = *avail;
      fseek(fsk, offset, SEEK_SET);
      fprintf(fsk, "%s", pk);
      fscanf(fsk, "%d", avail); /* Pega o proximo elemento da lista e atualiza a cabeca. */

    } else {
      /* Calcula onde termina a parte do indice que fica no disco.
       * tamDisco indica o numero de PKs no disco, o acrescimo do
       * tamanho do int * 2 eh devido ao cabecalho do arquivo. */
      offset = indSecun->tamDisco * TAM_TITULO + 2 * sizeof(int);
      (indSecun->tamDisco)++;

      fseek(fsk, offset, SEEK_SET);
      fprintf(fsk, "%s", pk);
      fprintf(fsk, "%d", temp->next);
      temp->next = indSecun->tamDisco;
    }
    

    token = strtok(NULL, " "); /* Pega um novo token na string, pra fazer uma nova SK. */
  }
  /*Ordena o vetor de SKs*/
  qsort(indSecun->vetor, indSecun->tamanho, sizeof(Sk), compare);
	
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



#include "catalogo.h"
#include "leitura.h"
#include "sk.h"

/* Constroi um indice secundario.  Se existe o arquivo de indice, ele 
carrega parte de estrutura pra memoria e deixa a outra no disco.
 Caso nao exista o indice, ele ira constri-lo a partir da base de
 dados.*/

/*Tetando fazer funfa pra comecar a debugar*/
IndSec * geraSk(TIndice *indPrim, FILE *base, availList *avail, const int tipoCampo){

  /*IndSec *indSk;*/
  FILE *fsk;

  switch (tipoCampo){
  case 0: /* Campo a ser lido eh o titulo. */
    fsk = fopen("titulo.si","r");
    if (fsk == NULL){
      return criaSk(indPrim, base, avail, tipoCampo);
    } else {
      return carregaSk(fsk, avail);
    }
    break;
  case 1: /* Campo Tipo */
    fsk = fopen("tipo.si","r");
    if (fsk == NULL){
      return criaSk(indPrim, base, avail, tipoCampo);
    } else {
      return carregaSk(fsk, avail);
    }
    break;
  case 2: /* Campo Autor */
    fsk = fopen("autor.si","r");
    if (fsk == NULL){
      return criaSk(indPrim, base, avail, tipoCampo);
    } else {
      return carregaSk(fsk, avail);
    }
    break;
  case 3: /* Campo Ano */
    fsk = fopen("ano.si","r");
    if (fsk == NULL){
      return criaSk(indPrim, base, avail, tipoCampo);
    } else {
      return carregaSk(fsk, avail);
    }
    break;
  }
  return NULL;
}


/* Funcao que,caso exista o arquivo de SKs, carrega a parte que deve
  ficar na RAM e no no cabeca da AVAIL LIST da parte que fica no
  disco */
IndSec * carregaSk(FILE *arqSk, availList *avail){
  /*Declaracao de variaveis*/
  IndSec *sk;
  int tamSk = 0;
	
  /*Inicializando o vetor de SKs*/
  sk = (IndSec *)malloc(sizeof(IndSec)); 
  sk->vetor = (Sk *) malloc(sizeof(Sk) * VETOR_MIN);
  sk->alocado = VETOR_MIN;
  sk->tamanho = 0;
	
  /*Leitura do tamanho do SK que fica no disco*/
  fscanf(arqSk, "%d", &(sk->tamDisco));

  /*Posiciona o ponteiro do arquivo para o inicio da parte que deve ficar na RAM*/
  fseek(arqSk, sk->tamDisco, SEEK_SET);

  /*Enquanto nao chega ao final do arquivo, */
  while (fscanf(arqSk, "%d", &(sk->vetor[tamSk].lenght)) ){
    fgets(sk->vetor[tamSk].key, sk->vetor[tamSk].lenght, arqSk);
    tamSk++;
    fscanf(arqSk, "%d", &(sk->vetor[tamSk].next));
    sk = realocaIndSec(sk);
  }
  return sk;
}

IndSec * criaSk(TIndice *indPrim, FILE *base, availList *avail, const int tipoCampo) {
  int i, tam;
  int offset = 0, offset_ext; /* Deslocamentos no arquivo */
  char  campo[201];
  IndSec * secundario = (IndSec *)malloc(sizeof(IndSec)); /* O indice secundario */
  FILE *fsk;

  /* Como vamos criar o indice, a lista invertida eh vazia. */
  *avail = -1;

  /*Inicializando o vetor de SKs*/ 
  secundario->vetor = (Sk *) malloc(sizeof(Sk) * VETOR_MIN);
  secundario->alocado = VETOR_MIN;
  secundario->tamanho = 0;

  switch (tipoCampo){
  case 0: /* Campo a ser lido eh o titulo. */
    tam = TAM_TITULO;
    offset_ext = 0;
    fsk = fopen("titulo.si","w");
    break;
  case 1: /* Campo Tipo */
    tam = TAM_TIPO;
    offset_ext = TAM_TITULO;
    fsk = fopen("tipo.si","w");
    break;
  case 2: /* Campo Autor */
    tam = TAM_AUTOR;
    offset_ext = TAM_TITULO + TAM_TIPO;
    fsk = fopen("autor.si","w");
    break;
  case 3: /* Campo Ano */
    tam = TAM_ANO;
    offset_ext = TAM_TITULO + TAM_TIPO + TAM_AUTOR;
    fsk = fopen("ano.si","w");
    break;
  }
  
  fprintf(fsk, "%d%d", -1, 0); /* Criando o arquivo com os campos do cabecalho zerados. */


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
    sk->lenght = strlen(token) + 2*sizeof(int);

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

      offset = temp * (TAM_TITULO + sizeof(int)) + 2 * sizeof(int); 
      fseek(fsk, offset, SEEK_SET); /* Posiciona o cursor pra gravar a nova PK */
      fprintf(fsk, "%s", pk);
      fscanf(fsk, "%d", avail); /* Pega o proximo elemento da avail list. */

      /* Vai inserir o novo elemento na lista da SK. */
      fseek(fsk, -1*sizeof(int), SEEK_CUR); 
      fprintf(fsk, "%d", sk->next);
      sk->next = temp;

    } else {
      /* Calcula onde termina a parte do indice que fica no disco.
       * tamDisco indica o numero de PKs no disco, o acrescimo do
       * tamanho do int * 2 eh devido ao cabecalho do arquivo. */
      offset = indSecun->tamDisco * (TAM_TITULO + sizeof(int)) + 2 * sizeof(int);

      /* Insere a nova PK no fim da parte em disco. Com o proximo
	 elemento sendo o primeiro da lista da qual sk eh a cabeca. */
      fprintf(fsk, "%s", pk);
      fprintf(fsk, "%d", sk->next);
      sk->next = indSecun->tamDisco;

      (indSecun->tamDisco)++;
    }
    
    sk = sk2;
    token = strtok(NULL, " "); /* Pega um novo token na string, pra fazer uma nova SK. */
  }

  /*Ordena o vetor de SKs*/
  qsort(indSecun->vetor, indSecun->tamanho, sizeof(Sk), compareSk);
	
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
  char *str1, *str2;
  int i, x, y;

  /* Calculo do tamanho das strings. */
  x = (*(Sk *)a).lenght - 2 * sizeof(int);
  y = (*(Sk *)b).lenght - 2 * sizeof(int);

  str1 = (*(Sk *)a).key;
  str2 = (*(Sk *)b).key;

  /* Copia os valores dos parametros, convertendo pra maiuscula */
  for (i = 0; i <= x; i++) str1[i] = toupper(str1[i]);
  str1[i] = '\0';

  for (i = 0; i <= y; i++) str2[i] = toupper(str2[i]);
  str2[i] = '\0';
 
  return strcmp(str1, str2);

}



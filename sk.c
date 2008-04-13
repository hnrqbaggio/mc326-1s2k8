#include <sk.h>


IndSec * geraSk(TIndice *, FILE *, const int).

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
	
  /*Leitura do no cabeca e do tamanho do SK que fica no disco*/
  fscanf(arqSk, "%d", sk->head);
  fscanf(arqSk, "%d", sk->tamDisco);

  /*Posiciona o ponteiro do arquivo para o inicio da parte que deve ficar na RAM*/
  fseek(arqSk, sek->tamDisco, SEEK_SET);

  /*Enquanto nao chega ao final do arquivo, */
  while (fscanf(arqSk, "%d", sk->vetor[tamSk].lenght) ){
    fgets(sk->vetor[tamSk].key, sk->vetor[tamSk].lenght, arqSk);
    tamSK++;
    fscanf(arqSk, "%d", sk->vetor[tamSk].next);
    sk = realocaIndSec(sk);
  }
  return sk;
}

IndSec * criaSk(TIndice *indPrim, FILE *base, const int tipoCampo) {
  int i;
  int offset = 0, offset_ext; /* Deslocamentos no arquivo */
  char * campo;
  IndSec * secundario = NULL; /* O indice secundario */

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
  Sk * temp2 = (Sk *) malloc(sizeof(Sk));
  Sk * result;
  char * token;
  int prox, anterior, tam;

  /* Quebra a string em varios tokens */
  token = strtok(campo, " ");
  while (token) { /* Realiza a insercao para cada novo token existente na string. */

    /* Inicializo os valores num elemento que usarei como chave da
       busca binaria no indice. */
    strcpy(temp->key, token);
    temp->next = -1;
    temp->lenght = strlen(token)+2*sizeof(int);

    strcpy(temp2->key, pk);
    temp2->next = -1;
    temp2->lenght = strlen(pk)+2*sizeof(int);

    result = (Sk*) bsearch(temp, indSecun->vetor, indSecun->tamanho, sizeof(Sk), compare);

    if (result) { /* Jah existe uma ocorrencia da SK, atualiza a lista invertida. */
      
    } else { /* Nao ha nenhuma ocorrencia da SK, insere um novo elemento no vetor do indice */
    
    }

    if (*avail == FIM_DE_LISTA) {
      temp2->next = temp->next;

      fseek(fsk, tamDisco, SEEK_SET);
      fprintf(base, "%d", temp2->lenght);
      fprintf(base, "%d", temp2->next);
      fprintf(base, "%s", temp2->key);

      temp->next = indSecun->tamDisco; /* Atualiza a cabeca da lista invertida */
      indSecun->tamDisco += temp2->lenght; /* Atualiza a proxima posicao livre do disco */

    } else {
      
      fseek(fsk, *avail, SEEK_SET);
      fscanf(fsk, "%d", tam);
      prox = ant = FIM_DE_LISTA;

      while (tam < temp2->lenght) {
	ant = prox;
	fscanf(fsk, "%d", prox);
	fseek(fsk, prox, SEEK_SET);
	fscanf(fsk, "%d", tam);
      }

      fscanf(fsk, "%d", prox);

      if (ant != FIM_DE_LISTA) {
	fseek(fsk, ant+sizeof(int), SEEK_SET);
	fprintf(fsk, "%d", prox);

      } else {
	*avail = prox;
      }

      fseek(fsk, -1*sizeof(int), SEEK_CUR);
      fprintf(base, "%d", temp2->next);
      fprintf(base, "%s", temp2->key);

      if (tam > temp2->lengh) fprintf(fsk, "%c", '\0'); /* Pra nao misturar com dados antigos */

    }
    token = strtok(NULL, " ");
  }
}


/* Realoca espaco para o vetor caso seja necessario. */
IndSec * realocaIndSec(IndSec *sec) {

  if (sec->tamanho == sec->alocado) {
    sec->alocado *= 2;
    sec->vetor = (Sk *) realloc(sec->vetor, sizeof(Sk) * sec->alocado);
  }

  return sec;
}



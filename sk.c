#include <sk.h>


IndSec * geraSk(TIndice *, FILE *, const int).

/*Funcao que,caso exista o arquivo de SKs, carrega a parte que deve ficar na RAM e no no cabeca da AVAIL LIST da parte que fica no disco*/
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

    /* Agora quebra o campo em varias substrings - os tokens*/ 
    token = strtok(campo, " ");
    while (token) {
      /* Insere a SK relativa ao token, sendo q neste caso a avail list eh vazia. */
      secundario = insereSk(secundario, token, FIM_DE_LISTA, tipoCampo);
      token = strtok(NULL, " ");
    }
  }

  return secundario;
}

IndSec * insereSk(IndSec *indSecun, char *token, int avail,  const int tipoCampo) {
  Sk * temp, *result;
  FILE *fsk; /* Arquivo de indice secundario */

  strcpy(temp->key, token);
  temp->next = -1;
  temp->lenght = strlen(token);

  result = (Sk*) bsearch(temp, indSecun->vetor, indSecun->tamanho, sizeof(Sk), compare);

  if (result) { /* Jah existe uma ocorrencia da SK, atualiza a lista invertida. */
    
  } else { /* Nao ha nenhuma ocorrencia da SK, insere um novo elemento no vetor do indice */
    
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

#include "catalogo.h"
#include "leitura.h"

/*Funcao de leitura da obra de arte a ser inserida*/
int leObra (TIndice *ind, TObra *obra) {
	
  ElementoIndice *chave = (ElementoIndice *) malloc(sizeof(ElementoIndice));
  ElementoIndice *result = NULL;
	
  printf("\nEntre com os dados de uma obra.\n");

  /*Leitura dos dados da obra*/
  /*Le os campos quem contem texto*/
  leTexto(obra->titulo, sizeof(obra->titulo), "Nome da Obra (ate 200 caracteres): ");
  preencher(obra->titulo, sizeof(obra->titulo));

  /*Verifica se ja existe a obra no catalogo*/
  strcpy(chave->pk, obra->titulo);
  chave->nrr = ind->tamanho; /* eh bom inicializar, entao jah coloca o nrr certo */
  result = (ElementoIndice *) bsearch(chave, ind->vetor, ind->tamanho, sizeof(ElementoIndice), compare);
	
  if (result) { /* Registro ja existente. Retorna falso */
    return 0;

  } else { 
    leTexto(obra->tipo, sizeof(obra->tipo), "Tipo (ate 100 caracteres): ");
    leTexto(obra->autor, sizeof(obra->autor), "Autor (ate 125 caracteres): ");

    /*Campos com numeros*/
    leNumeros(obra->ano, sizeof(obra->ano), "Ano de Publicacao (ate 4 digitos): ");
    leNumeros(obra->valor, sizeof(obra->valor), "Valor (ate 12 digitos): ");

    /*Campo Imagem*/
    leImagem(obra->imagem, sizeof(obra->imagem),"Identificador da Imagem (6 digitos + extensao): " );

    /*preenchendo com espacos e movendo o '\0' pro fim de cada campo*/ 
    preencher(obra->tipo, sizeof(obra->tipo));
    preencher(obra->autor, sizeof(obra->autor));
    preencher(obra->ano, sizeof(obra->ano));
    preencher(obra->valor, sizeof(obra->valor));
		
    /* verifica se o indice precisa de mais espaco. */
    (ind->tamanho)++;
    ind = realocaIndice(ind);

    /* atualizacao do indice */	
    ind->vetor[ind->tamanho-1] = *chave;
  }
	/*Insercao realizada com exito na pk*/
  free(chave);
  return 1;
}

/* Funcao para a leitura dos campos de texto */
void leTexto(char *campo, int tamanho, char *msg){
  int valido = 1;
  int i;
  char c;

  do {
    LimpaBuffer();
    if (!valido) MensagemErro();

    valido = 1;
    printf(msg);
    c = getchar();/*pega a primeira string*/

    while (c ==' ') c=getchar();/*analisa se tem espacos desnecessarios*/
    campo[0]=c;/*caso nao tenha espacos no comeco ele ja coloca no vetor*/
    
    /*le string por sting*/
    for (i=1;i<tamanho;i++){
      c=getchar();

      if (c=='\n') break;/*para quando acha um \n*/
      campo[i]=c; /*escreve no vetor*/

      /*se ele le dois espaços juntos ele só faz valer um*/
      if ((campo[i-1]==' ') && (campo[i]==' ')) i--;

    }

    campo[i]='\0';/*coloca um \0 no final do vetor*/

  } while (!valido);
    LimpaBuffer();
}

/* Funcao para a leitura dos campos que recebem numeros */
void leNumeros(char *campo, int tamanho, char *msg) {
  int i;
  int valido = 1;
  char temp[TAM_VALOR+5];

  do {
    LimpaBuffer();
    if (!valido) MensagemErro();

    valido = 1;
    printf(msg);
    fgets(temp, sizeof(temp), stdin);

    temp[strlen(temp)-1] = '\0'; /* fgets pega '\n'. Troco por '\0'. */
	
    /* ve se a entrada eh maior do que o permitido pelo campo. */
    if (strlen(temp) >= tamanho) valido = 0;

    /* verifica se esta vazia ou se comeca com espaco. */
    if (temp[0] == '\0' || temp[0] == ' ' ) valido = 0;
    
    /* verifica se os caracteres digitados são numeros. */
    for (i = 0; i < strlen(temp); i++) {
      if (temp[i] < '0' || temp[i] > '9') valido = 0;
    }
  } while (!valido);

  strcpy(campo, temp);
}

/*Funcao para a leitura do campo com identificador da imagem */
void leImagem(char *campo, int tamanho, char *msg) {
  char temp[TAM_IMAGEM+3];
  int i, tamEntrada, valido = 1;

  do {
    LimpaBuffer();
    if (!valido) MensagemErro();

    valido = 1;
    printf(msg);
    fgets(temp, sizeof(temp), stdin);

    tamEntrada = strlen(temp);
    temp[--tamEntrada] = '\0'; /* fgets pega '\n' */

    if (tamEntrada != TAM_IMAGEM) valido = 0; /* deve ter o tamanho correspondente ao numero de caracteres da imagem */

    /* verifica se os seis primeiros caracteres sao numeros.
       quanto aos outros tres impomos nenhuma restricao,
       podem ser numeros ou letras */
    for (i = 0; i < TAM_IMAGEM - 3; i++) {
      if (temp[i] < '0' || temp[i] > '9') valido = 0;
    }

  } while (!valido);

  strcpy(campo, temp);
}

/* Esta funcao preenche o restante de um campo com espacos
 * em branco, caso o tamanho da entrada do usuario seja menor
 * que o tamanho do campo. Também desloca o '\0' para o fim da string*/
void preencher(char *campo, int tamanho) {
  int begin = strlen(campo);
  int i;
  
  for (i = begin; i < tamanho - 1; i++) {
    campo[i] = ' ';
  }
  campo[i] = '\0';
}

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
 * parametros a struct TObra e o arquivo de saida e a avail da base de dados.
 * Retorna o endereco da obra inserida.
*/
int gravaObra(TObra obra, FILE *arq, availList *avail){
  
  int end, prox = 0;
  
  /*Pega o valor da avail, e percorre a base com fseek para inserir na posicao.*/
  fseek(arq, (*avail*(int)TAM_REG), SEEK_SET);
  /*Se for o final do arquivo, atualizo a avail com +1*/
  if(fscanf(arq, FORMATO_INT, &prox) == EOF) {
    end = *avail;
    *avail = *avail + 1;
    fprintf(arq, "%s", obra.titulo);
    fprintf(arq, "%s", obra.tipo);
    fprintf(arq, "%s", obra.autor);
    fprintf(arq, "%s", obra.ano);
    fprintf(arq, "%s", obra.valor);
    fprintf(arq, "%s", obra.imagem);
    return end;
  } else { /*Caso nao e final da base */
    end = *avail;
    *avail = prox;
    /*Volta para sobrescrever o antigo next da avail*/
    fseek(arq, -TAM_NUMERO, SEEK_CUR);
    fprintf(arq, "%s", obra.titulo);
    fprintf(arq, "%s", obra.tipo);
    fprintf(arq, "%s", obra.autor);
    fprintf(arq, "%s", obra.ano);
    fprintf(arq, "%s", obra.valor);
    fprintf(arq, "%s", obra.imagem);
    return end;
  }
}

/*** Funcoes de manipulacao do indice primario. ***/
/* funcao que carrega o indice do arquivo ou monta-o a partir da base */
TIndice * carregaIndice(FILE *base, TIndice *indice, availList *avail) {
  FILE *arq_ind; /* arquivo onde estao os indices, caso exista */
  int offset;
  int *tam;      /* apontador pro numero de elementos do vetor de
		    indice. Pra facilitar a leitura do codigo. */

  arq_ind = fopen(ARQ_PK, "r");
  indice = (TIndice *) malloc(sizeof(TIndice));
  indice->vetor = (ElementoIndice *) malloc(sizeof(ElementoIndice) * VETOR_MIN);
  indice->alocado = VETOR_MIN;
  indice->tamanho = 0;

  tam = &(indice->tamanho);

  if (arq_ind) { /* existe o arquivo */

    while(fgets(indice->vetor[(*tam)].pk, TAM_TITULO+1, arq_ind)) {
      (*tam)++;
      indice = realocaIndice(indice);
      fscanf(arq_ind, "%d", &(indice->vetor[*tam-1].nrr)); /* le o nrr do registro */
    }

    fclose(arq_ind);
  } else { /* vai ter que gerar a partir da base */
 
    while(fgets(indice->vetor[(*tam)].pk, TAM_TITULO+1, base)){
      (*tam)++;
      indice = realocaIndice(indice);
      indice->vetor[*tam-1].nrr = *tam - 1;

      /* Posiciona o cursor do arquivo no campo Titulo do proximo registro. */
      /* Usamos apenar tamanho e nao tamanho-1 pois iniciamos o nrr em zero. */
      offset = TAM_REG * (*tam);
      fseek(base, offset, SEEK_SET);
    }

    /* Como o indice neste caso nao esta ordenado, precisamos ordena-lo */
    qsort(indice->vetor, *tam, sizeof(ElementoIndice), compare);
  }
  /*Caso a availList da base seja vazia, aponta ela para o final do arquivo*/
  if(*avail == -1) {
    *avail = indice->tamanho;
  }
  return indice;
}

/*  Realiza a ordenacao do indice usando qsort. */
void ordenaIndice(TIndice *indice) {
  qsort(indice->vetor, indice->tamanho, sizeof(ElementoIndice), compare);
}

/* Gravacao do indice, ordenado por pk, no arquivo */
void gravaIndice(TIndice *indice) {
  FILE *ind;
  int i;

  ind = fopen(ARQ_PK, "w");

  for (i = 0; i < indice->tamanho; i++) {
    fprintf(ind, "%s", indice->vetor[i].pk);  /* grava a chave primaria   */
    fprintf(ind, "%d", indice->vetor[i].nrr); /* grava o nrr do registro. */
  }

  /* Libera memoria da estrutura E dos seus campos. */
  free(indice->vetor);
  free(indice);

  fclose(ind);
}

/* Consulta de uma obra na base. 
   Chave já vem preenchido. */
int consulta(ElementoIndice *chave, FILE *base, TIndice *indice, TObra *reg) {
  ElementoIndice *temp;
  int retorno;
  
  temp = (ElementoIndice *) bsearch(chave, indice->vetor, indice->tamanho, sizeof(ElementoIndice), compare);

  if (temp) { /* registro encontrado */
    /* TObra * reg = (TObra *) malloc(sizeof(TObra)); */
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
      printf("\n*******************************************************\n");
      printf("Registo encontrado e saida gerada com sucesso.\n");
      printf("A consulta esta disponivel no arquivo %s.\n", ARQ_HTML);
      printf("*******************************************************\n");				
    } else {
      printf("\n*********************\n");
      printf("Erro ao gerar saida!\n");
      printf("*********************\n");
    }
		/*Fim do HTML*/
    endHtml(saida);
    
    fclose(saida);

  /* return reg; */
		
  } else {/*Registro nao encontrado*/
    retorno = 0;
    printf("\n***********************\n");
    printf("Registo não encontrado.\n");
    printf("***********************\n");
  }

  return retorno;
}

/* 
 * Realiza a listagem as obras na base, e envia os resultados pra um
 * arquivo em html. 
 */
void listaBase(FILE *base, TIndice *indice) {
  int i;
  TObra reg;
  FILE *saida;
  ElementoIndice *temp;

  saida = fopen(ARQ_HTML, "w");

  /*Inicio do HTML*/
  headHtml(saida);
  
  temp = indice->vetor;	
  for (i = 0; i < indice->tamanho; i++) {
    fseek(base, TAM_REG*(temp[i].nrr), SEEK_SET);

    /* leitura do registro */
    fgets(reg.titulo, TAM_TITULO+1, base);
    fgets(reg.tipo, TAM_TIPO+1, base);
    fgets(reg.autor, TAM_AUTOR+1, base);
    fgets(reg.ano, TAM_ANO+1, base);
    fgets(reg.valor, TAM_VALOR+1, base);
    fgets(reg.imagem, TAM_IMAGEM+1, base);

    preencheHtml(saida, reg);
  }
  /*Final do HTML*/
  endHtml(saida);
  
  printf("\n*******************************************************\n");
  printf("A consulta esta disponivel no arquivo %s.\n", ARQ_HTML);
  printf("*******************************************************\n")	;

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

  str1 = (*(ElementoIndice *)a).pk;
  str2 = (*(ElementoIndice *)b).pk;

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
TIndice * realocaIndice(TIndice *ind) {

  if (ind->tamanho == ind->alocado) {
    ind->alocado *= 2;
    ind->vetor = (ElementoIndice *) realloc(ind->vetor, sizeof(ElementoIndice) * ind->alocado);
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

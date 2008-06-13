#include "busca.h"

void gravaHtml(resultadosBusca *);


/*Esta funcao unifica o processo de busca por PK e criacao de arquivo
  de saida. Um "adapter" para o main. */
void buscaPrimario(IndPrim *primario, FILE* base) {
	
  Pk *temp;
  resultadosBusca *busca = NULL;
	
  leTexto(temp->pk, sizeof(temp->pk), "Digite a PK da Obra: ");

  busca = buscaPk(temp, base, primario, NULL);

  if(busca && busca->tamanho) {

    gravaHtml(busca);
    printSearchSuccess();

  } else {
    printSearchFailed();
  }

  if (busca) liberaBusca(busca);
	
}


/*Esta funcao unifica o processo de busca po4 SK e criacao de arquivo
  de saida. Um "adapter" para o main. */
void buscaSecudario(IndPrim *primario, IndSec *secundario, FILE* base) {
	
  char temp[TAM_TITULO+1];
  resultadosBusca *busca;
	
  printf("Digite uma palavra: ");
  scanf("%s", temp);

  maiuscula(temp);
  secundario = trocaIndSec(secundario, temp);

  busca = buscaSk(temp, primario, secundario, base);

  if(busca && busca->tamanho) {

    gravaHtml(busca);
    printSearchSuccess();

  } else {
    printSearchFailed();
  }

  if (busca) liberaBusca(busca);
	
}

/*Esta funcao unifica o processo de busca por conteudo e criacao de arquivo
  de saida. Um "adapter" para o main. */
void buscaDescritor(IndDesc *descritores, IndPrim *primario, FILE *base) {

  char temp[TAM_TITULO];

  resultadosBusca *busca = NULL;

  printf("Digite o nome da imagem: ");
  scanf("%s", temp);
	
  busca = buscaPorConteudo(temp, descritores, primario, base);

  if(busca) {
    if(busca->tamanho) {

      gravaHtml(busca);
      printSearchSuccess();

    } else {
      printSearchFailed();
    }

    liberaBusca(busca);
  }
	
}


/* Consulta de uma obra na base. 
   Aloca espaco para o vetor de resposta. Esse espaco eh liberado nas funcoes de HTML. */
resultadosBusca * buscaPk(Pk *chave, FILE *base, IndPrim *indice, resultadosBusca *result) {
  
  Pk *temp;
  int tam, imprime = 0; /* Usado para saber se deve imprimir mensagens pro usuario*/

  /* criando a estrutura de resposta. */
  /* quando a funcao eh usada diretamente. */
  if(!result) {

    imprime = 1;

    result = (resultadosBusca *) malloc(sizeof(resultadosBusca));
    result->alocado = VETOR_MIN;
    result->tamanho = 0;
    result->obras = (TObra *) malloc(sizeof(TObra) * result->alocado);
    result->similaridades = (double *) malloc(sizeof(double)  * result->alocado);
    strcpy(result->chave, chave->pk);
    
    /* Desativa a similaridade. Se ela for necessaria, sera ativada no lugar certo. */
    result->similaridades[0] = -1;

  } else if (result->tamanho == result->alocado) {

    result->alocado *= 2;

    result->obras = (TObra *) realloc(result->obras, sizeof(TObra) * result->alocado);
    result->similaridades = (double *) realloc(result->similaridades, sizeof(double) * result->alocado);
  }

  /*Abro o indice relativo a pk a ser buscada*/
  maiuscula(chave->pk);
  indice = trocaIndPrim(indice, chave->pk);
  
  temp = (Pk *) bsearch(chave, indice->vetor, indice->tamanho, sizeof(Pk), compare);

  if (temp) { /* registro encontrado */
    tam = result->tamanho;

    fseek(base, TAM_REG * (temp->nrr), SEEK_SET);

    /* leitura do registro */
    fgets(result->obras[tam].titulo, TAM_TITULO + 1, base);
    fgets(result->obras[tam].tipo, TAM_TIPO + 1, base);
    fgets(result->obras[tam].autor, TAM_AUTOR + 1, base); 
    fgets(result->obras[tam].ano, TAM_ANO + 1, base);
    fgets(result->obras[tam].valor, TAM_VALOR + 1, base);
    fgets(result->obras[tam].imagem, TAM_IMAGEM + 1, base);
    
    (result->tamanho)++; /* Atualiza o tamanho dos vetores. */

  }

  return result;
}


resultadosBusca * buscaSk(char *chave, IndPrim *indPrim, IndSec *indSecun, FILE *base) {
  int offset;

  Sk temp, *result;
  Pk temp2;

  resultadosBusca *busca = NULL;
  
  FILE *fsk;
  
  char nomeArq[TAM_NOME_ARQ+10];
  
  sprintf(nomeArq, "%s%s", indSecun->tipoCampo, EXTENSAO_PK);
  fsk = fopen(nomeArq,"r");

  /*Busca chave no indice secundario*/
  strcpy(temp.key, chave);
  temp.lenght = strlen(chave);
  temp.next = -1;
  
  result = (Sk*) bsearch(&temp, indSecun->vetor, indSecun->tamanho, sizeof(temp), compareSk);
  
  if (result) { /*Encontrou chave*/
    
    temp2.nrr = result->next;
    
    while (temp2.nrr != -1) {
      
      offset = temp2.nrr * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;
      fseek(fsk, offset, SEEK_SET);
      
      fgets(temp2.pk, TAM_TITULO+1, fsk);
      fscanf(fsk, FORMATO_INT, &(temp2.nrr));
      
      busca = buscaPk(&temp2, base, indPrim, busca);
      
    }
    
    fclose(fsk);
    
    /* Informacao sobre a chave de busca. */
    strcpy(busca->chave, chave);
 
  }

  return busca;
}

resultadosBusca * buscaPorConteudo(char *arqImagem, IndDesc *indice, IndPrim *indPrim, FILE *base) {
	
  int i, n;
  IndDesc *resposta;
  Pk temp;
  FILE *teste;
  resultadosBusca *busca = NULL;
  
  /* Verifica se a imagem de referencia existe, para evitar que o programa capote. */
  
  if((teste = fopen(arqImagem, "r")) == NULL) {

    printf("\nA imagem fornecida nao pode ser carregada.\n");
    printf("Verifique se o nome esta correto.\n");
       
    return busca; /* Sai da funcao sem fazer nada. */
  }
  
  /* A imagem existe. Libera o arquivo. */
  fclose(teste);
  
  resposta = filtraInd(indice, arqImagem);
	
  printf("No maximo quantas imagens? ");
  scanf("%d", &n);

  for (i = resposta->tamanho-1; i >= 0 && n > 0; --i, n--) {

    strcpy(temp.pk, resposta->vetor[i].pk);

    busca = buscaPk(&temp, base, indPrim, busca);
    busca->similaridades[busca->tamanho-1] = resposta->vetor[i].similaridade;

  }
  
  /* Informacao sobre a chave de busca. Neste caso, a imagem a ser comparada. */
  strcpy(busca->chave, arqImagem);

  return busca;
}

void gravaHtml(resultadosBusca *result) {
  
  FILE *b;
  char aux[TAM_IMAGEM + 2];
  int j,i = 0, k;
  int ponto;

  /* Verifica se  eh uma busca por conteudo. */
  int porConteudo = (result->similaridades[0] != -1) ? 1 : 0;

  b = fopen(ARQ_HTML, "w");

  /*Cabecalho do HTML */
  fprintf(b, "%s", "<html><head><title>Consulta do catalogo de obras de arte</title></head>");

  /*Inicio da tabela de resposta. */
  fprintf(b, "%s","<body> <table border=\"1\" width=\"800\"><tr><th colspan ='3' align=\"center\"><font size='6' color='red'><b>Consulta do catalogo de obras de arte</b></th></tr>");

  fprintf(b, "<tr height=\"8\"></tr>");

  /* Informacoes sobre a busca. */
  fprintf(b, "<tr> <td colspan ='3'>");

  if(!porConteudo) {
    fprintf(b, "<p>Resultados da Busca por: <b>%s</b>", result->chave);
  } else {
    fprintf(b, "<p>Resultados da Semelhança com a imagem <b>%s</b>", result->chave);
    fprintf(b, "<br><a href=\"%s\"><img src=\"%s\"", result->chave, result->chave);
    fprintf(b, " width=\"180\" height=\"110\" alt=\"Clique na imagem para visualizar em tamanho original\"></a>");
  }

  fprintf(b, "<br>Número de resultados: <b>%d</b></td></tr>", result->tamanho);

  for (k = 0; k < result->tamanho; k++) {
  
    /*Preenchimento da tabela*/
    fprintf(b, "<tr height=\"8\"></tr>");
    fprintf(b, "<tr><td nowrap width=\"200\"><b>TITULO DA OBRA</b></td><td nowrap width=\"400\">");
    fprintf(b, "%s", result->obras[k].titulo);	
    fprintf(b, "</td><td nowrap width=\"200\" align=\"center\"><b>IMAGEM</b></td></tr>");
    fprintf(b, "<tr><td><b>TIPO DA OBRA</b></td><td>");
    fprintf(b, "%s", result->obras[k].tipo);
    fprintf(b, "</td><td rowspan=\"4\" align=\"center\">");	

    /* 
     * Este loop insere o ponto no nome do arquivo da imagem. Ele
     * verifica se ateh as tres ultimas posicoes do vetor ha somente
     * digitos numericos e insere o ponto assim que essa condicao
     * torna-se falsa. 
     */

    ponto = 0;
    for(i = 0, j = 0; i < TAM_IMAGEM; i++, j++)
      {
	aux[j] = result->obras[k].imagem[i];
	if ( (result->obras[k].imagem[i] < '0' || result->obras[k].imagem[i] > '9') && ponto == 0)
	  {
	    aux[j] = '.';
	    --i;
	    ponto = 1;
	  }
      }
    aux[j] = '\0';

    fprintf(b, "<a href=\"img/%s\"><img src=\"img/%s\" ", aux, aux);
    fprintf(b, "\"width=\"180\" height=\"110\" alt=\"Clique na imagem para visualizar em tamanho original\"></a></td></tr>");
    fprintf(b, "<tr><td><b>AUTOR</b></td><td>");
    fprintf(b, "%s", result->obras[k].autor);
    fprintf(b, "</td></tr><tr><td><b>ANO</b></td><td>");
    fprintf(b, "%s", result->obras[k].ano);
    fprintf(b, "</td></tr><tr><td><b>VALOR</b></td><td>");
    fprintf(b, "%s", result->obras[k].valor);
  
    if (porConteudo) {
      fprintf(b, "</td></tr><tr><td><b>Similaridade</b></td><td>");
      fprintf(b, "%0.1f%%", result->similaridades[k] * 100); /* Imprime a similaridade em porcentagem. */
    }
  
    fprintf(b, "%s", "</td><tr>");
  
  }

  /* Imprime o fim do arquivo HTML. */
  fprintf(b, "</table></body></html>");

  fclose(b);

}

/* Esta funcao libera a memoria alocada para a resposta das buscas.
 * Tera efeitos indesejavies se usada em um ponteiro que nao foi usado para uma busca. */
void liberaBusca(resultadosBusca *result) {

  free(result->obras);
  free(result->similaridades);
  free(result);

}

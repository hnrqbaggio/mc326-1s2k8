#include "busca.h"

FILE * Html(FILE *b, TObra valores, double similaridade);

void buscaSecudario(IndPrim *primario, IndSec *secundario, FILE* base) {
	
	char temp[TAM_TITULO+1];
	
	printf("Digite uma palavra: ");
	scanf("%s", temp);
	maiuscula(temp);
	secundario = trocaIndSec(secundario, temp);
	buscaSk(temp, primario, secundario, base);
	
}

void buscaSk(char *chave, IndPrim *indPrim, IndSec *indSecun, FILE *base) {
  
  Sk temp, *result;
  Pk temp2, *result2;

  FILE *fhtml, *fsk;
  TObra reg;

  int offset;
  char nomeArq[TAM_NOME_ARQ+10];
  
  sprintf(nomeArq, "%s%s", indSecun->tipoCampo, EXTENSAO_PK);
  fsk = fopen(nomeArq,"r");

  fhtml = fopen(ARQ_HTML, "w");
 
  /*Busco chave no indice secundario*/
  strcpy(temp.key, chave);
  temp.lenght = strlen(chave);
  temp.next = -1;

  result = (Sk*) bsearch(&temp, indSecun->vetor, indSecun->tamanho, sizeof(temp), compareSk);
  
  if (result) { /*Encontrou chave*/
    /*Cabecalho do HTML*/
    headHtml(fhtml);
    temp2.nrr = result->next;

    while (temp2.nrr != -1) {
      
      offset = temp2.nrr * (TAM_TITULO + TAM_NUMERO) + TAM_NUMERO;
      fseek(fsk, offset, SEEK_SET);

      fgets(temp2.pk, TAM_TITULO+1, fsk);
      fscanf(fsk, FORMATO_INT, &(temp2.nrr));
      
      /* Verifico o indice ao qual pertence a nova chave. */
      trocaIndPrim(indPrim, temp2.pk);

      result2 = (Pk *) bsearch(&temp2, indPrim->vetor, indPrim->tamanho, sizeof(temp2), compare);

      offset = TAM_REG * result2->nrr;
      fseek(base, offset, SEEK_SET);

      /* leitura do registro */
      fgets(reg.titulo, TAM_TITULO + 1, base);
      fgets(reg.tipo, TAM_TIPO + 1, base);
      fgets(reg.autor, TAM_AUTOR + 1, base); 
      fgets(reg.ano, TAM_ANO + 1, base);
      fgets(reg.valor, TAM_VALOR + 1, base);
      fgets(reg.imagem, TAM_IMAGEM + 1, base);

      /*Insere no arquivo HTML*/
    	preencheHtml(fhtml, reg);
    }
    /*Finaliza as tags abertas do HTML*/
    endHtml(fhtml);
    printSearchSuccess();
    
  } else {/*Nenhum registro encontrado*/
  	printSearchFailed();
  }

  fclose(fsk);
  fclose(fhtml);
}

void buscaPorConteudo(char *arqImagem, IndDesc *indice, IndPrim *indPrim, FILE *base) {
	
	int i;
	IndDesc * resposta;
	Pk temp, *result;
	FILE *saida;
	TObra reg;
	
	saida = fopen(ARQ_HTML, "w");

	resposta = filtraInd(indice, arqImagem);
	
	/*Inicio do HTML*/
	headHtml(saida);
	
	for (i = resposta->tamanho; i >=0; --i) {
		strcpy(temp.pk, resposta->vetor[i].pk);
		
		/*Abre o indice relativo a pk a ser buscada*/

  		indPrim = trocaIndPrim(indPrim, temp.pk);
		
		result = (Pk *) bsearch(&temp, indPrim->vetor, indPrim->tamanho, sizeof(temp), compare);
		
		if (result) {
		    
		    fseek(base, TAM_REG * (result->nrr), SEEK_SET);
		
		    /* leitura do registro */
		    fgets(reg.titulo, TAM_TITULO + 1, base);
		    fgets(reg.tipo, TAM_TIPO + 1, base);
		    fgets(reg.autor, TAM_AUTOR + 1, base); 
		    fgets(reg.ano, TAM_ANO + 1, base);
		    fgets(reg.valor, TAM_VALOR + 1, base);
		    fgets(reg.imagem, TAM_IMAGEM + 1, base);
		
		    /* passagem do resultado pra função que gera a saida em html */
		    Html(saida, reg, resposta->vetor[i].similaridade);
		    
		}	

	}

	/*Fim do HTML*/
	endHtml(saida);
	fclose(saida);

	printSearchSuccess();
}

FILE * Html(FILE *b, TObra valores, double similaridade) {
  
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
  
  fprintf(b, "%s","</td></tr><tr><td><b>Similaridade</b></td><td>");
  fprintf(b, "%f", similaridade);
  
  fprintf(b, "%s", "</td><tr>");
  
  return b;
}

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
}.

IndSec * criaSk(TIndice *, FILE *, const int).

IndSec * insereSk(SkVetor *, char *).


/* Realoca espaco para o vetor caso seja necessario. */
IndSec * realocaIndSec(IndSec *sec) {

  if (sec->tamanho == sec->alocado) {
    sec->alocado *= 2;
    sec->vetor = (Sk *) realloc(sec->vetor, sizeof(Sk) * sec->alocado);
  }

  return sec;
}

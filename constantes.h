/** 	\file constantes.h
 * 	\brief Arquivo com as definicoes das constantes usadas no codigo.
 */

#ifndef CONSTANTES_H_
#define CONSTANTES_H_

/** @{
 * @name Definicoes de tamanhos de campos e do tamanho do registro 
 */
#define TAM_TITULO 200 	/**< Tamanho do campo titulo da obra. */
#define TAM_TIPO 100		/**< Tamanho do campo tipo da obra. */
#define TAM_AUTOR 125	/**< Tamanho do campo autor da obra. */
#define TAM_ANO 4			/**< Tamanho do campo ano da obra. */
#define TAM_VALOR 12		/**< Tamanho do campo valor da obra. */
#define TAM_IMAGEM 9		/**< Tamanho do campo imagem da obra. */

/** Tamanho total de um registro. */
#define TAM_REG (TAM_TITULO+TAM_TIPO+TAM_AUTOR+TAM_ANO+TAM_VALOR+TAM_IMAGEM)
/** @} */

/**
 * @name Nomes dos arquivos que contem os registros do indice primario.
 * Cada um deles tera um sufixo numerico que indica o valor da funcao
 * de espalhamanto das chaves do inidice contido no arquivo.
 */
#define ARQ_PK ("primario")             


/** @{
 * @name Nomes para os arquivos que contem indices secundarios.
 * Como nos indices primarios, cada arquivo tera um sufixo que 
 * indicara o valor da funcao hash para as chaves no indice.
 */
#define ARQ_IS_TITULO ("titulo") 	/**< Avail list do indice de titulos */
#define ARQ_IS_TIPO ("tipo")		/**< Avail list da indice de tipos. */
#define ARQ_IS_AUTOR ("autor")		/**< Avail list da indice de autores. */
#define ARQ_IS_ANO ("ano")	        /**< Avail list da indice de anos.  */
/** @}*/

/** 
 * @name Nome para os arquivos que contem indices de descritores. 
 * Assim como nos demais indices, cada arquivo tera um sufixo que 
 * indicara o valor da funcao hash para as chaves no indice.
 */
#define ARQ_DESCRITOR ("descritor")

/** @{
 * @name Nomes dos arquivos de avail list. 
 */
#define ARQ_AVAIL_BASE   ("base24.av")  /**< Avail list da base de dados. */
#define ARQ_AVAIL_TITULO ("titulo.av")	/**< Avail list do indice de titulos. */
#define ARQ_AVAIL_TIPO   ("tipo.av")    /**< Avail list do indice de tipos */
#define ARQ_AVAIL_AUTOR  ("autor.av")	/**< Avail list do indice de autores */
#define ARQ_AVAIL_ANO    ("ano.av")	/**< Avail list do indice de anos. */
/** @} */

/** @{
 * @name Extensoes dos arquivos. 
 */
#define EXTENSAO_PK (".chp") 		/**< Extensao dos arquivos formada por chaves primarias: Indices primarios e BigFiles. */
#define EXTENSAO_SK (".chs") 		/**< Entensao dos arquivos formados por chaves secundarias. */
#define EXTENSAO_DESC (".dsc") 	/**< Entensao dos arquivos formados por descritores de imagens. */
/** @} */

/** @{ 
 * @name Demais constantes para nomes de arquivos.
 */
/** Base de dados */
#define ARQ_BASE ("base24.dat")

/** Html para saida dos resultados de busca */
#define ARQ_HTML ("base24.html")
/** @} */

/** @{
 * @name Constantes para gravacao de campos numericos.
 */
/** Define para a mascara usada para ler e gravar inteiros. */
#define FORMATO_INT ("%08d")

/** Define para o numero de digitos dos inteiros. */
#define TAM_NUMERO 8
/** @} */

/** @{
 * @name Constantes usadas como parametros na funcao geraHtml.
 */
/**
 * Adiciona tanto o cabecalho quanto o fim do html. 
 * Usado quando eh uma consulta de um registro especifico. 
 */
#define ALL 0 
#define HEAD 1 /**< Deve ser inserido um cabecalho no html */
#define END 2 /**< Deve fechar as tags do html. */

/** 
 * Diferencia os registros que nao sao nem inicio nem 
 * fim da listagem, logo, devem apenas ser inseridos 
 */
#define MEIO 3
/** @}*/

/** @{
 * @name Constantes usadas para determinar o tipo do campo para o indice. 
 */
#define TITULO 0
#define TIPO 1
#define AUTOR 2
#define ANO 3
/** @} */

/** @{ 
 * @name Demais contantes 
 */
/** Constante usada para indicar o fim de uma lista invertida. */
#define FIM_DE_LISTA -1

/** Constante usada na funcao de espalhamento dos indices primarios e secundarios. */
#define H 53

/** 
 * Constante para fazer reallocs mais eficientes. A primeira alocacao
 * sera feita com esse valor, e dobrando sempre que atingir o maximo 
 */
#define VETOR_MIN 20

/** Tamanho do nome de um arquivo. */
#define TAM_NOME_ARQ 256
/** @} */

#endif /*CONSTANTES_H_*/

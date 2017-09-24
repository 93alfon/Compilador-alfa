%{
	/*******************************
 	Alfonso Bonilla Trueba
 	Pareja 11 - Grupo 1311
	*******************************/
	#include <stdio.h>
	#include <stdlib.h>
	#include "../includes/alfa.h"
	#include "../includes/tabla_simbolos.h"
	#include "../includes/generacionCodigo.h"
 	#include <string.h>

	extern int yylex();
	extern int morf_err;
	extern int linea;
	extern int columna;
	extern int yyleng;
	extern FILE* yyout;
	FILE* ensamblador;
	int yyerror();
	int clase;
	int tipo;
	int tipo_funcion;

	int num_variables_locales_actual = 0;
	int pos_variable_local_actual = 1;
	int num_parametros_actual = 0;
	int pos_parametro_actual = 0;
	int tamanio_vector_actual = 0;
	int num_parametros_llamada_actual = 0;
	int en_explist = 0;
	int contador_etiqueta = 0;

	int sem_err = 0;
	int retorno_funcion = 0;
	
	TABLA_SIMBOLOS* ts;
%}

%union {
	tipo_atributos atributos;
}

%token <atributos> TOK_IDENTIFICADOR
%token <atributos> TOK_CONSTANTE_ENTERA


%token TOK_MAIN TOK_INT TOK_BOOLEAN TOK_ARRAY TOK_FUNCTION TOK_IF TOK_ELSE TOK_WHILE TOK_SCANF TOK_PRINTF TOK_RETURN TOK_PUNTOYCOMA TOK_COMA TOK_PARENTESISIZQUIERDO TOK_PARENTESISDERECHO TOK_CORCHETEIZQUIERDO TOK_CORCHETEDERECHO TOK_LLAVEIZQUIERDA TOK_LLAVEDERECHA TOK_ASIGNACION TOK_MAS TOK_MENOS TOK_DIVISION TOK_ASTERISCO TOK_AND TOK_OR TOK_NOT TOK_IGUAL TOK_DISTINTO TOK_MENORIGUAL TOK_MAYORIGUAL TOK_MENOR TOK_MAYOR TOK_ERROR TOK_TRUE TOK_FALSE


%left TOK_MAS TOK_MENOS TOK_OR
%left TOK_ASTERISCO TOK_DIVISION TOK_AND
%right MENOSU TOK_NOT

%type <atributos> exp
%type <atributos> comparacion
%type <atributos> identificador
%type <atributos> constante_logica
%type <atributos> constante_entera
%type <atributos> constante
%type <atributos> idf_llamada_funcion
%type <atributos> funcion
%type <atributos> elemento_vector
%type <atributos> fn_declaration
%type <atributos> fn_name
%type <atributos> if_exp
%type <atributos> if_exp_sentencias
%type <atributos> while_exp
%type <atributos> while



%start programa

%%

programa:	iniciaTabla TOK_MAIN TOK_LLAVEIZQUIERDA escritura_TS declaraciones escritura_codigo funciones escritura_main sentencias TOK_LLAVEDERECHA	{
		fprintf(yyout,";R1:\t<programa> ::= main { <declaraciones> <funciones> <sentencias> }\n");
		borrar_tabla_simbolos(ts);
		escribir_fin(ensamblador);
	}
	;

iniciaTabla:	/*Vacio*/	{
		ts = crear_tabla_simbolos();
	}
	;

escritura_TS:	/*Vacio*/	{
		escribir_seccion_data(ensamblador);
		escribir_cabecera_bss(ensamblador);
	}
	;

escritura_codigo:	/*Vacio*/{
		escribir_segmento_codigo(ensamblador);
	}
	;

escritura_main:	/*Vacio*/	{
		escribir_inicio_main(ensamblador);
	}
	;

declaraciones:	 declaracion	{	fprintf(yyout, ";R2:\t<declaraciones> ::= <declaracion>\n");	}
	|declaracion declaraciones	{	fprintf(yyout, ";R3:\t<declaraciones> ::= <declaracion> <declaraciones>\n");	}
	;

declaracion:	clase identificadores TOK_PUNTOYCOMA	{
		fprintf(yyout, ";R4:\t<declaracion> ::= <clase> <identificadores> ;\n");
	}
	;

clase:	clase_escalar	{
		fprintf(yyout, ";R5:\t<clase> ::= <clase_escalar>\n");
		clase = ESCALAR;
	}
	|clase_vector	{
		fprintf(yyout, ";R7:\t<clase> ::= <clase_vector>\n");
		clase = VECTOR;
	}
	;

clase_escalar:	tipo	{	fprintf(yyout, ";R9:\t<clase_escalar> ::= <tipo>\n");	}
	;

tipo:	TOK_INT	{
		fprintf(yyout, ";R10:\t<tipo> ::= int\n");
		tipo = ENTERO;
	}
	|	TOK_BOOLEAN {
		fprintf(yyout, ";R11:\t<tipo> ::= boolean\n");
		tipo = BOOLEANO;
	}
	;

clase_vector:	TOK_ARRAY tipo  TOK_CORCHETEIZQUIERDO constante_entera TOK_CORCHETEDERECHO	{
		fprintf(yyout, ";R15:\t<clase_vector> ::= array <tipo> [ <constante_entera> ]\n");
		tamanio_vector_actual = $4.valor_entero;
	}
	;

identificadores:	identificador	{	fprintf(yyout, ";R18:\t<identificadores> ::= <identificador>\n");	}
	|	identificador TOK_COMA identificadores {	fprintf(yyout, ";R19:\t<identificadores> ::= <identificador> , <identificadores>\n");	}
	;

funciones:	funcion funciones	{	fprintf(yyout, ";R20:\t<funciones> ::= <funcion> <funciones>\n");	}
	|	{	fprintf(yyout, ";R21:\t<funciones> ::=\n");	}
	;

fn_name:	TOK_FUNCTION tipo TOK_IDENTIFICADOR	{
		/*Busco si exite el identificador*/
		if(buscar_tabla_simbolo(ts, $3.lexema) == TRUE){
			fprintf(stderr,"****Error semantico en lin %d: Declaracion duplicada.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(!declaracion_funcion(ts, $3.lexema, tipo)){
			fprintf(stderr,"****Error semantico en lin %d: No se pueden declarar funciones anidadas.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		pos_variable_local_actual = 1;
		num_variables_locales_actual = 0;
		pos_parametro_actual = 0;
		num_parametros_actual = 0;
		tipo_funcion = tipo;
		/*Propagacion*/
		strcpy($$.lexema, $3.lexema);

		/*Generacion codigo*/
		

	}
	;

fn_declaration:	fn_name TOK_PARENTESISIZQUIERDO parametros_funcion TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA declaraciones_funcion	{
		modificar_valores_funcion(ts, $1.lexema, num_parametros_actual, num_variables_locales_actual);
		/*Propagacion*/
		strcpy($$.lexema, $1.lexema);

		/*Generacion codigo*/
		declarar_funcion(ensamblador, $1.lexema, num_variables_locales_actual);
	}
	;

funcion:	fn_declaration sentencias TOK_LLAVEDERECHA	{
		fprintf(yyout, ";R22:\t<funcion> ::= function <tipo> <identificador> ( <parametros_funcion> ) { <declaraciones_funcion> <sentencias> }\n");
		
		if(retorno_funcion == 0){
			fprintf(stderr,"****Error semantico en lin %d: Funcion %s sin sentencia de retorno.\n", linea, $1.lexema);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/*Cierro el ambito de la funcion para volver al global*/
		cierre_ambito(ts);
		modificar_valores_funcion(ts, $1.lexema, num_parametros_actual, num_variables_locales_actual);

		/*Reseteo el flag que comprueba si tiene o no return la funcion*/
		retorno_funcion = 0;
	}
	;

parametros_funcion : parametro_funcion resto_parametros_funcion	
		{	fprintf(yyout, ";R23:\t<parametros_funcion> ::= <parametro_funcion> <resto_parametros_funcion>\n");	}
	|	{	fprintf(yyout, ";R24:\t<parametros_funcion> ::=\n");	}
	;

resto_parametros_funcion: TOK_PUNTOYCOMA parametro_funcion resto_parametros_funcion	
		{	fprintf(yyout, ";R25:\t<resto_parametros_funcion> ::= ; <parametro_funcion> <resto_parametros_funcion>\n");	}
	|	{	fprintf(yyout, ";R26:\t<resto_parametros_funcion> ::=\n");	}
	;

parametro_funcion:	tipo idpf	{	fprintf(yyout, ";R27:\t<parametro_funcion> ::= <tipo> <idpf>\n");	}
	;

declaraciones_funcion:	declaraciones	{	fprintf(yyout, ";R28:\t<declaraciones_funcion> ::= <declaraciones>\n");	}
	|	{	fprintf(yyout, ";R29:\t<declaraciones_funcion> ::=\n");	}
	;

sentencias:	sentencia	{	fprintf(yyout, ";R30:\t<sentencias> ::= <sentencia>\n");}
	|	sentencia sentencias	{	fprintf(yyout, ";R31:\t<sentencias> ::= <sentencia> <sentencias>\n");	}
	;

sentencia:	sentencia_simple TOK_PUNTOYCOMA	{	fprintf(yyout, ";R32:\t<sentencia> ::= <sentencia_simple> ;\n");	}
	|	bloque	{	fprintf(yyout, ";R33:\t<sentencia> ::= <bloque>\n");	}
	;

sentencia_simple:	asignacion	 {	fprintf(yyout, ";R34:\t<sentencia_simple> ::= <asignacion>\n");	}
	|	lectura	{	fprintf(yyout, ";R35:\t<sentencia_simple> ::= <lectura>\n");	}
	|	escritura	{	fprintf(yyout, ";R36:\t<sentencia_simple> ::= <escritura>\n");	}
	|	retorno_funcion	{	fprintf(yyout, ";R38:\t<sentencia_simple> ::= <retorno_funcion>\n");	}
	;

bloque:	condicional	{	fprintf(yyout, ";R40:\t<bloque> ::= <condicional>\n");	}
	|	bucle	{	fprintf(yyout, ";R41:\t<bloque> ::= <bucle>\n");	}
	;

asignacion:	TOK_IDENTIFICADOR TOK_ASIGNACION exp	{
		fprintf(yyout, ";R43:\t<asignacion> ::= <TOK_IDENTIFICADOR> = <exp>\n");

		if(buscar_tabla_simbolo(ts, $1.lexema) == FALSE){
			fprintf(stderr, "****Error semantico en lin %d: Acceso a variable no declarada (%s).\n", linea, $1.lexema);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(get_categoria_tabla_simbolo(ts, $1.lexema) == FUNCION){
			fprintf(stderr,"****Error semantico en lin %d: Asignacion incompatible.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(get_clase_tabla_simbolo(ts, $1.lexema) == VECTOR){
			fprintf(stderr,"****Error semantico en lin %d: Asignacion incompatible.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(get_tipo_tabla_simbolo(ts, $1.lexema) != $3.tipo){
			fprintf(stderr,"****Error semantico en lin %d: Asignacion incompatible.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/*No hay propagacion en esta regla*/


		/*Generacion codigo*/
		if(ambito_variable(ts, $1.lexema) == GLOBAL){
			asignar_variable(ensamblador, $1.lexema, $3.es_direccion);
		}
		else if(get_categoria_tabla_simbolo(ts, $1.lexema) == PARAMETRO){
			asignar_parametro(ensamblador, get_posicion(ts, $1.lexema), num_parametros_actual, $3.es_direccion);
		} else{
			asignar_variable_local(ensamblador, get_posicion(ts, $1.lexema), $3.es_direccion);
		}


	}
	|	elemento_vector TOK_ASIGNACION exp	{
		fprintf(yyout, ";R44:\t<asignacion> ::= <elemento_vector> = <exp>\n");

		if($1.tipo != $3.tipo){
			fprintf(stderr, "****Error semantico en lin %d: Asignacion incompatible.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/*No hay propagacion en esta regla*/

		/*Generacion codigo*/
		asignar_array(ensamblador, $3.es_direccion);
	}
	;

elemento_vector:	TOK_IDENTIFICADOR TOK_CORCHETEIZQUIERDO exp TOK_CORCHETEDERECHO	{
		fprintf(yyout, ";R48:\t<elemento_vector> ::= <TOK_IDENTIFICADOR> [ <exp> ]\n");

		if(buscar_tabla_simbolo(ts, $1.lexema) == FALSE){
			fprintf(stderr, "****Error semantico en lin %d: Acceso a variable no declarada (%s).\n", linea, $1.lexema);
			return EXIT_FAILURE;
		}
		
		if($3.tipo != ENTERO){
			fprintf(stderr, "****Error semantico en lin %d: El indice en una operacion de indexacion tiene que ser de tipo entero.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(get_clase_tabla_simbolo(ts, $1.lexema) != VECTOR || get_categoria_tabla_simbolo(ts, $1.lexema) == FUNCION){
			fprintf(stderr,"****Error semantico en lin %d: Intento de indexacion de una variable que no es de tipo vector.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/*Propagacion*/
		$$.tipo = get_tipo_tabla_simbolo(ts, $1.lexema);
		$$.es_direccion = !en_explist;

		/*Generacion codigo*/

		escribir_elemento_vector(ensamblador, $1.lexema, $3.es_direccion, get_tam_vector(ts, $1.lexema), en_explist);
	}
	;

condicional: if_exp TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA	{
		fprintf(yyout, ";R50:\t<condicional> ::= if ( <exp> ) { <sentencias> }\n");

		/*Generacion codigo*/
		fin_condicional(ensamblador, $1.etiqueta);

	}
	|	if_exp_sentencias TOK_ELSE TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA	{
		fprintf(yyout, ";R51:\t<condicional> ::= if ( <exp> ) { <sentencias> } else { <sentencias> }\n");

		/*Generacion codigo*/
		fin_condicional_completo(ensamblador, $1.etiqueta);
	}
	;

if_exp:	TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO	{
		if($3.tipo != BOOLEANO){
			fprintf(stderr,"****Error semantico en lin %d: Condicional con condicion de tipo int.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/* Generacion codigo */
		$$.etiqueta = contador_etiqueta++;
		inicio_if(ensamblador, $3.es_direccion, $$.etiqueta);
		}
		;

if_exp_sentencias:	if_exp TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA	{
		/* Generacion codigo */
		$$.etiqueta = $1.etiqueta;
		if_else(ensamblador, $$.etiqueta);
	}
	;

bucle: while_exp sentencias TOK_LLAVEDERECHA	{
		fprintf(yyout, ";R52:\t<bucle> ::= while ( <exp> ) { <sentencias> }\n");

		/* Generacion codigo */
		fin_while(ensamblador, $1.etiqueta);
	}
	;

while_exp:	while exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA{

		if($2.tipo != BOOLEANO){
			fprintf(stderr,"****Error semantico en lin %d: Bucle con condicion de tipo int.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}
	
		/* Generacion de codigo */
		$$.etiqueta = $1.etiqueta;
		comprobacion_while(ensamblador, $2.es_direccion, $$.etiqueta);
	}
	;
													
while:	TOK_WHILE TOK_PARENTESISIZQUIERDO	{

		$$.etiqueta = contador_etiqueta++;

		/* Generacion codigo */
		inicio_while(ensamblador, $$.etiqueta);
	}
	;

lectura: TOK_SCANF TOK_IDENTIFICADOR	{
		fprintf(yyout, ";R54:\t<lectura> ::= scanf <TOK_IDENTIFICADOR>\n");

		if(buscar_tabla_simbolo(ts, $2.lexema) == FALSE){
			fprintf(stderr,"****Error semantico en lin %d: Acceso a variable no declarada %s.\n",linea, $2.lexema);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(get_categoria_tabla_simbolo(ts, $2.lexema) == FUNCION || get_clase_tabla_simbolo(ts, $2.lexema) == VECTOR){
			fprintf(stderr,"****Error semantico en lin %d: Asignacion incompatible.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/*Generacion codigo*/
		if(ambito_variable(ts, $2.lexema) == GLOBAL){
			leer_variable(ensamblador, $2.lexema, get_tipo_tabla_simbolo(ts, $2.lexema));
		}
		else if (get_categoria_tabla_simbolo(ts, $2.lexema) == PARAMETRO){
			leer_parametro(ensamblador, get_posicion(ts, $2.lexema), num_parametros_actual, get_tipo_tabla_simbolo(ts, $2.lexema));
		}else{
			leer_variable_local(ensamblador, get_posicion(ts, $2.lexema), get_tipo_tabla_simbolo(ts, $2.lexema));
		}


	}
	;

escritura: TOK_PRINTF exp	{
		fprintf(yyout, ";R56:\t<escritura> ::= printf <exp>\n");

		/*Generacion Codigo*/
		print(ensamblador, $2.es_direccion, $2.tipo);
	}
	;

retorno_funcion: TOK_RETURN exp	{
		fprintf(yyout, ";R61:\t<retorno_funcion> ::= return <exp>\n");

		if(ambito_actual(ts) == GLOBAL){
			fprintf(stderr, "****Error semantico en lin %d: Sentencia de retorno fuera del cuerpo de una función.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(tipo_funcion != $2.tipo){
			fprintf(stderr, "****Error semantico en lin %d: Mensaje desconocido 3: sentencia de retorno de un tipo no compatible\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		retorno_funcion = 1;

		/* Generacion de codigo */
		fin_funcion(ensamblador, $2.es_direccion);

	}
	;

exp: TOK_MENOS exp %prec MENOSU
	{
		fprintf(yyout, ";R76:\t<exp> ::= - <exp>\n");
		if($2.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Operacion aritmetica con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		$$.tipo = ENTERO;
		$$.es_direccion = 0;

		/* Generacion de codigo */
		cambiar_signo(ensamblador, $2.es_direccion);
	}
	;

exp: exp TOK_MAS exp	{

		fprintf(yyout, ";R72:\t<exp> ::= <exp> + <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Operacion aritmetica con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}
		
		$$.tipo = ENTERO;
		$$.es_direccion = 0;

		/*Generacion Codigo*/
		suma_enteros(ensamblador, $1.es_direccion, $3.es_direccion);
			
	}
	|	exp TOK_MENOS exp 	{

		fprintf(yyout, ";R73:\t<exp> ::= <exp> - <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Operacion aritmetica con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}
		
		$$.tipo = ENTERO;
		$$.es_direccion = 0;

		/*Generacion Codigo*/
		resta_enteros(ensamblador, $1.es_direccion, $3.es_direccion);

	}
	|	exp TOK_DIVISION exp	{

			fprintf(yyout, ";R74:\t<exp> ::= <exp> / <exp>\n");
			if($1.tipo != ENTERO || $3.tipo != ENTERO){
				fprintf(stderr,"****Error semantico en lin %d: Operacion aritmetica con operandos boolean.\n", linea);
				sem_err = 1;
				yyerror();
				return EXIT_FAILURE;
			}
		
		$$.tipo = ENTERO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		division_enteros(ensamblador, $1.es_direccion, $3.es_direccion);

	}
	|	exp TOK_ASTERISCO exp	{

		fprintf(yyout, ";R75:\t<exp> ::= <exp> * <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Operacion aritmetica con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}
		
		$$.tipo = ENTERO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		multipliacion_enteros(ensamblador, $1.es_direccion, $3.es_direccion);

	}
	|	exp TOK_AND exp 	{

		fprintf(yyout, ";R77:\t<exp> ::= <exp> && <exp>\n");
		if($1.tipo != BOOLEANO || $3.tipo != BOOLEANO){
			fprintf(stderr,"****Error semantico en lin %d: Operacion logica con operandos int.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}
		
		$$.tipo = BOOLEANO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		bool_and(ensamblador, $1.es_direccion, $3.es_direccion);
	}
	|	exp TOK_OR exp 	{

		fprintf(yyout, ";R78:\t<exp> ::= <exp> || <exp>\n");
		if($1.tipo != BOOLEANO || $3.tipo != BOOLEANO){
			fprintf(stderr,"****Error semantico en lin %d: Operacion logica con operandos int.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}
		
		$$.tipo = BOOLEANO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		bool_or(ensamblador, $1.es_direccion, $3.es_direccion);
	}
	|	TOK_NOT exp	{

		fprintf(yyout, ";R79:\t<exp> ::= ! <exp>\n");
		if($2.tipo != BOOLEANO){
			fprintf(stderr,"****Error semantico en lin %d: Operacion logica con operandos int.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}
		
		$$.tipo = BOOLEANO;
		$$.es_direccion =0;

		bool_not(ensamblador, $2.es_direccion);

	}
	|	TOK_IDENTIFICADOR	{

		fprintf(yyout, ";R80:\t<exp> ::= <TOK_IDENTIFICADOR>\n");
		if(buscar_tabla_simbolo(ts, $1.lexema) == FALSE){
			fprintf(stderr,"****Error semantico en lin %d: Declaracion duplicada.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(get_clase_tabla_simbolo(ts, $1.lexema) == VECTOR){
			fprintf(stderr,"****Error semantico en lin %d: Asignacion incompatible.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		if(get_categoria_tabla_simbolo(ts, $1.lexema) == FUNCION){
			fprintf(stderr,"****Error semantico en lin %d: Asignacion incompatible.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		$$.tipo = get_tipo_tabla_simbolo(ts, $1.lexema);
		$$.es_direccion=!en_explist;/*1*/

		if(ambito_variable(ts, $1.lexema) == GLOBAL){
			apilar_direccion_variable(ensamblador, $1.lexema, en_explist);
		}
		else if (get_categoria_tabla_simbolo(ts, $1.lexema) == PARAMETRO){
			apilar_direccion_parametro(ensamblador, get_posicion(ts, $1.lexema), num_parametros_actual, en_explist);
		}else{
			apilar_direccion_variable_local(ensamblador, get_posicion(ts, $1.lexema), en_explist);
		}

	}
	|	constante	{
		fprintf(yyout, ";R81:\t<exp> ::= <constante>\n");
		$$.tipo = $1.tipo;
		$$.valor_entero = $1.valor_entero;
		$$.es_direccion = $1.es_direccion;
	}
	|	TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO	{
		fprintf(yyout, ";R82:\t<exp> ::= ( <exp> )\n");
		$$.tipo = $2.tipo;
		$$.es_direccion = $2.es_direccion;
	}
	|	TOK_PARENTESISIZQUIERDO comparacion TOK_PARENTESISDERECHO	{
		fprintf(yyout, ";R83:\t<exp> ::= ( <comparacion> )\n");
		$$.tipo = $2.tipo;
		$$.es_direccion = $2.es_direccion;
	}
	|	elemento_vector	{
		fprintf(yyout, ";R85:\t<exp> ::= <elemento_vector>\n");
		$$.tipo = $1.tipo;
		$$.es_direccion = $1.es_direccion;
	}
	|	idf_llamada_funcion TOK_PARENTESISIZQUIERDO lista_expresiones TOK_PARENTESISDERECHO	{
		fprintf(yyout, ";R88:\t<exp> ::= <idf_llamada_funcion> ( <lista_expresiones> )\n");

		/*Compruebo que el numero de parametros de la funcion es correcto*/
		if(num_parametros_llamada_actual != get_num_params_funcion(ts, $1.lexema)){
			fprintf(stderr,"****Error semantico en lin %d: Numero incorrecto de parametros en llamada a funcion.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/*Reseteo el flag de de que ya no recibo argumentos*/
		en_explist = 0;
		$$.tipo = get_tipo_tabla_simbolo(ts, $1.lexema);
		$$.es_direccion = 0;

		/* Generacion codigo */
		llamada_funcion(ensamblador, $1.lexema, get_num_params_funcion(ts, $1.lexema));

	}
	;

idf_llamada_funcion : TOK_IDENTIFICADOR	{
		fprintf(yyout, ";R88.2:\t<idf_llamada_funcion> ::= <TOK_IDENTIFICADOR>\n");
		/*Buscar en la tabla de simbolos el identificador*/
		if(buscar_tabla_simbolo(ts, $1.lexema) == FALSE){
			fprintf(yyout, "****Error semantico en lin %d: Acceso a variable no declarada (%s)\n",linea, $1.lexema);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/*Comprobar que el identificador es de tipo funcion*/
		if(get_categoria_tabla_simbolo(ts, $1.lexema) != FUNCION){
			fprintf(stderr,"****Error semantico en lin %d: SIN MENSAJE(%s).\n", linea, $1.lexema);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;			
		}

		/*Comprobacion para no pasar como parametro una funcion*/
		if(en_explist == 1){
			fprintf(stderr,"****Error semantico en lin %d: No esta permitido el uso de llamadas a funciones como parametros de otras funciones.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		/*Seteo esta variable a 0 para contar el numero de argumentos*/
		num_parametros_llamada_actual = 0;
		/*Comienza a recibir parametros, levantamos el flag*/
		en_explist = 1;
		/*Propagacion del lexema*/
		strcpy($$.lexema, $1.lexema);
	}
	;

lista_expresiones: exp resto_lista_expresiones	{
		fprintf(yyout, ";R89:\t<lista_expresiones> ::= <exp> <resto_lista_expresiones>\n");
		num_parametros_llamada_actual++;
	}
	|	/*Vacia*/	{
		fprintf(yyout, ";R90:\t<lista_expresiones> ::=\n");
	}
	;

resto_lista_expresiones: TOK_COMA exp resto_lista_expresiones	{
		fprintf(yyout, ";R91:\t<resto_lista_expresiones> ::= ; <exp> <resto_lista_expresiones>\n");
		num_parametros_llamada_actual++;

		/*Generacion codigo*/
		if($2.es_direccion == 1){
			traducir_direccion_valor(ensamblador);
		}

	}
	|	/*Vacia*/	{
		fprintf(yyout, ";R92:\t<resto_lista_expresiones> ::=\n");
	}
	;

comparacion: exp TOK_IGUAL exp	{

		fprintf(yyout, ";R93:\t<comparacion> ::= <exp> == <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Comparacion con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		$$.tipo = BOOLEANO;
		$$.es_direccion = 0;

		/*Generacion Codigo*/
		igual(ensamblador, $1.es_direccion, $3.es_direccion);
	}
	|	exp TOK_DISTINTO exp	{

		fprintf(yyout, ";R94:\t<comparacion> ::= <exp> != <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Comparacion con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		$$.tipo = BOOLEANO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		distinto(ensamblador, $1.es_direccion, $3.es_direccion);
	}
	|	exp TOK_MENORIGUAL exp	{

		fprintf(yyout, ";R95:\t<comparacion> ::= <exp> <= <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Comparacion con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		$$.tipo = BOOLEANO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		menor_igual(ensamblador, $1.es_direccion, $3.es_direccion);
	}
	|	exp TOK_MAYORIGUAL exp	{

		fprintf(yyout, ";R96:\t<comparacion> ::= <exp> >= <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Comparacion con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		$$.tipo = BOOLEANO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		mayor_igual(ensamblador, $1.es_direccion, $3.es_direccion);
	}
	|	exp TOK_MENOR exp	{

		fprintf(yyout, ";R97:\t<comparacion> ::= <exp> < <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Comparacion con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		$$.tipo = BOOLEANO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		menor(ensamblador, $1.es_direccion, $3.es_direccion);
	}
	|	exp TOK_MAYOR exp	{

		fprintf(yyout, ";R98:\t<comparacion> ::= <exp> > <exp>\n");
		if($1.tipo != ENTERO || $3.tipo != ENTERO){
			fprintf(stderr,"****Error semantico en lin %d: Comparacion con operandos boolean.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}

		$$.tipo = BOOLEANO;
		$$.es_direccion =0;

		/*Generacion Codigo*/
		mayor(ensamblador, $1.es_direccion, $3.es_direccion);
	}
	;

constante: constante_logica	{
		fprintf(yyout, ";R99:\t<constante> ::= <constante_logica>\n");
		$$.tipo = $1.tipo;
		$$.es_direccion = $1.es_direccion;
	}
	|	constante_entera	{
		fprintf(yyout, ";R100:\t<constante> ::= <constante_entera>\n");
		$$.tipo = $1.tipo;
		$$.es_direccion = $1.es_direccion;

		/*Generacion codigo*/
		escribir_constante_entera(ensamblador, $1.valor_entero);
	}
	;

constante_logica: TOK_TRUE	{
		fprintf(yyout, ";R102:\t<constante_logica> ::= true\n");
		$$.tipo = BOOLEANO;
		$$.es_direccion = 0;

		/*Generacion Codigo*/
		escribir_constante_booleana(ensamblador, 1);
	}
	|	TOK_FALSE	{
		fprintf(yyout, ";R103:\t<constante_logica> ::= false\n");
		$$.tipo = BOOLEANO;
		$$.es_direccion = 0;

		/*Generacion Codigo*/
		escribir_constante_booleana(ensamblador, 0);
	}
	;

constante_entera: TOK_CONSTANTE_ENTERA	{
		fprintf(yyout, ";R104:\t<constante_entera> ::= TOK_CONSTANTE_ENTERA\n");
		$$.tipo = ENTERO;
		$$.es_direccion = 0;
	}
	;

identificador: TOK_IDENTIFICADOR	{
		fprintf(yyout, ";R108:\t<identificador> ::= TOK_IDENTIFICADOR\n");

		if(clase == VECTOR){
			if((tamanio_vector_actual < 1 ) || (tamanio_vector_actual > MAX_TAMANIO_VECTOR)){
				fprintf(stderr,"****Error semantico en lin %d: El tamanyo del vector %s excede los limites permitidos (1,64).\n", linea, $1.lexema);
				sem_err = 1;
				yyerror();
				return EXIT_FAILURE;
			}
		}

		if(buscar_tabla_simbolo(ts, $1.lexema) == TRUE){
			fprintf(stderr,"****Error semantico en lin %d: Declaracion duplicada.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}else{
			if(ambito_actual(ts) == GLOBAL){
				if(clase != VECTOR)
					tamanio_vector_actual = 1;

				declaracion_variable(ts, $1.lexema, tipo, clase, tamanio_vector_actual, 0);
				/*Generacion codigo*/
				declarar_variable(ensamblador, $1.lexema, clase, tamanio_vector_actual);
			}else{
				if(clase != ESCALAR){
					fprintf(stderr,"****Error semantico en lin %d: Variable local de tipo no escalar.\n",linea);
					sem_err = 1;
					yyerror();
					return EXIT_FAILURE;
				}
				declaracion_variable(ts, $1.lexema, tipo, clase, tamanio_vector_actual, pos_variable_local_actual);
				pos_variable_local_actual++;
				num_variables_locales_actual++;
			}
		}

	}
	;

idpf:	TOK_IDENTIFICADOR {	fprintf(yyout,";R108.2:\t<idpf> ::= TOK_IDENTIFICADOR\n");
		/*Buscamos en el ambito actual*/
		if(buscar_tabla_simbolo(ts, $1.lexema) == TRUE){
			fprintf(stderr,"****Error semantico en lin %d: Declaracion duplicada.\n", linea);
			sem_err = 1;
			yyerror();
			return EXIT_FAILURE;
		}else{
			/*Insercion sin olvidar pos_parametro_actual*/
			declaracion_parametro(ts, $1.lexema, tipo, ESCALAR, 0, pos_parametro_actual);
			pos_parametro_actual++;
			num_parametros_actual++;
		}
	}
	;

%%
	int yyerror(){

		if(morf_err == 0 && sem_err == 0){
			fprintf(stderr, "****Error sintactico en [lin %d, col %d]\n", linea, columna - yyleng);
		}

		borrar_tabla_simbolos(ts);
		return EXIT_FAILURE;
	}

/*******************************
  Alfonso Bonilla Trueba
  Pareja 11 - Grupo 1311
*******************************/

#include "../includes/generacionCodigo.h"
#include <string.h>

/*Contadores globales para generar etiquetas diferentes*/
int cont_mayor = 0;
int cont_menor = 0;
int cont_igualdad = 0;
int cont_not = 0;


/******************************* SEGMENTOS ***********************************/

void escribir_cabecera_bss(FILE* f){
	fprintf(f, "\nsegment .bss\n");
	fprintf(f, "\t_pila resd 1\n");
}

void escribir_seccion_data(FILE* f){
	fprintf(f, ";ASM GENERADO POR EL COMPILADOR DE ALFONSO BONILLA TRUEBA\n\n");
	fprintf(f, "segment .data\n");
	fprintf(f, "\t_mensaje_1 dw \"****Error de ejecucion: Division por cero.\",0\n");
	fprintf(f, "\t_mensaje_2 dw \"****Error de ejecucion: Indice fuera de rango.\",0\n");
}

void declarar_variable(FILE* f, char * nombre,  TIPO tipo,  int cantidad){
	switch(tipo){
		case ENTERO:
			fprintf(f, "\t;Integer %s\n"
			"\t_%s resd %d\n",nombre,nombre,cantidad);
			break;
		case BOOLEANO:
			fprintf(f, "\t;Boolean %s\n"
			"\t_%s resd %d\n",nombre,nombre,cantidad);
			break;
		default:
			fprintf(stderr, "****Error, tipo desconocido");
			break;
		}
	return;
}

void escribir_segmento_codigo(FILE* f){
	fprintf(f, "\nsegment .text\n");
	fprintf(f, "\tglobal main\n");
	fprintf(f, "\textern scan_int, scan_boolean, print_int, print_boolean, print_blank, print_endofline, print_string\n");
}

void escribir_inicio_main(FILE* f){
	fprintf(f, "\nmain:\n");
	fprintf(f, "\tmov dword [_pila],esp\n");
}

void escribir_fin(FILE* f){

	fprintf(f, "fin:\n");
	fprintf(f, "\tmov dword esp, [_pila]\n");
	fprintf(f, "\tret\n");
	fprintf(f, "error_1: push dword _mensaje_1\n");
	fprintf(f, "\tcall print_string\n");
	fprintf(f, "\tadd esp, 4\n");
	fprintf(f,"\tcall print_endofline\n");
	fprintf(f, "\tjmp near fin\n");
	fprintf(f, "error_2: push dword _mensaje_2\n");
	fprintf(f, "\tcall print_string\n");
	fprintf(f, "\tadd esp, 4\n");
	fprintf(f,"\tcall print_endofline\n");
	fprintf(f, "\tjmp near fin\n");
}

/*************************************************************************************/

void cambiar_signo(FILE * f, int es_direccion_op1){

	fprintf(f, "\t;Cambio de Signo\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tneg eax\n");
	fprintf(f, "\tpush dword eax\n");

	return;
}

void suma_enteros(FILE* f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Suma de enteros\n");
	fprintf(f, "\tpop dword edx\n");

	if (es_direccion_op2 == 1)
		fprintf(f, "\tmov dword edx , [edx]\n");

	fprintf(f, "\t; cargar el primer operando en eax\n");
	fprintf(f,"\tpop dword eax\n");

	if (es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax , [eax]\n");

	fprintf(f, "\t; realizar la suma y dejar el resultado en eax \n");
	fprintf(f, "\tadd eax,edx\n");
	fprintf(f, "\t; apilar el resultado\n");
	fprintf(f, "\tpush dword eax\n");

	return;
}

void resta_enteros(FILE* f, int es_direccion_op1, int es_direccion_op2){
	fprintf(f, "\t;Resta de enteros\n");
	fprintf(f, "\tpop dword edx\n");

	if (es_direccion_op2 == 1)
		fprintf(f, "\tmov dword edx , [edx]\n");

	fprintf(f, "\t; cargar el primer operando en eax\n");
	fprintf(f,"\tpop dword eax\n");

	if (es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax , [eax]\n");

	fprintf(f, "\t; realizar la resta y dejar el resultado en eax \n");
	fprintf(f, "\tsub eax,edx\n");
	fprintf(f, "\t; apilar el resultado\n");
	fprintf(f, "\tpush dword eax\n");
	return;
}

void division_enteros(FILE* f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Division de enteros\n");
	fprintf(f, "\tpop dword ebx\n");

	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\tcmp ebx, 0\n");
	fprintf(f, "\tje error_1\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tcdq\n");
	fprintf(f, "\tidiv ebx\n");
	fprintf(f, "\tpush dword eax\n");

	return;
}

void multipliacion_enteros(FILE* f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Multiplicacion de enteros\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\timul eax, ebx\n");
	fprintf(f, "\tpush dword eax\n");

	return;
}

void mayor(FILE * f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f,"\t;Comparacion Mayor\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");

	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\tcmp eax, ebx\n");
	fprintf(f, "\tjg near mayor_%d\n", cont_mayor);
	fprintf(f, "\tmov dword eax, 0\n");
	fprintf(f, "\tjmp near fin_mayor_%d\n", cont_mayor);
	fprintf(f, "mayor_%d:\n", cont_mayor);
	fprintf(f, "\tmov dword eax, 1\n");
	fprintf(f, "fin_mayor_%d:\n", cont_mayor);
	fprintf(f, "\tpush dword eax\n");

	cont_mayor++;/*Aumento uno para evitar conflictos*/

	return;
}

void mayor_igual(FILE * f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Comparacion Mayor Igual\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\tcmp eax, ebx\n");
	fprintf(f, "\tjge near mayor_igual_%d\n", cont_mayor);
	fprintf(f, "\tmov dword eax, 0\n");
	fprintf(f, "\tjmp near fin_mayor_igual_%d\n", cont_mayor);
	fprintf(f, "mayor_igual_%d:\n", cont_mayor);
	fprintf(f, "\tmov dword eax, 1\n");
	fprintf(f, "fin_mayor_igual_%d:\n", cont_mayor);
	fprintf(f, "\tpush dword eax\n");

	cont_mayor++;/*Aumento uno para evitar conflictos*/

	return;
}

void menor(FILE * f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Comparacion Menor\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\tcmp eax, ebx\n");
	fprintf(f, "\tjl near menor_%d\n", cont_menor);
	fprintf(f, "\tmov dword eax, 0\n");
	fprintf(f, "\tjmp near fin_menor_%d\n", cont_menor);
	fprintf(f, "menor_%d:\n", cont_menor);
	fprintf(f, "\tmov dword eax, 1\n");
	fprintf(f, "fin_menor_%d:\n", cont_menor);
	fprintf(f, "\tpush dword eax\n");

	cont_menor++;/*Aumento uno para evitar conflictos*/

	return;
}

void menor_igual(FILE * f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Comparacion Menor Igual\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\tcmp eax, ebx\n");
	fprintf(f, "\tjle near menor_igual_%d\n", cont_menor);
	fprintf(f, "\tmov dword eax, 0\n");
	fprintf(f, "\tjmp near fin_menor_igual_%d\n", cont_menor);
	fprintf(f, "menor_igual_%d:\n", cont_menor);
	fprintf(f, "\tmov dword eax, 1\n");
	fprintf(f, "fin_menor_igual_%d:\n", cont_menor);
	fprintf(f, "\tpush dword eax\n");

	cont_menor++;/*Aumento uno para evitar conflictos*/

	return;
}

void igual(FILE * f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Igualdad de enteros\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");

	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");

	fprintf(f, "\tcmp eax, ebx\n");
	fprintf(f, "\tje near igual_%d\n", cont_igualdad);
	fprintf(f, "\tmov dword eax, 0\n");
	fprintf(f, "\tjmp near fin_igual_%d\n", cont_igualdad);
	fprintf(f, "igual_%d:\n", cont_igualdad);
	fprintf(f, "\tmov dword eax, 1\n");
	fprintf(f, "fin_igual_%d:\n", cont_igualdad);
	fprintf(f, "\tpush dword eax\n");
	
	cont_igualdad++;/*Aumento uno para evitar conflictos*/

	return;
}

void distinto(FILE * f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Desigualdad de enteros\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\tcmp eax, ebx\n");
	fprintf(f, "\tjne near desigual_%d\n", cont_igualdad);
	fprintf(f, "\tmov dword eax, 0\n");
	fprintf(f, "\tjmp near fin_desigual_%d\n", cont_igualdad);
	fprintf(f, "desigual_%d:\n", cont_igualdad);
	fprintf(f, "\tmov dword eax, 1\n");
	fprintf(f, "fin_desigual_%d:\n", cont_igualdad);
	fprintf(f, "\tpush dword eax\n");

	cont_igualdad++;/*Aumento uno para evitar conflictos*/

	return;
}


/************************* OPERACIONES LOGICAS ***************************/

void bool_and(FILE * f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Operacion AND\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\tand eax, ebx\n");
	fprintf(f, "\tpush dword eax\n");

	return;
} 

void bool_or(FILE * f, int es_direccion_op1, int es_direccion_op2){

	fprintf(f, "\t;Operacion OR\n");
	fprintf(f, "\tpop dword ebx\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	if(es_direccion_op2 == 1)
		fprintf(f, "\tmov dword ebx, [ebx]\n");
	
	fprintf(f, "\tor eax, ebx\n");
	fprintf(f, "\tpush dword eax\n");

	return;
} 

void bool_not(FILE * f, int es_direccion_op1){

	fprintf(f, "\t;Operacion NOT\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion_op1 == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tor eax, eax\n");
	fprintf(f, "\tjz near negar_falso_%d\n", cont_not);
	fprintf(f, "\tmov dword eax, 0\n");
	fprintf(f, "\tjmp near fin_negacion_%d\n", cont_not);
	fprintf(f, "negar_falso_%d:\n", cont_not);
	fprintf(f, "\tmov dword eax, 1\n");
	fprintf(f, "fin_negacion_%d:\n", cont_not);
	fprintf(f, "\tpush dword eax\n");

	cont_not++;/*Aumento uno para evitar conflictos*/

	return;
}


/***************************  CONDICIONALES ******************************/

void inicio_if(FILE *f, int es_direccion, int contador){
	fprintf(f, "\t;Inicio IF %d\n", contador);
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tcmp eax, 0\n");
	fprintf(f, "\tje near fin_si%d\n", contador);

	return;
}

void if_else(FILE *f, int contador){
	fprintf(f, ";Medio de la condicional %d\n", contador);
	fprintf(f, "\tjmp near fin_sino%d\n", contador);
	fprintf(f, "fin_si%d:\n", contador);
	
	return;
}

void fin_condicional(FILE *f, int contador){
	fprintf(f, ";Fin IF %d\n", contador);
	fprintf(f, "fin_si%d:\n", contador);

	return;
}

void fin_condicional_completo(FILE *f, int contador){
	fprintf(f, ";Fin IF %d\n", contador);
	fprintf(f, "fin_sino%d:\n", contador);

	return;
}

void inicio_while(FILE *f, int contador){
	fprintf(f, ";WHILE %d\n", contador);
	fprintf(f, "inicio_while%d:\n", contador);

	return;
}

void comprobacion_while(FILE *f, int es_direccion, int contador){

	fprintf(f, "\t;COMPROBACION WHILE %d\n", contador);
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tcmp eax, 0\n");
	fprintf(f, "\tje near fin_while%d\n", contador);

	return;
}

void fin_while(FILE *f, int contador){
	fprintf(f,";END WHILE%d\n", contador);
	fprintf(f, "\tjmp near inicio_while%d\n", contador);
	fprintf(f, "fin_while%d:\n", contador);

	return;
}


/*********************** ASIGNACIONES **********************************/

void asignar_variable(FILE *f, char *nombre, int es_direccion){

	fprintf(f, "\t;asignacion de una variable\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tmov dword [_%s], eax\n", nombre);

	return;
}

void asignar_array(FILE *f, int es_direccion){
	fprintf(f, "\t;Asignacion elemento vector\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tpop dword edx\n");
	fprintf(f, "\tmov dword [edx], eax\n");

	return;
}

void asignar_parametro(FILE *f, int posicion, int num_args, int es_direccion){

	fprintf(f, "\t;Asignacion de una variable global\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tmov dword [ebp + %d], eax\n", 4 + 4*(num_args - posicion));

	return;
}

void asignar_variable_local(FILE *f, int posicion, int es_direccion){

	fprintf(f, "\t;Asignacion de una variable global\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tmov dword [ebp - %d], eax\n", 4*posicion);

	return;
}


/*********************** ESCRITURAS **********************************/

void escribir_constante_booleana(FILE *f, int bool){
	if(bool == 1){
		fprintf(f, ";Constante TRUE\n");
		fprintf(f, "\tpush dword 1\n");
	}else{
		fprintf(f, ";Constante FALSE\n");
		fprintf(f, "\tpush dword 0\n");
	}
	return;
}

void escribir_constante_entera(FILE *f, int num){
	fprintf(f, ";Constante ENTERA\n");
	fprintf(f, "\tpush dword %d\n", num);
	return;
}

void apilar_direccion_variable(FILE *f, char *nombre, int en_explist){
	fprintf(f, ";cargo variable %s\n", nombre);

	if(en_explist == 1)
		fprintf(f, "\tpush dword [_%s]\n", nombre);
	else
		fprintf(f, "\tpush dword _%s\n", nombre);

	return;
}

void escribir_elemento_vector(FILE *f, char * nombre, int es_direccion, int tam_max, int en_explist){

	fprintf(f, "\t;escritura elemento de vector\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion == 1)
		fprintf(f, "\tmov dword eax, [eax]\n");
	
	fprintf(f, "\tcmp eax, 0\n");
	fprintf(f, "\tjl near error_2\n");
	fprintf(f, "\tcmp eax, %d\n", tam_max - 1);
	fprintf(f, "\tjg near error_2\n");
	fprintf(f, "\tmov dword edx, _%s\n", nombre);

	if(en_explist == 1)
		fprintf(f, "\tpush dword [edx + eax*4]\n");
	else {
		fprintf(f, "\tlea eax, [edx + eax*4]\n");
		fprintf(f, "\tpush dword eax\n");
	}

	return;
}


/***************************** FUNCIONES *********************************/

void declarar_funcion(FILE *f, char *name, int num_local_var){
	fprintf(f, ";Funcion %s\n", name);
	fprintf(f, "_%s:\n", name);
	fprintf(f, "\tpush ebp\n");
	fprintf(f, "\tmov ebp, esp\n");
	fprintf(f, "\tsub esp, %d\n", num_local_var*4);

	return;
}

void fin_funcion(FILE *f, int es_direccion){
	fprintf(f, ";return de la funcion\n");
	fprintf(f, "\tpop dword eax\n");

	if(es_direccion == 1)
		fprintf(f, "\tmov eax, [eax]\n");

	fprintf(f, "\tmov esp, ebp\n");
	fprintf(f, "\tpop ebp\n");
	fprintf(f, "ret\n");

	return;
}

void llamada_funcion(FILE *f, char *name, int num_params){
	fprintf(f, ";Llamada a funcion\n");
	fprintf(f, "\tcall _%s\n", name);
	fprintf(f, "\tadd esp, %d\n", num_params*4);
	fprintf(f, "\tpush dword eax\n");

	return;
}

void apilar_direccion_variable_local(FILE *f, int posicion, int en_explist){

	fprintf(f, ";Cargamos variable local\n");

	if(en_explist == 1)
		fprintf(f, "\tpush dword [ebp - %d]\n", 4*posicion);
	else{
		fprintf(f, "\tlea eax, [ebp - %d]\n", 4*posicion);
		fprintf(f, "\tpush dword eax\n");
	}

	return;
}

void apilar_direccion_parametro(FILE *f, int posicion, int num_args, int en_explist){

	fprintf(f, ";Cargar varible parametro\n");

	if(en_explist == 1){
		fprintf(f, "\tpush dword [ebp + %d]\n", 4 + 4*(num_args - posicion));
	}else {
		fprintf(f, "\tlea eax, [ebp + %d]\n", 4 + 4*(num_args - posicion));
		fprintf(f, "\tpush dword eax\n");
	}

	return;
}

void traducir_direccion_valor(FILE *f){
	fprintf(f, "\t;Pasar de direccion a valor\n");
	fprintf(f, "\tmov dword eax, [eax]\n");

	return;
}

void leer_parametro(FILE * f, int posicion, int num_args, TIPO tipo){
	fprintf(f, "\t;Scanf\n");
	fprintf(f, "\tlea eax, [ebp + %d]\n", 4+4*(num_args - posicion));
	fprintf(f, "\tpush dword eax\n");

	if(tipo == ENTERO)
		fprintf(f, "\tcall scan_int\n");
	else if(tipo == BOOLEANO)
		fprintf(f, "\tcall scan_boolean\n");

	fprintf(f, "\tadd esp,4\n");

	return;
}

void leer_variable_local(FILE * f, int posicion, TIPO tipo){
	fprintf(f, "\t;Scanf local\n");
	fprintf(f, "\tlea eax, [ebp - %d]\n", 4*posicion);
	fprintf(f, "\tpush dword eax\n");

	if(tipo == ENTERO)
		fprintf(f, "\tcall scan_int\n");
	else if(tipo == BOOLEANO)
		fprintf(f, "\tcall scan_boolean\n");

	fprintf(f, "\tadd esp,4\n");

	return;
}

/********************************************************************/




void print(FILE * f, int es_direccion, TIPO tipo){

	fprintf(f,";Imprimir variable\n");

	if(es_direccion == 1){
		fprintf(f, "\tpop dword eax\n");
		fprintf(f, "\tmov dword eax, [eax]\n");
		fprintf(f, "\tpush dword eax\n");
	}

	if(tipo == ENTERO)
		fprintf(f, "\tcall print_int\n");
	else if(tipo == BOOLEANO)
		fprintf(f, "\tcall print_boolean\n");

	fprintf(f, "\tadd esp, 4\n");
	fprintf(f, "\tcall print_endofline\n");
}

void leer_variable(FILE * f, char * var, TIPO tipo){

	fprintf(f, "\t;Scanf %s\n", var);
	fprintf(f, "\tpush dword _%s\n", var);
	if(tipo == ENTERO)
		fprintf(f, "\tcall scan_int\n");
	else if(tipo == BOOLEANO)
		fprintf(f, "\tcall scan_boolean\n");
	fprintf(f, "\tadd esp,4\n");

	return;
}

void salto_linea(FILE *f){
	fprintf(f,"\tcall print_endofline\n");
}
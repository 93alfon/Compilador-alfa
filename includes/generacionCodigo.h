/*******************************
  Alfonso Bonilla Trueba
  Pareja 11 - Grupo 1311
*******************************/

#ifndef GENERACIONCODIGO_H
#define GENERACIONCODIGO_H
#include <stdio.h>
#include "alfa.h"


/***************************** SEGMENTOS ********************************/
void escribir_cabecera_bss(FILE* f);
void escribir_seccion_data(FILE* f);
void declarar_variable(FILE* f, char * nombre,  TIPO tipo,  int cantidad);
void escribir_segmento_codigo(FILE* f);
void escribir_inicio_main(FILE* f);
void escribir_fin(FILE* f);


/************************* OPERACIONES ENTEROS ***************************/
void cambiar_signo(FILE * f, int es_direccion_op1);
void suma_enteros(FILE* f, int es_direccion_op1, int es_direccion_op2);
void resta_enteros(FILE* f, int es_direccion_op1, int es_direccion_op2);
void division_enteros(FILE* f, int es_direccion_op1, int es_direccion_op2);
void multipliacion_enteros(FILE* f, int es_direccion_op1, int es_direccion_op2);
void mayor(FILE* f, int es_direccion_op1, int es_direccion_op2);
void mayor_igual(FILE* f, int es_direccion_op1, int es_direccion_op2);
void menor(FILE* f, int es_direccion_op1, int es_direccion_op2);
void menor_igual(FILE* f, int es_direccion_op1, int es_direccion_op2);
void igual(FILE* f, int es_direccion_op1, int es_direccion_op2);
void distinto(FILE* f, int es_direccion_op1, int es_direccion_op2);


/***************************  CONDICIONALES ******************************/
void inicio_if(FILE *f, int es_direccion, int contador);
void if_else(FILE *f, int contador);
void fin_condicional(FILE *f, int contador);
void fin_condicional_completo(FILE *f, int contador);
void inicio_while(FILE *f, int contador);
void comprobacion_while(FILE *f, int es_direccion, int contador);
void fin_while(FILE *f, int contador);


/************************* OPERACIONES LOGICAS ***************************/
void bool_and(FILE * f, int es_direccion_op1, int es_direccion_op2);
void bool_or(FILE * f, int es_direccion_op1, int es_direccion_op2);
void bool_not(FILE * f, int es_direccion_op1);


/**************************** ASIGNACIONES *******************************/
void asignar_variable(FILE *f, char *nombre, int es_direccion);
void asignar_array(FILE *f, int es_direccion);
void asignar_parametro(FILE *f, int posicion, int num_args, int es_direccion);
void asignar_variable_local(FILE *f, int posicion, int es_direccion);

/***************************** ESCRITURAS ********************************/
void escribir_constante_booleana(FILE *f, int bool);
void escribir_constante_entera(FILE *f, int num);
void apilar_direccion_variable(FILE *f, char *nombre, int en_explist);
void escribir_elemento_vector(FILE *f, char * nombre, int es_direccion, int tam_max, int en_explist);

/***************************** FUNCIONES *********************************/
void declarar_funcion(FILE *f, char *name, int num_local_var);
void fin_funcion(FILE *f, int es_direccion);
void llamada_funcion(FILE *f, char *name, int num_params);
void escribir_parametro(FILE *f, char *nombre, int es_direccion);
void traducir_direccion_valor(FILE *f);
void leer_parametro(FILE * f, int posicion, int num_args, TIPO tipo);
void leer_variable_local(FILE * f, int posicion, TIPO tipo);
void apilar_direccion_variable_local(FILE *f, int posicion, int en_explist);
void apilar_direccion_parametro(FILE *f, int posicion, int num_args, int en_explist);

/*************************************************************************/

void print(FILE * f, int es_direccion, TIPO tipo);
void leer_variable(FILE * f, char * var, TIPO tipo);
void salto_linea(FILE *f);


#endif
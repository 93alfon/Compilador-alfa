/*******************************
  Alfonso Bonilla Trueba
  Pareja 11 - Grupo 1311
*******************************/

#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

#include "tablaHash.h"
#include "alfa.h"
#include <stdio.h>
#include <stdlib.h>

#define	TAM_TAB_SIMB	163

typedef enum { GLOBAL, LOCAL } AMBITO;
typedef enum { FALSE = 0, TRUE = 1 } BOOL;

typedef struct {
	TABLA_HASH * global;
	TABLA_HASH * local;
} TABLA_SIMBOLOS;


/******************** CREACION DESTRUCCION ***************************/
TABLA_SIMBOLOS *crear_tabla_simbolos();
void borrar_tabla_simbolos(TABLA_SIMBOLOS* tabla);

/*********************** DECLARACIONES *******************************/
STATUS declaracion_variable(TABLA_SIMBOLOS* tabla, char* name, TIPO tipo, CLASE clase, int value, int posicion);
STATUS declaracion_parametro(TABLA_SIMBOLOS* tabla, char* name, TIPO tipo, CLASE clase, int value, int posicion);
STATUS declaracion_funcion(TABLA_SIMBOLOS* tabla, char* name, TIPO tipo);
STATUS cierre_ambito(TABLA_SIMBOLOS* tabla);

/********************* BUSQUEDAS - GET *******************************/
BOOL buscar_tabla_simbolo(TABLA_SIMBOLOS *tabla, char* name);
CATEGORIA get_categoria_tabla_simbolo(TABLA_SIMBOLOS *tabla, char* name);
TIPO get_tipo_tabla_simbolo(TABLA_SIMBOLOS *tabla, char* name);
CLASE get_clase_tabla_simbolo(TABLA_SIMBOLOS *tabla, char* name);
int get_num_params_funcion(TABLA_SIMBOLOS *tabla, char *name);
int get_tam_vector(TABLA_SIMBOLOS *tabla, char *name);
int get_posicion(TABLA_SIMBOLOS *tabla, char *name);
AMBITO ambito_actual(TABLA_SIMBOLOS *tabla);
AMBITO ambito_variable(TABLA_SIMBOLOS *tabla, char *name);

/*********************** MODIFICACIONES *******************************/
void modificar_valores_funcion(TABLA_SIMBOLOS *tabla, char* name, int n_param, int n_vars);

#endif
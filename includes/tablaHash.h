/* 
 * Fichero: tablaHash.h
 * Autor: Equipo docente PAUTLEN
 * Curso: 2016-17
 */

#ifndef TABLAHASH_H
#define TABLAHASH_H

#include <stdio.h>
#include "alfa.h"

/**************** CONSTANTES ****************/

#define POS_INI_PARAMS 0        /* posición de inicio de parámetros de función (empiezan a contar en 0) */
#define POS_INI_VARS_LOCALES 1  /* posición de inicio de variables locales de función (empiezan a contar en 1) */

#define HASH_INI 5381
#define HASH_FACTOR 33


/**************** DECLARACIONES DE TIPOS ****************/

/* Retorno de función error/ok */
typedef enum { ERR = 0, OK = 1 } STATUS;


/* Información de un símbolo */
typedef struct {
    char *lexema;           /* identificador */
    CATEGORIA categoria;    /* categoría */    
    TIPO tipo;              /* tipo */
    CLASE clase;            /* clase */
    int adicional1;    /* valor si escalar, longitud si vector, número de parámetros si función */
    int adicional2;    /* posición en llamada a función si parámetro, posición de declaración si variable local de función, número de variables locales si función */
} INFO_SIMBOLO;

/* Nodo de la tabla hash */
typedef struct nodo_hash {
    INFO_SIMBOLO *info;      /* información del símbolo */
    struct nodo_hash *siguiente;    /* puntero al siguiente nodo (encadenamiento si colisión en misma celda) */
} NODO_HASH;

/* Tabla hash */
typedef struct {
    int tam;            /* tamaño de la tabla hash */
    NODO_HASH **tabla;  /* tabla en sí (array de tam punteros a nodo) */
} TABLA_HASH;


/**************** FUNCIONES ****************/

INFO_SIMBOLO *crear_info_simbolo(const char *lexema, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2);
void liberar_info_simbolo(INFO_SIMBOLO *is);
NODO_HASH *crear_nodo(INFO_SIMBOLO *is);
void liberar_nodo(NODO_HASH *nh);
TABLA_HASH *crear_tabla(int tam);
void liberar_tabla(TABLA_HASH *th);
unsigned long hash(const char *str);
INFO_SIMBOLO *buscar_simbolo(const TABLA_HASH *th, const char *lexema);
STATUS insertar_simbolo(TABLA_HASH *th, const char *lexema, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2);
void borrar_simbolo(TABLA_HASH *th, const char *lexema);

#endif
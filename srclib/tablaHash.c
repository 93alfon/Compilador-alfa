/* 
 * Fichero: tablaHash.c
 * Autor: Equipo docente PAUTLEN
 * Curso: 2016-17
 */
#define _GNU_SOURCE 1
#include <stdlib.h>
#include <string.h>
#include "../includes/tablaHash.h"

/* 
 * Crea una estructura INFO_SIMBOLO a partir de los datos pasados.
 * La memoria para el lexema se duplica, con lo cual el lexema pasado se puede reasignar fuera de la función sin cambiar la estructura.
 * 
 * Entrada:
 *      lexema: identificador del símbolo.
 *      categ: categoría del símbolo.
 *      tipo: tipo del símbolo.
 *      clase: clase del símbolo.
 *      adic1: valor adicional del símbolo (dependiente de lo anterior, consultar tablaHash.h)
 *      adic2: valor adicional del símbolo (dependiente de lo anterior, consultar tablaHash.h)
 *
 * Salida:
 *      INFO_SIMBOLO *: puntero a la estructura reservada, NULL si se produce algún error.
 */
INFO_SIMBOLO *crear_info_simbolo(const char *lexema, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2) {
    INFO_SIMBOLO *is;

    if ((is = (INFO_SIMBOLO *) malloc(sizeof(INFO_SIMBOLO)))) {
        /* Duplicar lexema */
        if (!(is->lexema = strdup(lexema))) {
            free(is);
            return NULL;
        }
        is->categoria = categ;
        is->tipo = tipo;
        is->clase = clase;
        is->adicional1 = adic1;
        is->adicional2 = adic2;
    }
    return is;
}

/* 
 * Libera una estructura INFO_SIMBOLO.
 * La memoria del lexema se libera también.
 *
 * Entrada:
 *      is: estructura INFO_SIMBOLO a liberar.
 *
 * Salida: ninguna.
 */
void liberar_info_simbolo(INFO_SIMBOLO *is) {
    if (is) {
        if (is->lexema) {
            free(is->lexema);
        }
        free(is);
    }
}

/* 
 * Crea una estructura NODO_HASH.
 * La información del símbolo se asume ya reservada con crear_info_simbolo y sólo se copia, no se duplica.
 *
 * Entrada:
 *      is: estructura INFO_SIMBOLO con la información del símbolo.
 *
 * Salida:
 *      NODO_HASH *: puntero al nodo reservada, NULL si se produce algún error.
 */
NODO_HASH *crear_nodo(INFO_SIMBOLO *is) {
    NODO_HASH *nh;

    if ((nh = (NODO_HASH *) malloc(sizeof(NODO_HASH)))) {
        nh->info = is;
        nh->siguiente = NULL;   /* no hay siguiente */
    }
    return nh;
}

/* 
 * Libera una estructura NODO_HASH.
 * La información del símbolo se libera también.
 *
 * Entrada:
 *      nh: estructura NODO_HASH a liberar.
 *
 * Salida: ninguna.
 */
void liberar_nodo(NODO_HASH *nh) {
    if (nh) {
        liberar_info_simbolo(nh->info);
        free(nh);
    }
}

/* 
 * Crea una tabla hash.
 * La tabla hash consiste en un array de punteros a NODO_HASH del tamaño especificado.
 *
 * Entrada:
 *      tam: tamaño de la tabla hash a crear.
 *
 * Salida:
 *      TABLA_HASH *: puntero a la tabla hash creada, NULL si se produce algún error.
 */
TABLA_HASH *crear_tabla(int tam) {
    TABLA_HASH *th;

    /* Reservar estructura */
    if ((th = (TABLA_HASH *) malloc(sizeof(TABLA_HASH)))) {
        /* Reservar tabla en sí */
        if (!(th->tabla = (NODO_HASH **) calloc(tam, sizeof(NODO_HASH *)))) {    /* todos los punteros a nodo a NULL */
            free(th); 
            return NULL;
        }
        th->tam = tam;
    }
    return th;
}

/* 
 * Libera una tabla hash.
 * Esto a su vez implica liberar todos los nodos apuntados directa o indirectamente por la tabla, 
 * así como la información de todos los símbolos.
 *
 * Entrada:
 *      th: tabla hash a liberar.
 *
 * Salida: ninguna.
 */
void liberar_tabla(TABLA_HASH *th) {
    int i;
    NODO_HASH *n1, *n2;
    
    if (th) {
        if (th->tabla) {
            /* Recorrer punteros */
            for (i = 0; i < th->tam; i++) {
                n1 = th->tabla[i];
                /* Liberar lista enlazada de cada uno */
                while (n1) {
                    n2 = n1->siguiente;
                    liberar_nodo(n1);
                    n1 = n2;      
                }
            }
            free(th->tabla);
        }
        free(th);
    }
}

/* 
 * Implementa una función hash multiplicativa para cadenas.
 *
 * Entrada:
 *      str: cadena de la que calcular el hash.
 *
 * Salida:
 *      unsigned long: hash calculado para la cadena.
 */
unsigned long hash(const char *str) {
    unsigned long h = HASH_INI;
    unsigned char *p;

    for (p = (unsigned char *) str; *p; p++) {
        h = h*HASH_FACTOR + *p;
    }
    return h;
}

/* 
 * Busca un símbolo en la tabla hash.
 * 
 * Entrada:
 *      th: tabla hash donde buscar.
 *      lexema: identificador del símbolo.
 *
 * Salida: 
 *      INFO_SIMBOLO *: puntero a la información del símbolo, NULL si el símbolo no se encuentra.
 */
INFO_SIMBOLO *buscar_simbolo(const TABLA_HASH *th, const char *lexema) {
    unsigned int ind;    
    NODO_HASH *n;
        
    /* Calcular posición */
    ind = hash(lexema) % th->tam;
    /* Buscar en lista enlazada */
    n = th->tabla[ind];
    while (n && (!n->info || strcmp(n->info->lexema, lexema))) {
        n = n->siguiente;
    }
    return n ? n->info : NULL;
}

/* 
 * Inserta un símbolo en la tabla hash.
 * Si el símbolo ya existe se produce un error.
 * 
 * Entrada:
 *      th: tabla hash donde insertar.
 *      lexema: identificador del símbolo.
 *      categ: categoría del símbolo.
 *      tipo: tipo del símbolo.
 *      clase: clase del símbolo.
 *      adic1: valor adicional del símbolo (dependiente de lo anterior, consultar tablaHash.h)
 *      adic2: valor adicional del símbolo (dependiente de lo anterior, consultar tablaHash.h)
 *
 * Salida: 
 *      STATUS: OK si se inserta correctamente, ERR si no (por ya existir o por memoria insuficiente).
 */
STATUS insertar_simbolo(TABLA_HASH *th, const char *lexema, CATEGORIA categ, TIPO tipo, CLASE clase, int adic1, int adic2) {
    int ind;
    INFO_SIMBOLO *is;    
    NODO_HASH *n = NULL;
    
    /* Buscar símbolo */
    if (buscar_simbolo(th, lexema)) {
        return ERR;
    }
    /* Calcular posición */
    ind = hash(lexema) % th->tam;
    /* Reservar nodo e info del nodo */
    if (!(is = crear_info_simbolo(lexema, categ, tipo, clase, adic1, adic2))) {
        return ERR;
    }
    if (!(n = crear_nodo(is))) {
        liberar_info_simbolo(is);
        return ERR;
    }
    /* Insertar al principio de la lista enlazada para ahorrar tiempo */
    n->siguiente = th->tabla[ind];
    th->tabla[ind] = n;
    return OK;
}

/* 
 * Borra un símbolo de la tabla hash.
 * Si el símbolo no existe no se produce ningún efecto.
 * 
 * Entrada:
 *      th: tabla hash donde insertar.
 *      lexema: identificador del símbolo.
 *
 * Salida: ninguna.
 */
void borrar_simbolo(TABLA_HASH *th, const char *lexema) {
    int ind;
    NODO_HASH *n, *prev = NULL;

    /* Calcular posición */
    ind = hash(lexema) % th->tam;
    /* Buscar nodo con elemento y nodo previo */
    n = th->tabla[ind];
    while (n && (!n->info || strcmp(n->info->lexema, lexema))) {
        prev = n;
        n = n->siguiente;
    }
    /* Si no está el elemento, no hay nada que hacer */
    if (!n) return;
    /* Si sí que está, hay que borrar el nodo y reasignar punteros */
    if (!prev) {
        /* Caso especial: el nodo a borrar es el primero */
        th->tabla[ind] = n->siguiente;
    }
    else {
        /* Caso normal: el nodo a borrar es cualquier otro */
        prev->siguiente = n->siguiente;
    }
    liberar_nodo(n);
    return;
}


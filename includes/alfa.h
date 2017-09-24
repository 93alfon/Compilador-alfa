#ifndef _ALFA_H
#define _ALFA_H
#define MAX_LONG_ID 100
#define MAX_TAMANIO_VECTOR 64

#define INT  1
#define BOOLEAN  2

/* Categoría de un símbolo: variable, parámetro de función o función */
typedef enum { VARIABLE, PARAMETRO, FUNCION } CATEGORIA;

/* Tipo de un símbolo: sólo se trabajará con enteros y booleanos */
typedef enum { ENTERO, BOOLEANO } TIPO;

/* Clase de un símbolo: pueden ser variables atómicas (escalares) o listas/arrays (vectores) */
typedef enum { ESCALAR, VECTOR } CLASE;


	/* otros defines */
	typedef struct {
		char lexema[MAX_LONG_ID+1];
		int tipo;
		int valor_entero;
		int es_direccion;
		int etiqueta;
	}tipo_atributos;
	
#endif
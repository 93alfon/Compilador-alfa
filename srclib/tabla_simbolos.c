/*******************************
  Alfonso Bonilla Trueba
  Pareja 11 - Grupo 1311
*******************************/
#include "../includes/tabla_simbolos.h"

TABLA_SIMBOLOS *crear_tabla_simbolos(){

	TABLA_SIMBOLOS * table = (TABLA_SIMBOLOS *) malloc(1*sizeof(TABLA_SIMBOLOS));
	if(table == NULL)
		return NULL;

	table->global = crear_tabla(TAM_TAB_SIMB);
	table->local = NULL;
	return table;
}

STATUS declaracion_variable(TABLA_SIMBOLOS* tabla, char* name, TIPO tipo, CLASE clase, int value, int posicion){

	if(tabla == NULL || name == NULL)
		return ERR;

	if(tabla->local == NULL)
		return insertar_simbolo(tabla->global, name, VARIABLE, tipo, clase, value, posicion);    
	else
		return insertar_simbolo(tabla->local, name, VARIABLE, tipo, clase, value, posicion);
	return OK;
}

STATUS declaracion_parametro(TABLA_SIMBOLOS* tabla, char* name, TIPO tipo, CLASE clase, int value, int posicion){

	if(tabla == NULL || name == NULL)
		return ERR;

	if(tabla->local == NULL)
		return insertar_simbolo(tabla->global, name, PARAMETRO, tipo, clase, value, posicion);    
	else
		return insertar_simbolo(tabla->local, name, PARAMETRO, tipo, clase, value, posicion);
	return OK;
}

STATUS declaracion_funcion(TABLA_SIMBOLOS* tabla, char* name, TIPO tipo){

	if(tabla == NULL || name == NULL)
		return ERR;

	if(tabla->local != NULL)
		return ERR;

	if(insertar_simbolo(tabla->global, name, FUNCION, tipo, ESCALAR, 0, 0) == ERR)
		return ERR;
	
	tabla->local = crear_tabla(TAM_TAB_SIMB);
	
	insertar_simbolo(tabla->local, name, FUNCION, tipo, ESCALAR, 0, 0);

	return OK;
}

STATUS cierre_ambito(TABLA_SIMBOLOS* tabla){

	if(tabla == NULL)
		return ERR;

	if(tabla->local == NULL)
		return ERR;

	liberar_tabla(tabla->local);
	tabla->local = NULL;

	return OK;	
}

BOOL buscar_tabla_simbolo(TABLA_SIMBOLOS *tabla, char* name){
	INFO_SIMBOLO* busqueda;

	if(tabla == NULL || name == NULL)
		return FALSE;

	if(tabla->local!=NULL){
		busqueda = buscar_simbolo(tabla->local, name);

		if(busqueda == NULL)
			return FALSE;

		return TRUE;
	}

	busqueda = buscar_simbolo(tabla->global, name);

	if(busqueda == NULL)
		return FALSE;
	
	return TRUE;
}

CATEGORIA get_categoria_tabla_simbolo(TABLA_SIMBOLOS *tabla, char* name){
	INFO_SIMBOLO* s;
	
	/*Primero buscamos en el ambito local*/
	if(tabla->local != NULL){
		s = buscar_simbolo(tabla->local, name);
		if(s != NULL)
			return s->categoria;
	}

	/*En caso de no haber encontrado nada en la local buscamos en global*/
	s = buscar_simbolo(tabla->global, name);
	return s->categoria;
}

TIPO get_tipo_tabla_simbolo(TABLA_SIMBOLOS *tabla, char* name){
	INFO_SIMBOLO* s;

	/*Primero buscamos en el ambito local*/
	if(tabla->local != NULL){
		s = buscar_simbolo(tabla->local, name);
		if(s != NULL)
			return s->tipo;
	}

	/*En caso de no haber encontrado nada en la local buscamos en global*/
	s = buscar_simbolo(tabla->global, name);
	return s->tipo;
}

CLASE get_clase_tabla_simbolo(TABLA_SIMBOLOS *tabla, char* name){
	INFO_SIMBOLO* s;
	
	/*Primero buscamos en el ambito local*/
	if(tabla->local != NULL){
		s = buscar_simbolo(tabla->local, name);
		if(s != NULL)
			return s->clase;
	}

	/*En caso de no haber encontrado nada en la local buscamos en global*/
	s = buscar_simbolo(tabla->global, name);
	return s->clase;
}

int get_num_params_funcion(TABLA_SIMBOLOS *tabla, char *name){
	INFO_SIMBOLO* s;
	
	/*Busco en global que es el unico ambito que permite funciones*/
	s = buscar_simbolo(tabla->global, name);
	return s->adicional1;
}

int get_tam_vector(TABLA_SIMBOLOS *tabla, char *name){
	INFO_SIMBOLO* s;
	
	/*Primero buscamos en el ambito local*/
	if(tabla->local != NULL){
		s = buscar_simbolo(tabla->local, name);
		if (s != NULL)
			return s->adicional1;
	}

	/*En caso de no haber encontrado nada en la local buscamos en global*/
	s = buscar_simbolo(tabla->global, name);
	return s->adicional1;
}

int get_posicion(TABLA_SIMBOLOS *tabla, char *name){
	INFO_SIMBOLO* s;
	
	/*Primero buscamos en el ambito local*/
	if(tabla->local != NULL){
		s = buscar_simbolo(tabla->local, name);
		if (s != NULL)
			return s->adicional2;
	}

	/*En caso de no haber encontrado nada en la local buscamos en global*/
	s = buscar_simbolo(tabla->global, name);
	return s->adicional2;
}

void modificar_valores_funcion(TABLA_SIMBOLOS *tabla, char* name, int n_param, int n_vars){
	INFO_SIMBOLO* s;
	
	/*CDE*/
	if(tabla == NULL || name == NULL)
		return;

	/*Busco en global que es el unico ambito que permite funciones*/
	s = buscar_simbolo(tabla->global, name);
	s->adicional1 = n_param;
	s->adicional2 = n_vars;
}

AMBITO ambito_actual(TABLA_SIMBOLOS *tabla){
	if(tabla->local != NULL)
		return LOCAL;
	return GLOBAL;
}

AMBITO ambito_variable(TABLA_SIMBOLOS *tabla, char *name){
	INFO_SIMBOLO* s;
	
	if(tabla->local != NULL){
		s = buscar_simbolo(tabla->local, name);
		if(s != NULL)
			return LOCAL;
	}
	return GLOBAL;
}

void borrar_tabla_simbolos(TABLA_SIMBOLOS* tabla){

	if(tabla == NULL)
		return;

	if(tabla->local != NULL)
		liberar_tabla(tabla->local);
	liberar_tabla(tabla->global);

}
/*******************************
  Alfonso Bonilla Trueba
  Pareja 11 - Grupo 1311
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include "../includes/alfa.h"
#include "../includes/y.tab.h"

extern int yylex_destroy (void);

extern FILE* yyin;
extern FILE* yyout;
extern FILE* ensamblador;

int main(int argc, char *argv[]){

	/*Comprobacion de los argumentos*/
	if(argc!=3){
		printf("./alfa <fichero_entrada> <fichero_salida>\n");
		return(EXIT_FAILURE);
	}

	/*Apertura de ficheros con CdE*/
	yyin = fopen(argv[1], "r");
	if(yyin == NULL)
		return(EXIT_FAILURE);

	yyout = fopen("objs/registro.log","w");
	if(yyout == NULL){
		fclose(yyin);
		return(EXIT_FAILURE);
	}

	ensamblador = fopen(argv[2],"w");
	if(ensamblador == NULL){
		fclose(yyin);
		fclose(yyout);
		return(EXIT_FAILURE);
	}

	yyparse();

	fclose(yyin);
	fclose(yyout);
	fclose(ensamblador);
	yylex_destroy();
	
	return(EXIT_SUCCESS);
}

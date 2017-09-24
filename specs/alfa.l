%{
	/*******************************
	Alfonso Bonilla Trueba
	Pareja 11 - Grupo 1311
	*******************************/
	
	/*Codigo C yyleng */
	#include <stdio.h>
	#include <string.h>
	#include "../includes/alfa.h"
	#include "../includes/y.tab.h"

	int linea = 1;
	int columna = 1;
	int morf_err = 0;
%}

%option noyywrap

%%
[ \t]	{ columna ++; }
"\n"	{ columna = 1; linea++; }
"\r\n"	{ columna = 1; linea++; }
"\n\r"	{ columna = 1; linea++; }
"//".*	{ columna = 1; }
"main"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_MAIN; }
"int"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_INT; }
"boolean"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_BOOLEAN; }
"array"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_ARRAY; }
"function"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_FUNCTION; }
"if"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_IF; }
"else"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_ELSE; }
"while"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_WHILE; }
"scanf"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_SCANF; }
"printf"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_PRINTF; }
"return"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_RETURN; }
";"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_PUNTOYCOMA; }
","	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_COMA; }
"("	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_PARENTESISIZQUIERDO; }
")"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_PARENTESISDERECHO; }
"["	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_CORCHETEIZQUIERDO; }
"]"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_CORCHETEDERECHO; }
"{"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_LLAVEIZQUIERDA; }
"}"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_LLAVEDERECHA; }
"=="	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_IGUAL; }
"="	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_ASIGNACION; }
"+"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_MAS; }
"-"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_MENOS; }
"/"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_DIVISION; }
"*"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_ASTERISCO; }
"&&"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_AND; }
"||"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_OR; }
"!="	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_DISTINTO; }
"!"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_NOT; }
"<="	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_MENORIGUAL; }
">="	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_MAYORIGUAL; }
"<"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_MENOR; }
">"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_MAYOR; }
"true"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_TRUE; }
"false"	{ fprintf(yyout, ";D:\t%s\n", yytext); columna += yyleng; return TOK_FALSE; }

[a-zA-Z][a-zA-Z0-9]*	{ if(yyleng > 100){
							fprintf(stderr, "****Error en [lin %d, col %d]: identificador demasiado largo (%s)\n", linea, columna, yytext);
							morf_err = 1;
							return TOK_ERROR;
							}
						columna += yyleng;
						fprintf(yyout, ";D:\t%s\n", yytext);
						strcpy(yylval.atributos.lexema, yytext);
						return TOK_IDENTIFICADOR;
					}

[0-9]+	{	fprintf(yyout, ";D:\t%s\n", yytext);
			columna += yyleng;
			yylval.atributos.valor_entero = atoi(yytext);
			return TOK_CONSTANTE_ENTERA;
		}

.	{ fprintf(stderr, "****Error en [lin %d, col %d]: simbolo no permitido (%s)\n", linea, columna, yytext); morf_err = 1; return TOK_ERROR; }
%%

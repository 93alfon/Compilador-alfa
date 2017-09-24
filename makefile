CC=gcc
CCFLAGS= -g
LIB=
HDIR=includes
OBJDIR=objs
SRCDIR=src
LIBSRCDIR=srclib
LIBOBJDIR=$(LIBSRCDIR)/obj
LIBDIR=lib
SPECS=specs
ASMDIR=.


SOURCES= $(shell ls -1 $(SRCDIR)/*.c | xargs)
OBJS= $(shell echo $(SOURCES) |  sed -e 's:\.c\+:\.o:g' |  sed -e 's:$(SRCDIR)/:$(OBJDIR)/:g')
LIBSOURCES= $(shell ls -1 $(LIBSRCDIR)/*.c | xargs)
LIBOBJS= $(shell echo $(LIBSOURCES) |  sed -e 's:\.c\+:\.o:g' | sed -e 's:$(LIBSRCDIR)/:$(LIBOBJDIR)/:g')
HEADERS= $(shell ls -1 $(HDIR)/*.h | xargs)
ASM= $(shell ls -1 $(ASMDIR)/*.asm | xargs)
ASMOBJ= $(shell echo $(ASM) | sed -e 's:\.asm:\.o:g')
ASMPROG= $(shell echo $(ASM) | sed -e 's:\.asm::g')


all: alfa

$(LIBSRCDIR)/y.tab.c: $(SPECS)/alfa.y
	@echo -e '\e[1;93m\t***Generando alfa***\n\e[0m'
	@printf 'Generando analizador sintactico con bison...'
	@bison --defines=$(HDIR)/y.tab.h -y -v --output=$@ $^
	@echo -e '\e[1;36m[OK] \e[0m'

$(LIBSRCDIR)/lex.yy.c: $(SPECS)/alfa.l
	@printf 'Generando analizador morfologico con flex...'
	@flex --outfile=$@  $^
	@echo -e '\e[1;36m[OK] \e[0m'

$(LIBOBJDIR)/%.o: $(LIBSRCDIR)/%.c $(HEADERS)
	@echo -n Compilando objeto \'$<\'...
	@$(CC) $(CCFLAGS) -c $< -o $@ $(LIB)
	@echo -e '\e[1;36m[OK] \e[0m'
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@echo -n compilando objeto \'$<\'...
	@$(CC) $(CCFLAGS) -c $< -o $@  $(LIB)
	@echo -e '\e[1;36m[OK] \e[0m'

alfa: $(LIBOBJDIR)/y.tab.o $(LIBOBJDIR)/lex.yy.o $(LIBOBJS) $(OBJS)  
	@echo -n compilando ejecutable \'$@\'...
	@$(CC) $(CCFLAGS) $^ -o $@ $(LIB)
	@echo -e '\e[1;36m[OK] \e[0m'

nasm:
	nasm -g -o $(ASMOBJ) -f elf32 $(ASM)
	gcc -m32 -o $(ASMPROG) $(ASMOBJ) lib/alfalib.o

.PHONY: clean all clear help 

clear: clean

autores:
	@echo -e '\e[1;32m\n\tPractica realizada por:\e[0m'
	@echo -e '\e[1;34m\t------------------------------\e[0m'
	@echo -e '\e[1;34m\t--  Alfonso Bonilla Trueba  --\e[0m'
	@echo -e '\e[1;34m\t------------------------------\e[0m'
	@echo -e '\e[1;32m\tPareja 11 Grupo 1311\e[0m\n'

clean:
	@echo -e '\e[1;93mUse make help para ver los comandos disponibles\e[0m'
	@printf 'eliminando ejecutables y limpiando directorio obj...'
	@rm -f $(OBJDIR)/*.o
	@rm -f $(OBJDIR)/*.log
	@rm -f $(LIBOBJDIR)/*.o
	@rm -f alfa
	@rm -f $(LIBSRCDIR)/y.*
	@rm -f $(LIBSRCDIR)/lex.yy.c
	@rm -f $(HDIR)/y.tab.h
	@rm -f *.o
	@echo -e '\e[1;36m[OK] \e[0m'

help:
	@echo -e '\e[1;93m\t-make: realiza un clean, genera un ejecutable pruebaMorfo\n\t  que recibe como argumentos un fichero de entrada y otro de salida\e[0m'
	@echo -e '\t-make tablaHash: compila lex.yy.c a partir de las especificaciones\n\t de alfa.l'
	@echo -e '\e[1;93m\t-make clean: borra los ejecutables y limpia el directorio obj\e[0m'
	@echo -e '\t-make autores: muestra los nombres, el gurpo y la pareja autora de esta practica'

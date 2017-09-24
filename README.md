# Compilador Alfa
### Proyecto de Autómatas y Lenguajes

Compilador del lenguaje de programación alpha (sintaxis incluida en un pdf) que genera el codigo en NASM

## Pasos:
1. **Generar el compilador:**<br>
Mediante el comando <tt>make</tt> se nos genera un ejecutable llamado <b>alfa</b></br>
Con el comando <tt>make clean</tt> se limpian todos los ficheros generados en la compilación, dejando solo los ficheros de codigo fuente.
2. **Generar el codigo ensamblador:**<br>
Ejecutamos el compilador alfa generado anteriormente <tt>./alfa <entrada.alf> <salida.asm></tt></br> 
><i>Los ficheros escritos en alfa tienen extension .alf y los ficheros que se generan en codigo ensamblador tienen extensión.asm</i>
3. **Generar ejecutable de nuestro programa:**<br>
En el makefile existe un objetivo para crear el ejecutable con nasm <tt>make nasm</tt>
><i> En este punto es importante que solo exista un <tt>.asm</tt> en el directorio, el generado en el paso anterior, si existen mas <tt>.asm</tt> se generará un conflicto</i>
4. **Ejecución de nuestro programa:**<br>
Con el comando anterior habremos generado un ejecutable con el nombre del fichero de salida, es decir, si el fichero de salido era <tt>hola.asm</tt> el ejecutable generado en este paso será <tt>hola</tt> y al ejecutarlo realizara las operaciones que describimos en <tt>entrafa.alf</tt>

## Autor
Alfonso Bonilla Trueba

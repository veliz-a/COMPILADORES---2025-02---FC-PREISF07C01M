# COMPILADORES---2025-02---FC-PREISF07C01M

*.exe

Informe de commits realizados ordenados del ultimo al primero:
main/ veliz
- b9f08fe Alejandra Veliz (2025-09-12): Merge pull request #1 from veliz-a/veliz-branch
  
  En esta actualización de laboratorioCompiladores.cpp se reemplazó el procesamiento manual de caracteres por un tokenizador dedicado que identifica palabras, números, símbolos y operadores relacionales (==, !=). La clase Parser fue reestructurada para ser más modular y clara, incorporando funciones auxiliares para validar identificadores y valores, además de ampliar las reglas para tipos de datos (int, float, char), valores numéricos, decimales y caracteres entre comillas. El main también se simplificó, mostrando primero los tokens obtenidos y luego analizando cada bloque de código de forma independiente con la gramática de declaraciones, asignaciones y condicionales.
- 3469426 Alejandra Veliz (2025-09-12): Versión lista para presentación

  En esta versión de laboratorioCompiladores.cpp se amplió considerablemente la funcionalidad del parser: se añadió la función parseASIGNACION() para reconocer instrucciones del tipo ID = EXPR; (por ejemplo: x = 5;), se mejoró parseINSTRUCCIONES() para aceptar tanto declaraciones como asignaciones dentro de bloques { ... } y se incorporó la función auxiliar extraerTokens() para manipular subconjuntos de tokens (se decidio agregar esto debido a que las llaves del IF estan en lineas distintas en varios casos). En el main() ya no se valida solo un if, sino que ahora se muestran los tokens en modo debug, se separa el código en bloques (delimitados por ; o estructuras if/else) y se analiza cada bloque de forma independiente contra distintas gramáticas (IF, COND, DECL, ASIGNACION), validando además las instrucciones internas de los bloques con llaves.
  
- 62d9287 Alejandra Veliz (2025-09-12): Validador automático de expresiones aritméticas en asignaciones

  Se agregó un bloque nuevo de validación de expresiones aritméticas en asignaciones dentro del main(). Ya que previamente solo validaba el IF.
  
- 465aaf5 Alejandra Veliz (2025-09-12): Mejoras en el parser de expresiones aritméticas

  Este commit extiende la gramática del parser para soportar expresiones aritméticas completas (con +, -, *, / y paréntesis), mejorando mucho la capacidad del analizador. 
  
- b75302d Alejandra Veliz (2025-09-12): Merge branch 'veliz-branch' of https://github.com/veliz-a/COMPILADORES---2025-02---FC-PREISF07C01M into veliz-branch

- 241a006 Alejandra Veliz (2025-09-12): Comentario explicativo o cambios realizados (Improved)

  Se reemplazaron las descripciones anteriores por descripciones más claras y explicativas, con un formato uniforme.
  El código en sí no cambió la lógica, los métodos (parseIF, parseCOND, parseDECL, etc.) funcionan exactamente igual que antes.
  
- 5190ec5 HugoRN19 (2025-09-12): Update archivo.txt
  
- ba64e18 Alejandra Veliz (2025-09-12): Comentario explicativo o cambios realizados

  Se agregaron comentarios para un mejor ordenamiento del código. Se realizó un cambio en la función bien relativo a la lectura de archivo.txt
- 22ad3a5 Alejandra Veliz (2025-09-12): Primera version de Parsers

  Se creó la función tokenizar y la clase Parser.
  La función tokenizar Recorre el string de entrada y separa en tokens. Detecta:Espacios en blanco, Operadores de 2 caracteres (==, !=), Delimitadores de 1 carácter ((){}=;<>!) y Palabras (identificadores, números).
  La clase parser recibe los tokens y verifica que la secuencia siga las reglas gramaticales planteadas en gramatica.txt mediante los métodos que posee.
- 694facd Alejandra Veliz (2025-09-12): Inicio Tarea 2 commit

  Se pego el codigo de ejemplo proporcionado por el profesor en ‎laboratorioCompiladores.cpp, ademas se agrego archivos.txt y gramatica.txt.
  
- 52a5511 Alejandra Veliz (2025-09-12): Initial commit

  Se creo los archivos cpp, se observa se esta reutilizando el archivo de un codigo de un ejercicio previo realizado en clase.
- ed23823 Alejandra Veliz (2025-09-12): first commit

  Se colocó información del título de la tarea.


rama Solari
- 08f400b Sofia Solari (2025-09-12): Update laboratorioCompiladores.cpp
- 72cd20d Sofia Solari (2025-09-12): Update laboratorioCompiladores.cpp
- 5a5cd04 Sofia Solari (2025-09-12): Update laboratorioCompiladores.cpp
- 2113020 Sofia Solari (2025-09-12): Update laboratorioCompiladores.cpp
- b59706c Sofia Solari (2025-09-12): Update laboratorioCompiladores.cpp
- 694facd Alejandra Veliz (2025-09-12): Inicio Tarea 2 commit
- 52a5511 Alejandra Veliz (2025-09-12): Initial commit
- ed23823 Alejandra Veliz (2025-09-12): first commit





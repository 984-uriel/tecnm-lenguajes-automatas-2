# Diseño y decisiones

Flujo: archivo → lexer → parser/AST → semántica → TAC optimizado →
ensamblador AT&T → as → gcc → ejecutable.

- `compilador.c` coordina y libera las estructuras en todos los modos.
- `lexer.c` mantiene línea y columna desde 1 y reconoce palabras completas.
- `parser.c` implementa las precedencias de la gramática sin generadores.
  Sincroniza errores en punto y coma o fin de bloque. Limita el anidamiento a 256.
- `ast.c` representa listas de sentencias y hasta tres hijos por nodo.
- `semantica.c` asigna un ID estable por declaración. Los bloques desactivan
  símbolos al salir, pero conservan sus IDs para el backend.
- Una declaración entra en su ámbito antes de evaluar su inicializador;
  `int x=x;` se diagnostica como lectura de la nueva variable sin valor.
- Un if con else acepta una variable como inicializada solo si ambas ramas
  la inicializan. Sin else se conserva el estado de entrada. Igual para while.
- `tac.c` usa operandos tipados (constante o casilla), nunca evalúa cadenas
  como código. Los nombres visibles de variables se conservan, y los IDs de
  casilla distinguen shadowing. Los temporales evitan nombres declarados.
- Plegado de subárboles constantes con aritmética de complemento a dos de 32 bits.
  Nunca se pliega una división por cero. `INT_MIN/-1` se define como INT_MIN.
- DCE marca instrucciones con efectos y conserva la clausura de temporales
  que alimentan esas instrucciones; el harness prueba una cadena muerta y viva.
- `codegen.c` reserva una casilla de pila por símbolo y temporal.
  Linux usa System V AMD64, marco alineado a 16 bytes y printf con formato fijo.
  Windows local usa cdecl de MinGW32 para comprobar salidas sin un intérprete.
- `--run` usa nombres temporales generados internamente; la ruta recibida no
  se interpola en comandos. Los temporales se eliminan al terminar. Linux usa
  creación exclusiva C11; el CRT antiguo de MinGW requiere comprobar existencia.
  La compatibilidad Windows está pensada para ejecución local secuencial.
- El literal positivo máximo es 2147483647. Se acepta -2147483648.
- No se propagan valores de variables como si fueran constantes: dentro de ciclos
  eso sería incorrecto sin análisis de flujo.
- `--ast` imprime la estructura sintáctica sin exigir semántica válida;
  `--check`, TAC y backend sí pasan por la semántica.


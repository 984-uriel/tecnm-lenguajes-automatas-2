# Demo del Hito 1 en tres minutos

Antes de empezar: compilar, ejecutar pruebas y dejar la terminal en `proyecto_integrador/`. Practicar con `practicar-hito1-windows.ps1`. En la demo no se recorren los doce casos completos: se muestra su resultado y se explican los representativos.

## De 0 a 25 segundos

“Nuestro compilador lee archivos Lenguajito. En este hito presentamos el lexer, el parser manual y el árbol sintáctico de expresiones, declaraciones y asignaciones. También comprobamos errores sintácticos y variables sin declarar.”

Mostrar los archivos `lexer.c`, `parser.c`, `ast.c` y `compilador.c`.

## De 25 a 60 segundos

Ejecutar `lenguajito.exe tests\hito1\01_expresion.lj --tokens`.

Explicar `1:1 KW int`: línea 1, columna 1, palabra reservada y lexema `int`. Mostrar ID, NUM, OP, DELIM y EOF. Los espacios y comentarios no son tokens. El lexer no calcula el resultado de la expresión.

## De 60 a 115 segundos

Mostrar los tres AST oficiales con `--ast`.

1. `01_expresion.lj`: `1 + 2 * 3` produce un nodo `+` con `*` como hijo derecho. La multiplicación tiene mayor precedencia.
2. `02_asignacion.lj`: los paréntesis hacen que la suma sea el hijo izquierdo de la multiplicación. El nodo principal es una asignación.
3. `03_sin_init.lj`: `int y;` produce `(programa (declarar y))`, sin inicializador.

El AST expresa estructura. No se obtiene `7` porque todavía no se está ejecutando ni optimizando.

## De 115 a 155 segundos

Mostrar `04_invalida.lj --check`: falta la expresión después de `=` y aparece S01. Mostrar `pruebas\hito1\no_declarada.lj --check`: `y` no fue declarada y aparece E01. Ambos terminan con código 1 esperado.

Explicar que el lexer descubre caracteres inválidos, el parser descubre estructuras inválidas y la comprobación semántica básica descubre nombres no declarados.

## De 155 a 180 segundos

Mostrar la salida real de `make test-hito1` y de las pruebas propias. Indicar el commit y tag reales cuando estén creados. Cerrar con las funciones del hito y una limitación: esta presentación no acredita TAC, ensamblador ni binarios ELF.

## Preguntas de preparación

- ¿Por qué `*` queda dentro de `+`? Porque `aditiva` consume expresiones multiplicativas completas.
- ¿Cómo se asocia `10 - 3 - 2`? A la izquierda: `(10 - 3) - 2`.
- ¿Dónde aumentan línea y columna? En el recorrido del lexer; un salto de línea reinicia la columna a 1.
- ¿Qué diferencia hay entre token y nodo? El token representa una unidad léxica; el nodo representa una construcción sintáctica.
- ¿Por qué la asignación del segundo AST no exige declarar `x`? El modo `--ast` solo valida la sintaxis; `--check` sí comprueba la declaración.
- ¿Qué devuelve un programa válido con `--check`? Código 0 y ninguna salida de error.
- ¿Qué hace `main.c`? Valida los argumentos, elige el modo y llama a `compilar`.

El reparto de voces es opcional. Los tres integrantes deben poder responder todas las preguntas.

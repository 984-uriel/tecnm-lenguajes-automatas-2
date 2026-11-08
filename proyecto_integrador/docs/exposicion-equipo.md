# Ensayo del equipo: cinco minutos

Integrantes: Ingrid Paola Dzul Segovia, Jonathan Uriel Che Castillo y
Josue Efrain Martin Pool. El reparto siguiente es para exponer; todos deben
comprender el compilador completo y revisar personalmente el código.

## Preparación inmediata en este equipo

1. En IntelliJ abre la terminal PowerShell.
2. Desde la raíz Proyecto Integrador ejecuta:

```powershell
cd proyecto_integrador
.\practicar-windows.ps1
```

El script compila, prueba y se detiene antes de cada demostración para que
expliquen lo que van a ejecutar. Para una comprobación automática completa:
`powershell -NoProfile -File .\practicar-windows.ps1 -SinPausas`.

En el equipo de la presentación oficial usen Linux x86-64, GCC, make, binutils
y Python 3.10 o posterior. Desde proyecto_integrador ejecuten:

```bash
bash practicar-linux.sh
```

Windows sirve para ensayar tokens, AST, semántica, TAC y resultados. Para
mostrar el ELF se necesita Linux; no basta con enseñar el .exe de Windows.
Si no tienen Linux local, pueden ensayar en un Codespace del repositorio.
En GitHub, el workflow de main también conserva un artefacto ELF con su
ensamblador. Revisen que puedan descargarlo antes de la clase.

## 0:00–0:35 · Josue: objetivo y evidencia

“Somos Ingrid, Jonathan y Josue. Presentamos Lenguajito, un compilador escrito
en C11. Recibe un archivo .lj con enteros, asignaciones, if/else, while y print.
El proceso transforma texto en tokens, árbol sintáctico, TAC y finalmente
ensamblador y un ejecutable. Las pruebas originales del curso se conservaron.
La implementación inicial tuvo asistencia de IA y está documentada; explicaremos
las decisiones que revisamos.”

Mostrar `make test` ya preparado en una terminal. Resultado esperado: 20/20
doradas, 32/32 adicionales y el harness DCE. No emplear todo el tiempo leyendo
la salida ni afirmar revisión humana si todavía no la han realizado.

## 0:35–1:35 · Ingrid: lexer y parser

Ejecutar los pasos de tokens y AST del ensayo.

“El lexer reconoce palabras, nombres, números y símbolos, y guarda línea y
columna para ubicar errores. El parser descendente recursivo aplica las reglas
de la gramática. En 1 + 2 * 3, la multiplicación queda más profunda en el árbol
porque tiene mayor precedencia. El AST conserva la estructura del programa.”

Archivos para señalar: lexer.c, parser.c y ast.c. Identificar en parser.c las
funciones aditiva, multiplicativa y primaria. EOF significa fin de archivo.

## 1:35–2:35 · Jonathan: semántica y errores

Ejecutar el caso de redeclaración; el código de salida 1 es lo esperado.

“Un programa puede tener sintaxis válida y aun así usar una variable inexistente
o declararla dos veces. La tabla de símbolos registra cada declaración y su
ámbito. E01 indica variable inexistente o leída sin valor; E02, redeclaración;
E04, divisor constante cero; S01, sintaxis o carácter inválido. Un bloque puede
ocultar una variable externa mediante shadowing.”

“Si una variable se inicializa solo dentro de un if sin else o dentro de un
while, no sabemos si esa asignación se ejecutará. Por eso no se acepta su lectura
posterior. Si ambas ramas del if/else la inicializan, sí se acepta.”

## 2:35–3:35 · Jonathan: TAC y optimización

Mostrar `tests/tac/01_folding.lj` y ejecutar --tac. Debe verse `print 10`.

“El TAC divide las expresiones en instrucciones simples con temporales. El
plegado calcula expresiones constantes durante la compilación. La eliminación
de código muerto conserva los temporales que alimentan instrucciones necesarias
y retira los que nadie utiliza. La prueba dce.c comprueba ambos casos.”

Mostrar también docs/optimizacion.md: incluye dos ejemplos antes/después.
No decir que elimina todas las asignaciones a variables: esa optimización
específica no forma parte de esta implementación.

## 3:35–4:45 · Josue: backend y ejecución

Ejecutar --run de 08_anidado.lj. Salida: 1, 2, 4, 3, 6 y 9.
Mostrar `make demo-bin` y `readelf -h build/anidado` en Linux.

“El backend transforma el TAC a instrucciones x86-64 AT&T. Reserva casillas
de pila distintas para las variables y temporales. as produce el objeto y gcc
lo enlaza. --run ejecuta ese binario; no interpreta el AST. readelf confirma
ELF64 y la arquitectura x86-64. printf imprime los enteros con un formato fijo.”

Señalar pushq/movq del prólogo, call printf y leave/ret del epílogo en el .s.

## 4:45–5:00 · Cierre

“Mostramos un programa con anidamiento, un diagnóstico semántico y una
optimización visible. Las pruebas se pueden reproducir con make test y la
bitácora registra el apoyo de IA y las comprobaciones.”

## Preguntas que los tres deben poder contestar

| Pregunta | Respuesta breve |
| :--- | :--- |
| ¿Lexer y parser hacen lo mismo? | El lexer forma tokens; el parser reconoce su estructura gramatical. |
| ¿Qué es el AST? | Una representación de expresiones y sentencias como árbol. |
| ¿Qué es shadowing? | Una declaración interna oculta temporalmente un nombre externo. |
| ¿Por qué una casilla por declaración? | Para que dos variables con igual nombre no compartan almacenamiento. |
| ¿Qué representa 0 en una condición? | Falso; cualquier entero distinto de cero es verdadero. |
| ¿Por qué no sustituir variables por su valor inicial? | Pueden cambiar, especialmente dentro de ciclos. |
| ¿Qué hace ifFalso? | Salta a una etiqueta cuando la condición es cero. |
| ¿Cómo se informa más de un error? | Se acumulan diagnósticos; el parser sincroniza en ; o }, con máximo 20. |
| ¿Por qué alinear la pila? | Es parte del convenio de llamadas de System V AMD64. |
| ¿Una prueba verde garantiza ausencia total de errores? | No; demuestra los casos ejecutados. Hay pruebas adicionales y sanitizadores. |
| ¿Qué falta para una entrega académica completa? | Revisión humana, aportaciones reales, avances y demo del equipo. |

## Antes de entregar

- Abrir el enlace de main y comprobar que los tres nombres están correctos.
- Repetir el ensayo con cronómetro y rotar las explicaciones entre integrantes.
- Registrar las decisiones revisadas por cada persona en las bitácoras.
- Adjuntar la evidencia de CI y el binario ELF si el docente lo solicita.
- Los tags de cierre, si apuntan todos a la versión final, no representan una
  historia de avances en distintas sesiones. No inventar fechas ni commits.

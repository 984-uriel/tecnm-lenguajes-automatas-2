# Entrega · Hito 1 — Léxico, sintaxis y AST

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Docente:** MCC. Iván Márquez Larios  
**Proyecto:** Lenguajito  
**Equipo:** Ingrid Paola Dzul Segovia, Jonathan Uriel Che Castillo y Josue Efrain Martin Pool  
**Repositorio:** https://github.com/josue6martin/tecnm-lenguajes-automatas-2  
**Tag publicado:** `v0.1-hito1`, sobre `3e09970377805c528cccd4936de765530bf96d00`; conservado sin cambios  
**Fecha de preparación de esta auditoría:** 2026-11-06

> **Estado:** borrador en revisión. Los apartes de Jonathan están completados y verificados con sus commits (`56b2f77`, `b95b1cf`). El de Ingrid quedó completado con su commit propio `f3271b8` (revisión de `src/lexer.c` y verificación en Windows); el de Josue sigue abierto a su autor. No se atribuyen demos que todavía no se hayan comprobado.

---

## 1. Qué funciona

En el Hito 1 se integraron el análisis léxico, el análisis sintáctico y el árbol sintáctico abstracto (AST). Los archivos principales son `src/lexer.c`, `src/parser.c` y `src/ast.c`. Desde una terminal Linux o GitHub Codespaces ubicada en `proyecto_integrador/`, los comandos son:

```bash
make
./lenguajito tests/hito1/01_expresion.lj --tokens
./lenguajito tests/hito1/01_expresion.lj --ast
./lenguajito tests/hito1/04_invalida.lj --check
./lenguajito tests/invalidos/01_no_declarada.lj --check
```

- `--tokens` imprime línea, columna, categoría y lexema.
- `--ast` imprime el árbol sintáctico según la gramática oficial.
- `--check` reporta `S01` ante un error sintáctico y `E01` ante una variable no declarada.

Ejemplo de entrada (`tests/hito1/01_expresion.lj`):

```c
int x = 1 + 2 * 3;
```

Salida AST esperada (`tests/hito1/01_expresion.ast`):

```text
(programa (declarar x (+ (num 1) (* (num 2) (num 3)))))
```

El árbol demuestra que la multiplicación tiene prioridad sobre la suma.

## 2. Qué no funciona (y por qué)

Los cuatro casos oficiales del Hito 1 tienen evidencia de ejecución exitosa; no se detectaron fallos en esos casos. Esto no demuestra que se hayan revisado todas las entradas posibles.

- [x] Ejecutar las cuatro pruebas oficiales en Codespaces y adjuntar la captura en el reporte actualizado.
- [x] Revisar entradas propias con paréntesis, operador unario y errores sin modificar las pruebas oficiales. (Jonathan, 2026-11-09: 10/10 en `pruebas/hito1/` con `gcc -Wall -Wextra -Werror`; `tests/` intactas)
- [x] Agregar caso límite del léxico sin modificar las pruebas oficiales. (Ingrid, 2026-11-09: caso `prefijo_kw.{lj,tokens}` en `pruebas/hito1/` — los cinco prefijos de palabras reservadas se clasifican como `ID`; 11/11 en `pruebas/run_hito1.py`, es decir los 10 previos más este; `tests/` intactas; commit `3a4eac9`)
- [ ] Registrar aportaciones y observaciones reales de cada integrante. (Ingrid: registrada el 2026-11-09 con su revisión de `src/lexer.c`; Josue: pendiente)
- [ ] Documentar cualquier limitación adicional que se descubra.

**Alcance:** TAC y backend corresponden a hitos posteriores; no son requisitos de este primer hito.

## 3. Pruebas

Comando oficial:

```bash
make test-hito1
```

Resultado documentado en GitHub Actions el 5 de noviembre de 2026:

```text
Hito 1: 4/4 pruebas superadas
```

**Evidencia:** https://github.com/josue6martin/tecnm-lenguajes-automatas-2/actions/runs/42097233258  
**Commit probado:** `cd714e459c4cb5c1ba07984c115cc17d89f23bef`

Las cuatro pruebas oficiales comprueban tres árboles AST y un error `S01`. El modo `--tokens` y el diagnóstico `E01` deben mostrarse también en la demostración.

**Propuestas iniciales de pruebas adicionales (registro histórico):**

| Caso | Qué comprueba | Evidencia |
| :--- | :--- | :--- |
| `int resultado = (1 + 2) * 3;` | Precedencia con paréntesis | Pendiente |
| `int valor = -5;` | Operador unario | Pendiente |
| `int x = ;` | Error de sintaxis `S01` | Pendiente de captura propia |
| `int x = y;` | Variable no declarada `E01` | Pendiente |

Los casos nuevos incorporados se guardan en `pruebas/hito1/`, fuera de `tests/`, para conservar intactas las pruebas del curso. La tabla anterior registra propuestas; el conjunto efectivamente ejecutado en `pruebas/run_hito1.py` consta de once casos (8 originales, los 2 de Jonathan en `56b2f77` y el caso `prefijo_kw` de Ingrid en `3a4eac9`) y está documentado en el reporte actualizado. El verificador `verificar-hito1-windows.ps1` conserva sus ocho comprobaciones específicas.

## 4. Reparto de trabajo

La tabla refleja una **propuesta de revisión y exposición**, no la autoría del código inicial generado con asistencia de IA. Cada integrante debe anotar sus actividades reales y los commits que le correspondan.

| Integrante | Archivos / etapas para revisar y explicar | Commits principales / evidencia |
| :--- | :--- | :--- |
| Ingrid Paola Dzul Segovia | `src/lexer.c`: tokens, categorías, posiciones y errores léxicos | `f3271b8` (revisión de `lexer.c`, verificación en Windows y registro de su aparte en este documento; evidencia técnica previa: `docs/evidencias/02-tokens.png`); `3a4eac9` (caso de prueba `prefijo_kw`) |
| Jonathan Uriel Che Castillo | `src/parser.c`, `src/ast.c`: gramática, precedencias y representación AST | `56b2f77` (pruebas propias `unario` y `comparacion` + registro en `run_hito1.py`); `b95b1cf` (aparte personal en este documento) |
| Josue Efrain Martin Pool | `src/compilador.c`, `Makefile`, `tests/hito1/`: integración, diagnóstico y pruebas | `cd714e4` (implementación), `069a723` (`test-hito1-propias` + reporte), `bf172b7` (fix verificador Windows), `aa315d1` (documentación) |

### Punto de vista de Ingrid — revisión confirmada

> Al estudiar la parte léxica de Lenguajito, me parece importante cómo el compilador reconoce palabras reservadas, números, identificadores y operadores. También considero útil que registre la línea y la columna, porque así podemos encontrar dónde aparece un error. Quiero verificar estos resultados directamente con los ejemplos antes de confirmar mi revisión.

**Comentario o corrección personal de Ingrid:** Revisé `src/lexer.c` línea por línea (con asistencia de IA para preparar la lectura) y lo confirmé ejecutando el compilador. El lexer distingue las palabras reservadas (`int`, `if`, `else`, `while`, `print`) de los identificadores comparando la longitud exacta del lexema: comprobé que `intx` e `impar` se clasifican como `ID` y no como `KW`, mientras que `int` sí sale como `KW`. Registra la línea y la columna de cada token: en `caracter_invalido.lj` el error `S01` se reporta en la columna exacta 11 donde está el carácter inválido. Ignora los comentarios `//` (la línea 1 de `tokens.lj` no produce ningún token) y añade el `EOF` una sola vez al final (`2:1 EOF` / `4:1 EOF`). Observación que anoto: en modo `--tokens` un error léxico interrumpe la impresión antes de mostrar los tokens, decisión que vive en la integración, no en el lexer.

**Prueba realizada / commit:** Compilé en Windows con `gcc 16.2.0 -std=c11 -Wall -Wextra -Werror -O2` sin warnings y ejecuté `.\verificar-hito1-windows.ps1` → **4/4 pruebas oficiales y 8/8 propias superadas**. Además ejecuté `.\lenguajito.exe tests\hito1\01_expresion.lj --tokens` (formato `LINEA:COLUMNA CAT lexema` conforme a la especificación), `.\lenguajito.exe pruebas\hito1\tokens.lj --tokens` (coincide carácter por carácter con `pruebas/hito1/tokens.tokens`) y dos casos límite propios (`intx`/`impar` como `ID`; `!` aislado → `S01`). Evidencia técnica previa en `docs/evidencias/02-tokens.png`. Commits propios: `f3271b8` (revisión y registro) y `3a4eac9` (caso de prueba `prefijo_kw.{lj,tokens}` en `pruebas/hito1/` con su registro en `run_hito1.py`, suite propia en 11/11).

### Punto de vista de Jonathan — propuesta para que él la revise

> Lo que más me llama la atención del parser y el AST es que reconocer tokens no basta: el compilador necesita organizarlos de acuerdo con las reglas del lenguaje. El ejemplo `1 + 2 * 3` muestra cómo se respeta la prioridad de las operaciones. Para explicar esta etapa, necesito comprobar los árboles esperados y los errores de sintaxis.

**Comentario o corrección personal de Jonathan:** Revisé `src/parser.c` y `src/ast.c` y verifiqué cómo la precedencia queda definida por la cadena de llamadas (`expresion` → `aditiva` → `multiplicativa` → `unaria` → `primaria`) y la asociatividad a la izquierda por los `while` de `aditiva()` y `multiplicativa()`. Comprobé con el compilador que `-5` se imprime como `(neg (num 5))` y que en `1 + 2 < 4` la suma se resuelve antes que la comparación, como pide la gramática.
**Prueba realizada / commit:** Agregué `pruebas/hito1/unario.{lj,ast}` y `pruebas/hito1/comparacion.{lj,ast}` con su registro en `run_hito1.py`. Compilado en Windows con `gcc -std=c11 -Wall -Wextra -Werror` sin warnings: 10/10 pruebas propias y 4/4 oficiales. Commit `56b2f77`.

### Punto de vista de Josue — propuesta para que él la revise

> Considero que lo más importante del primer hito es comprender cómo se conectan el lexer, el parser y el AST para analizar un programa. El comando `make test-hito1` ayuda a verificar que las salidas coincidan con los casos del profesor. Antes de la entrega, quiero repetir las pruebas y explicar los resultados, no solamente mostrar que aparecen en verde.

**Comentario o corrección personal de Josue:** Revisé la integración en `src/compilador.c`: conecta lexer → parser → semántica y despacha cada modo de la línea de comandos. Corregí la compatibilidad local con MinGW y la validación del rango de literales, y arreglé el directorio de trabajo del verificador de Windows. Considero importante repetir las pruebas y explicar los resultados, no solo mostrarlas en verde.
**Prueba realizada / commit:** Agregué el objetivo `test-hito1-propias` al `Makefile` y documenté las pruebas propias con el reporte del Hito 1 (commit `069a723`); fix del verificador Windows (commit `bf172b7`); implementación inicial (commit `cd714e4`). La ejecución de GitHub Actions confirmó 4/4 oficiales, 8/8 propias y 20/20 generales.

**Validación de opiniones:** pendientes de aprobación y ajuste por cada autor.

## 5. Bitácora C-R-E-O (IA)

Se utilizó asistencia de IA para preparar la implementación inicial y la documentación. La tabla distingue lo producido de lo aceptado o revisado realmente por el equipo.

| Contexto | Rol | Especificación | Output | ¿Qué aceptaron, corrigieron o rechazaron? |
| :--- | :--- | :--- | :--- | :--- |
| Preparación del Proyecto Integrador | Asistente de programación | C11, GCC con `-Wall -Wextra -Werror`, libc, parser manual y gramática del curso | Código modular integrado al repositorio | Aceptado: compila limpio y las pruebas pasan (CI verde). Corregimos compatibilidad MinGW y validación de rango de literales. Confirmación de Ingrid registrada; confirmación final de Josue pendiente |
| Léxico y tokens | Apoyo de implementación | `KW`, `ID`, `NUM`, `OP`, `DELIM`, `EOF`; formato `LINEA:COLUMNA TIPO lexema` | `src/lexer.c` y `--tokens` | Ingrid: aceptado tras revisar `lexer.c` línea por línea y verificar en Windows con `--tokens`, dos casos límite propios y `verificar-hito1-windows.ps1` (4/4 oficiales + 8/8 propias); agregó el caso `prefijo_kw` a `pruebas/hito1/` (suite propia en 11/11); commits `f3271b8` y `3a4eac9` |
| Parser y AST | Apoyo de implementación | Análisis descendente recursivo, precedencias y AST del formato oficial | `src/parser.c`, `src/ast.c`, `--ast` | Jonathan: aceptado tras revisar `parser.c` y `ast.c` línea por línea; agregó las pruebas `unario` y `comparacion` (commit `56b2f77`) |
| Diagnósticos y verificación | Asistente de pruebas | `S01` / `E01`, pruebas oficiales sin cambios y `make test-hito1` | Evidencia automatizada 4/4 en GitHub Actions | Josue: revisó la integración y verificó CI 4/4 oficiales + 8/8 propias + 20/20 generales; corrigió el verificador Windows (`bf172b7`) y documentó el reporte (`069a723`). Confirmación final pendiente |

**Correcciones documentadas de la implementación asistida:** compatibilidad local con MinGW y validación del rango de literales; no se atribuyen a un integrante sin evidencia. No se deben inventar decisiones individuales.

**Registro de decisiones del equipo:**

- **Aceptamos:** El compilador asistido por IA cumple la especificación: compilación limpia con `gcc -std=c11 -Wall -Wextra -Werror`, 4/4 pruebas oficiales y 8/8 propias en Windows (`verificar-hito1-windows.ps1`) y CI verde en los commits `56b2f77` y `b95b1cf`. Jonathan aceptó su parte (`parser.c`, `ast.c`) tras revisarla línea por línea y comprobar los árboles con el compilador; Ingrid aceptó su parte (`lexer.c`) tras revisarla línea por línea y verificarla con `--tokens` y casos límite propios; la aprobación de Josue queda pendiente de su propia revisión.
- **Corregimos:** (1) Compatibilidad local con MinGW y validación del rango de literales (ya documentadas arriba). (2) Jonathan: se agregaron las pruebas `unario` y `comparacion` y su registro en `run_hito1.py` — commit `56b2f77`. (3) Jonathan: registro de su aparte personal en este documento — commit `b95b1cf`. (4) Ingrid: registro de su revisión de `lexer.c` con la verificación ejecutada en Windows — commit `f3271b8`. (5) Ingrid: caso límite `prefijo_kw` (los cinco prefijos de palabras reservadas clasificados como `ID`) registrado en `run_hito1.py`, suite propia en 11/11 — commit `3a4eac9`.
- **Rechazamos:** No se registraron rechazos.
- **Consulta adicional a IA:** 2026-11-08 y 2026-11-09 — Jonathan pidió a la IA revisar el repositorio contra `PROYECTO.md` y `gramatica.md`. La IA verificó que las pruebas doradas `tests/` son idénticas (hash por hash) a las del repo del curso, que `--tokens`, `--ast` y los diagnósticos cumplen el formato de la especificación, y señaló pendientes: bitácora incompleta, commits individuales, avances de las sesiones 05 y 08, y cuatro tags sobre el mismo commit. El equipo aceptó completar esta bitácora y registrar commits individuales; la decisión sobre los tags `v0.2`/`v0.3`/`v1.0` queda pendiente.

## 6. Demo

**Meta: 3 minutos para el Hito 1.**

1. **Ingrid (1 min):** explicar el lexer y ejecutar `./lenguajito tests/hito1/01_expresion.lj --tokens`.
2. **Jonathan (1 min):** explicar parser/AST y ejecutar `./lenguajito tests/hito1/01_expresion.lj --ast`.
3. **Josue (1 min):** mostrar `./lenguajito tests/hito1/04_invalida.lj --check` y `make test-hito1`, explicar el error `S01` y el resultado 4/4.

- [ ] Demo ensayada por los tres integrantes.
- [ ] Cada integrante puede explicar las partes esenciales del Hito 1.
- [x] Evidencias reales de ejecución en Windows y Linux anexadas al reporte actualizado.
- [ ] Opiniones personales revisadas por sus autores (Jonathan: revisado y aprobado; Ingrid: revisado y aprobado el 2026-11-09; Josue: pendiente).
- [x] Aportes y commits individuales registrados (Jonathan: `56b2f77` y `b95b1cf`; Ingrid: `f3271b8`, `e346f39`, `dccc14a`, `5c0d0c6` y `3a4eac9`; Josue: pendiente de registro).
- [x] Tag `v0.1-hito1` existente verificado; su existencia no acredita aceptación individual ni demo.

**Conclusión conjunta propuesta para revisar:**

> Este primer hito permite comprender cómo el código fuente se separa en tokens y se organiza en un árbol sintáctico. Las pruebas nos ayudan a comprobar el funcionamiento frente a los ejemplos oficiales. Antes de presentar, queremos entender y verificar cada etapa para poder explicarla con nuestras propias palabras.


## Actualización con evidencias reales del Hito 1

Se incorporó el [reporte PDF](hito1/Reporte-Hito-1.pdf) con nueve capturas: pruebas específicas, tokens, tres AST, aceptación de un programa válido, errores S01/E01 y `make test-hito1` en GitHub Codespaces con 4/4 superadas. Las pruebas propias específicas pasaron 8/8 en Windows y pueden repetirse con `make test-hito1-propias` en Linux. No se atribuye a los integrantes una auditoría personal ni una presentación que todavía no se haya registrado.

La tabla de pruebas propuestas de la sección 3 conserva el historial de preparación. Los once casos efectivamente incorporados están en `pruebas/hito1/` (los 8 originales, `unario` y `comparacion` de Jonathan, y `prefijo_kw` de Ingrid); el reporte explica sus resultados. No se modificó `tests/`.

El tag existente apunta a la implementación previa; este commit añade documentación y comprobaciones. No se movió el tag. Los testimonios propuestos y campos personales anteriores siguen sujetos a revisión de sus autores.


### Revisión técnica posterior

La [ejecución de GitHub Actions del commit 069a723](https://github.com/josue6martin/tecnm-lenguajes-automatas-2/actions/runs/42265588230) confirmó 4/4 pruebas oficiales del Hito 1, 8/8 propias y 20/20 generales. Esto completa la verificación automática de las pruebas propias en Linux. La revisión y las opiniones personales de los integrantes siguen pendientes de confirmación por sus autores.

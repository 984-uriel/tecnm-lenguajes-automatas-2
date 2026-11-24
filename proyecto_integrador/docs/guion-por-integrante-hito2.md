# Guion por integrante · Hito 2 — comandos y preguntas trampa

> **Nota:** documento de estudio para la defensa del Hito 2. Provisional: se
> ajusta o retira cuando el hito quede entregado y aceptado.

Guion de respaldo para la demo del Hito 2. Cada integrante sabe **qué ejecutar**
y **qué responder** si el docente lo cuestiona. Práctica previa:
`verificar-windows.ps1` (esperado: Windows 20/20) y `python tests\run.py --hito2`
(esperado: 14/14).

---

## 0. Preparar la PC de la demo

Desde PowerShell, dentro de `proyecto_integrador/`:

```powershell
gcc -std=c11 -Wall -Wextra -Werror -O2 -o lenguajito.exe src\*.c
powershell -NoProfile -ExecutionPolicy Bypass -File .\verificar-windows.ps1
```

En Linux / GitHub Codespaces:

```bash
cd proyecto_integrador
make && make test-hito2     # 14/14
```

**Plan B si la PC falla:** el CI verde en GitHub Actions desde el celular
(`github.com/josue6martin/tecnm-lenguajes-automatas-2/actions`).

---

## 1. Ingrid — Gramática completa en el parser · 1 minuto

**Comandos:**

```powershell
.\lenguajito.exe tests\validos\03_if.lj --ast
.\lenguajito.exe tests\validos\04_while.lj --ast
.\lenguajito.exe tests\invalidos\05_sintaxis.lj --check
```

**Qué decir:**

- El Hito 1 tenía solo declaraciones y expresiones; el Hito 2 agregó
  **bloques `{ }`**, **`if/else`**, **`while`** y **`print`**.
- La gramática (en `docs/gramatica.md`): `sentencia := declaracion |
  asignacion | impresion | seleccion | iteracion | bloque`.
- Las llaves de `if` y `while` son **obligatorias**: eso elimina el
  *dangling else* y el parser sigue siendo simple.
- El AST de un `if` con `else` es
  `(si COND (entonces BLOQUE) (si_no BLOQUE))`.
- Todo lo que no sigue la gramática sigue siendo **`S01`** con su posición.

**Preguntas típicas:**

- *¿Cómo se ve un `while` en el AST?* → `(mientras COND BLOQUE)`.
- *¿Se puede escribir `if (x) print(1);` sin llaves?* → No: `S01`. Las llaves
  son parte de la gramática del curso.
- *¿Cuánto anidamiento aguanta?* → 256 niveles; después `S01 "anidamiento
  excesivo"` (también aplica a paréntesis).
- *¿Se modificaron las pruebas doradas?* → No: `tests/` sigue intacto,
  verificado contra el repo del curso.

---

## 2. Jonathan — Tabla de símbolos, ámbitos y `--run` · 1 minuto

**Comandos:**

```powershell
.\lenguajito.exe tests\validos\05_shadowing.lj --run        # imprime 2, 1
.\lenguajito.exe tests\invalidos\02_redeclaracion.lj --check  # E02
.\lenguajito.exe tests\invalidos\01_no_declarada.lj --check    # E01
.\lenguajito.exe tests\invalidos\04_div_cero.lj --check        # E04
.\lenguajito.exe tests\validos\08_anidado.lj --run
```

**Qué decir:**

- La **tabla de símbolos** vive en `semantica.c`: un arreglo de
  `{nombre, ámbito, activo, inicializado}` que crece con `crecer()` y se
  libera al terminar.
- **Ámbitos:** el programa arranca en el ámbito 0; cada bloque `{ }` incrementa
  el contador al entrar y al salir **desactiva** (`activo = 0`) los símbolos de
  ese ámbito. Los del ámbito anterior siguen ahí, intactos.
- **Shadowing:** `buscar()` recorre la tabla **de atrás hacia adelante** y solo
  mira los `activo`. En `int x=1; { int x=2; print(x); } print(x);` la `x`
  interna está después en la tabla → gana la interna; al cerrar la llave se
  desactiva y vuelve a verse la externa. El test real imprime `2` y luego `1`
  (con tres niveles anidados imprime `3, 2, 1` — caso de `pruebas/run.py`).
- **E02** solo si la redeclaración es **en el mismo ámbito**; en otro ámbito es
  shadowing legal.
- **E01** cubre tres casos: variable no declarada, **lectura antes de asignar**
  (el flag `inicializado`) y asignación a variable no declarada.
  Detalle fino: en `if/else` la variable solo queda inicializada si **ambas
  ramas** la inicializan.
- **`--run`:** en este hito ejecuta el programa. En Linux usa el binario
  nativo (TAC → ensamblador → gcc); en Windows **interpreta el AST** — opción
  permitida por el DoD del Hito 2 ("interpretando el AST está permitido en
  este hito"), porque el toolchain local es de 64 bits y el backend genera
  ensamblador de 32. La salida es idéntica en ambas vías.
- La aritmética usa `operar()` de `util.c`: 32 bits con wrap definido
  (`-2147483648 - 1` → `2147483647`), sin overflow firmado de C.

**Preguntas típicas:**

- *¿Dónde se guarda el ámbito?* → En el contador `ambito` del análisis
  (`semantica.c`) y en cada símbolo; se incrementa/decrementa en `BLOQUE`.
- *¿Qué pasa con `int x; print(x);`?* → `E01: lectura antes de asignar`: la
  declaración sin inicializador queda con `inicializado = 0`.
- *¿Y `int x=x;`?* → `E01`: se lee antes de que exista el valor.
- *¿Por qué `E04` dice "constante"?* → porque solo se detecta en compilación
  cuando el divisor es constante (`2/(3-3)`); los programas de prueba no
  dividen entre cero en tiempo de ejecución.
- *¿Cuántos errores reporta como máximo?* → 20, luego termina con código 1.
- *¿Se puede hacer `while` infinito?* → Sí, el lenguaje no tiene `break`; un
  `while(1){}` corre indefinidamente (como en C).
- *¿Qué hay que pasar para considerar listo el hito?* → `make test-hito2`
  (14/14), que los diagnósticos tengan el formato exacto y la demo.

---

## 3. Josue — Recuperación de errores y pruebas · 1 minuto

**Comandos:**

```powershell
.\lenguajito.exe pruebas\run.py            # (desde PowerShell: python pruebas\run.py)
python pruebas\run.py                      # → Propias: 32/32
python tests\run.py --hito2                # → Hito 2: 14/14
.\lenguajito.exe tests\invalidos\05_sintaxis.lj --check
```

**Demo de recuperación (varios errores en una pasada):**

Crea un archivo `tmp.lj` con esto:
```c
int x=1; int x=2; print(y); print(2/(3-3));
```
y ejecuta:
```powershell
.\lenguajito.exe tmp.lj --check
# → E02 (redeclaracion) + E01 (no declarada) + E04 (div cero) en UNA salida
```

**Qué decir:**

- Tras un error, el parser entra en **modo pánico** en `lista()`: salta hasta
  el siguiente `;` o `}`, lo consume y sigue con la siguiente sentencia.
  Así reporta **varios errores en una sola pasada** (máx 20).
- Si un token no avanza el cursor, se consume uno para no colgarse.
- Formato exacto: `archivo:línea:columna: error EXX: mensaje` — empieza en 1.
- Códigos de este hito: **`S01`** sintaxis/carácter, **`E01`** no declarada o
  lectura antes de asignar, **`E02`** redeclaración en el mismo ámbito,
  **`E04`** división entre cero constante.
- Salidas: `0` todo bien, `1` errores de compilación, `2` uso malo de la CLI.
- Verificación: `verificar-windows.ps1` 20/20, `tests/run.py` 14/14 en
  Hito 2, 20/20 completo, pruebas propias 32/32, y el CI de GitHub Actions
  en verde (run #25).

**Preguntas típicas:**

- *¿Por qué no se detiene en el primer error?* → La recuperación es un requisito
  del DoD: "se reportan **varios** errores en una pasada".
- *¿Qué diferencia hay entre `S01` y `E01`?* → `S01` es estructura/lexema
  malo (parser o lexer); `E01` es semántica (declaraciones).
- *¿Las pruebas doradas se tocaron?* → No. Son el contrato del curso.
- *¿Dónde está la evidencia?* → El CI verde y `docs/entrega_hito2.md`.

---

## 4. Preguntas generales (cualquiera de los tres debe responder)

- **¿Qué añade el Hito 2 al Hito 1?**
  > "La gramática completa —bloques, if/else, while y print—, la tabla de
  > símbolos con ámbitos anidados y shadowing, los errores semánticos nuevos
  > E02 y E04, la recuperación de varios errores por pasada, y el modo --run
  > que ya ejecuta programas."

- **¿Cuántas funciones hay ahora?**
  > "**61** en 11 archivos `.c`: parser 13, tac 11, semántica 8, util 8,
  > lexer 6, interpretar 5, ast 4, codegen 3, main 2, compilador 1."

- **¿Qué es la bitácora C-R-E-O?**
  > Contexto, Rol, Especificación y Output de cada uso de la IA, más qué se
  > aceptó, corrigió o rechazó; está en `docs/entrega_hito2.md`.

- **¿Qué sigue?** → Hito 3 (sesión 30): TAC para todas las construcciones con
  temporales `_t0, _t1...`, plegado de constantes y eliminación de código muerto.

- **¿Qué limita este hito?** → Aún no hay binario ELF (Hito 4); `--tac` y
  `--asm` existen pero la demo del hito no los exige.

- **¿Quién hizo qué?** → Ver la tabla de `docs/entrega_hito2.md` y el README;
  cada integrante explica cualquier parte que aparezca ahí.

---

## 5. Antes de presentar (checklist del equipo)

1. `verificar-windows.ps1` en verde (20/20) en la PC de la demo.
2. Los tres ensayaron su minuto en voz alta.
3. Cada quien sabe responder **todas** las preguntas de la sección 4.
4. Actions verde a la mano en el celular (plan B).
5. Tags: existe `v0.1-hito1`. El `v0.2-hito2` debe apuntar al commit final
   cuando se acepte la entrega (hoy apunta a un commit viejo — pendiente).

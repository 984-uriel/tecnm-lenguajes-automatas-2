# Exposición por integrante · Hito 1 — comandos y respuestas

Guion de respaldo para la demo del Hito 1. Cada integrante sabe **qué ejecutar** y
**qué responder** si el docente lo cuestiona. La demo oficial son 3 minutos; esto es
para las preguntas. Práctica previa: `practicar-hito1-windows.ps1`.

---

## 0. Preparar la PC de la demo (2 minutos antes)

Desde PowerShell, dentro de `proyecto_integrador/`:

```powershell
# Si la PC ya tiene gcc, nada. Si NO la tiene (una sola vez):
winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT   # abrir terminal nueva después

# Compilar + correr TODO (4 oficiales + 8 propias):
powershell -NoProfile -ExecutionPolicy Bypass -File .\verificar-hito1-windows.ps1
```

En Linux / GitHub Codespaces:

```bash
cd proyecto_integrador
make
make test-hito1          # 4/4 oficiales
```

**Plan B si la PC falla:** mostrar `docs/hito1/Reporte-Hito-1.pdf` (9 capturas reales)
y el CI verde en GitHub Actions desde el celular
(`github.com/josue6martin/tecnm-lenguajes-automatas-2/actions`).

---

## 1. Ingrid — Lexer (`src/lexer.c`) · 1 minuto

**Comandos:**

```powershell
.\lenguajito.exe tests\hito1\01_expresion.lj --tokens
.\lenguajito.exe pruebas\hito1\tokens.lj --tokens
.\lenguajito.exe pruebas\hito1\caracter_invalido.lj --check    # carácter raro → S01
```

**Qué decir:**

- Cada línea es `LINEA:COLUMNA TIPO lexema`, p. ej. `1:1 KW int` → línea 1, columna 1,
  palabra reservada, lexema `int`.
- Categorías: `KW` (int/if/else/while/print), `ID`, `NUM`, `OP`, `DELIM` y `EOF`
  (una sola línea, al final).
- Los espacios y comentarios `//` **no** son tokens. El identificador distingue
  mayúsculas (`suma` ≠ `Suma`).
- Todo lo que no encaja es error léxico `S01` con su posición.

**Preguntas típicas:**

- *¿Dónde se guarda la posición?* → En `Lugar {linea, columna}` de cada token
  (`interno.h`); el recorrido del lexer suma columna por carácter y la reinicia en 1
  al saltar de línea (`lexer.c`).
- *¿Quién reporta el carácter inválido?* → el lexer, con `S01` y el lugar exacto.
- *¿El lexer calcula `1 + 2`?* → No. Solo divide en unidades; calcular es del parser
  en adelante.

---

## 2. Jonathan — Parser y AST (`src/parser.c`, `src/ast.c`) · 1 minuto

**Comandos:**

```powershell
.\lenguajito.exe tests\hito1\01_expresion.lj --ast      # 1 + 2 * 3
.\lenguajito.exe tests\hito1\02_asignacion.lj --ast      # (1 + 2) * 3
.\lenguajito.exe pruebas\hito1\unario.lj --ast           # -5
.\lenguajito.exe pruebas\hito1\comparacion.lj --ast      # 1 + 2 < 4
```

**Qué decir (los 3 apellidos):**

- **Descendente:** empieza en la regla `programa` de la gramática y va bajando hasta
  `primaria`, de lo grande a lo chico.
- **Recursivo:** cada función llama a la siguiente (`expresion → aditiva →
  multiplicativa → unaria → primaria`) y `primaria` **vuelve a llamar a `expresion`**
  cuando encuentra un paréntesis.
- **A mano:** escrito función por función en C, sin `flex`, `bison` ni `ANTLR`
  (regla del proyecto).
- **Precedencia:** `1 + 2 * 3` queda `(+ (num 1) (* (num 2) (num 3)))` porque
  `aditiva()` consume una multiplicativa completa antes de mirar el `+`.
- **Asociatividad izquierda:** `10 - 3 - 2` → `(- (- (num 10) (num 3)) (num 2))`.
- Mis pruebas propias: `-5` → `(neg (num 5))` y `1 + 2 < 4` →
  `(< (+ ...) (num 4))`, la comparación se resuelve al final. Commits `56b2f77`.

**Preguntas típicas:**

- *¿Por qué `*` queda dentro de `+`?* → `aditiva` llama primero a `multiplicativa`,
  que "se come" el `2 * 3` completo.
- *¿Es LL(1)?* → Sí: con un token de anticipación se decide siempre (la gramática
  del curso lo garantiza).
- *¿Qué pasa si hay un error en medio de una sentencia?* → modo pánico en `lista()`:
  salta hasta `;` o `}` para seguir reportando (máximo 20 errores en una pasada).
- *¿Por qué el AST no dice `7`?* → el AST representa **estructura**, no valores; el
  resultado llega en hitos posteriores (TAC/backend).

---

## 3. Josue — Integración, diagnósticos y pruebas · 1 minuto

**Comandos:**

```powershell
.\lenguajito.exe tests\hito1\03_sin_init.lj --check       # válido → código 0, sin salida
.\lenguajito.exe tests\hito1\04_invalida.lj --check        # S01 → código 1
.\lenguajito.exe pruebas\hito1\no_declarada.lj --check     # E01 → código 1
.\verificar-hito1-windows.ps1                              # 4/4 oficiales + 8/8 propias
```

**Qué decir:**

- `compilar()` orquesta el pipeline: lexer → parser → semántica → modo elegido.
- Formato exacto del diagnóstico:
  `archivo:línea:columna: error EXX: mensaje` (empieza en 1; texto libre, formato no).
- Códigos de salida: `0` todo bien, `1` errores de compilación, `2` uso malo de la CLI.
- Las pruebas doradas de `tests/` **no se modificaron** (verificado hash por hash
  contra el repo del curso) y el CI queda en verde en GitHub Actions.

**Preguntas típicas:**

- *¿Cuántos errores reportan como máximo?* → 20, luego termina con código 1.
- *¿Por qué no tocaron `tests/`?* → son el contrato del curso; cambiarlos deja el
  criterio "Pruebas" en 0.
- *¿Qué diferencia hay entre `S01` y `E01`?* → `S01` es estructura/lexema malo;
  `E01` es semántica: variable no declarada o leída antes de tener valor.
- *¿Dónde está la evidencia?* → `docs/hito1/Reporte-Hito-1.pdf` (9 capturas) y el CI.

---

## 4. Preguntas generales (cualquiera de los tres debe responder)

- *¿Por qué C11 y solo libc?* → Reglas del proyecto: sin bibliotecas de terceros.
- *¿Qué es la bitácora C-R-E-O?* → Contexto, Rol, Especificación y Output de cada
  uso de la IA, más **qué aceptaron, corrigieron o rechazaron**; está en
  `docs/entrega_hito1.md`.
- *¿Qué sigue?* → Hito 2 (sesión 20): gramática completa (bloques, if/else, while,
  print), tabla de símbolos con ámbitos y errores `E02`.
- *¿Qué limita este hito?* → No hay TAC, ensamblador ni binario ELF todavía (hitos 3
  y 4). `--run` no se usa en este hito.
- *¿Quién hizo qué?* → Ver `README.md` y la tabla de reparto de
  `docs/entrega_hito1.md`; cada integrante explica cualquier parte que aparezca ahí.

## 5. Antes de presentar (checklist del equipo)

1. `verificar-hito1-windows.ps1` en verde en la PC de la demo (o `make test-hito1` en Linux).
2. Los tres ensayaron su minuto con `practicar-hito1-windows.ps1`.
3. Cada quien sabe responder **todas** las preguntas de la sección 4.
4. Evidencia a la mano: PDF del reporte + Actions verde (celular).
5. Tags: existe `v0.1-hito1`. Si alguien pregunta por los otros tres tags, responder
   con la verdad: se crearon juntos al preparar la estructura y los hitos 2–4 aún no
   se entregan; **recomendación del equipo: eliminarlos hasta que cada hito se entregue**.

---

## 6. Preguntas trampa del docente (cualquiera de los tres debe poder responder)

Preguntas observadas en clase a otros equipos. Respuestas basadas en el código real
del repositorio — repasar antes de la demo.

### 6.1 ¿Cómo funciona el programa?

> "Es un compilador de una sola pasada por etapas. `compilar()` (en
> `src/compilador.c`) abre el archivo, lo lee completo en memoria, y corre el
> pipeline: **lexer → parser → semántica → modo solicitado**. El lexer
> (`lexear`) convierte el texto en tokens; el parser (`parsear`) construye el
> AST; la semántica (`analizar`) valida declaraciones y lecturas; y al final se
> imprime lo que pidió el modo: `--tokens`, `--ast` o el resultado de
> `--check`. Si en cualquier etapa hay errores, no se pasa a la siguiente."

### 6.2 ¿Cómo inicia el programa?

> "Entra por `main()` en `src/main.c`. Valida que vengan 1 o 2 argumentos
> (archivo y modo), si el modo no viene usa `--check` por defecto, verifica que
> el modo esté en la lista permitida (`--tokens`, `--ast`, `--check`, `--tac`,
> `--asm`, `--run`) y enseguida llama a `compilar(argv[1], modo)`. Si los
> argumentos están mal, imprime el uso y devuelve código **2**."

Flujo: `main()` → valida args/modo (`main.c:21-33`) → `compilar(ruta, modo)` → devuelve 0, 1 o 2.

### 6.3 ¿Qué agente / qué modelo de IA usaron?

> "Usamos un **asistente de programación** (IA conversacional) como apoyo de
> implementación y revisión, siempre bajo supervisión del equipo. Cada uso está
> registrado en la **bitácora C-R-E-O** de `docs/entrega_hito1.md`: qué
> contexto se le dio, qué rol jugó, qué especificación siguió y qué produjo —
> y en la última columna está **qué aceptamos, qué corregimos y qué
> rechazamos**. El código lo revisamos y lo comprobamos nosotros con las
> pruebas; la IA no define el criterio de aprobación."

⚠️ Ojo: **no inventar el nombre de la herramienta que usó cada quien**. Cada
integrante responde con la suya (la de Jonathan: OpenCode con modelo MiMo, según
su propia bitácora). Si preguntan por lo de Ingrid o Josue, que ellos respondan.

### 6.4 ¿Cuántas funciones hay?

Conteo real del código fuente en `src/` (10 archivos `.c`):

| Archivo | Funciones | Detalle |
| :--- | ---: | :--- |
| `parser.c` | 13 | `parsear`, `sentencia`, `bloque`, `lista`, `expresion`, `aditiva`, `multiplicativa`, `unaria`, `primaria`, `binaria`, `actual`, `aceptar`, `exigir` |
| `tac.c` | 11 | `generar_tac`, `optimizar_tac`, `expresion`, `sentencias`, `emitir`, `temporal`, `casilla`, `literal`, `op_imprimir`, `tac_imprimir`, `tac_liberar` |
| `semantica.c` | 8 | `analizar`, `revisar`, `expr`, `estado`, `buscar`, `diagnostico`, `restaurar`, `simbolos_liberar` |
| `util.c` | 8 | `reservar`, `crecer`, `copiar`, `error`, `firmado`, `operar`, `operador`, `constante` |
| `lexer.c` | 6 | `lexear`, `agregar`, `tokens_imprimir`, `tokens_liberar`, `letra`, `digito` |
| `ast.c` | 4 | `nodo`, `ast_imprimir`, `imprimir`, `ast_liberar` |
| `codegen.c` | 3 | `cargar`, `ensamblador`, `ejecutar_binario` |
| `main.c` | 2 | `main`, `modo_valido` |
| `compilador.c` | 1 | `compilar` |
| **TOTAL** | **56** | |

> "Hay **56 funciones** en total. Las del Hito 1 que usamos hoy son las de
> `lexer.c` (6), `parser.c` (13), `ast.c` (4), `semantica.c` (8),
> `util.c` (8) y `compilador.c` (1). Las de `tac.c` y `codegen.c` son
> infraestructura de los hitos siguientes. De esas 56, **solo 1 es pública en
> `compilador.h`**: `compilar()`; el resto son `static` o internas."

### 6.5 ¿Dónde se ubica el problema y cuáles son?

(Dos lecturas de la pregunta — responder las dos:)

**Dónde se ubica el problema (la posición del error):**
> "Cada diagnóstico lleva la posición exacta en el formato
> `archivo:línea:columna: error EXX: mensaje`, con línea y columna que empiezan
> en 1. La posición la lleva cada token en su estructura `Lugar {linea,
> columna}` y la calcula el lexer mientras recorre el texto. El formato lo
> imprime `error()` en `src/util.c`; el texto es libre, el formato no."

**Dónde se ubica el problema (en qué etapa/código):**
> "Depende del tipo: el problema **léxico** lo detecta `lexer.c` (carácter
> inválido), el **sintáctico** lo detecta `parser.c` (estructura que no sigue
> la gramática) y el **semántico** lo detecta `semantica.c` (declaraciones)."

**Cuáles son (los códigos de este hito):**
> "**`S01`**: error de lexema o de estructura — carácter inválido, expresión
> incompleta, punto y coma faltante. **`E01`**: error semántico — variable no
> declarada o lectura antes de asignación. Ambos terminan el proceso con código
> de salida **1**; un programa válido devuelve **0** y un uso malo de la CLI **2**."

### 6.6 ¿Dónde está `compilador.h`?

> "Está en **`proyecto_integrador/src/compilador.h`**, junto a los demás
> archivos del compilador. Es el contrato de la interfaz: declara una sola
> función, `int compilar(const char *ruta, const char *modo)`, que recibe la
> ruta del archivo y el modo, y devuelve el código de salida (0, 1 o 2). Lo
> incluyen `main.c` (para llamarlo) y `compilador.c` (para definirlo)."


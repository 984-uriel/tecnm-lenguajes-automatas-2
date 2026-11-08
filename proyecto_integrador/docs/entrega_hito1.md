# Entrega · Hito 1 — Léxico, sintaxis y AST

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Docente:** MCC. Iván Márquez Larios  
**Proyecto:** Lenguajito  
**Equipo:** Ingrid Paola Dzul Segovia, Jonathan Uriel Che Castillo y Josue Efrain Martin Pool  
**Repositorio:** https://github.com/josue6martin/tecnm-lenguajes-automatas-2  
**Tag publicado:** `v0.1-hito1`, sobre `3e09970377805c528cccd4936de765530bf96d00`; conservado sin cambios  
**Fecha de preparación de esta auditoría:** 2026-11-06

> **Estado:** borrador para revisión de los tres integrantes. Las opiniones son propuestas, no testimonios confirmados. No se atribuyen trabajos individuales ni demos que todavía no se hayan comprobado.

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
- [ ] Revisar entradas propias con paréntesis, operador unario y errores sin modificar las pruebas oficiales.
- [ ] Registrar aportaciones y observaciones reales de cada integrante.
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

Los casos nuevos incorporados se guardan en `pruebas/hito1/`, fuera de `tests/`, para conservar intactas las pruebas del curso. La tabla anterior registra propuestas; el conjunto efectivamente ejecutado consta de ocho casos y está documentado en el reporte actualizado.

## 4. Reparto de trabajo

La tabla refleja una **propuesta de revisión y exposición**, no la autoría del código inicial generado con asistencia de IA. Cada integrante debe anotar sus actividades reales y los commits que le correspondan.

| Integrante | Archivos / etapas para revisar y explicar | Commits principales / evidencia |
| :--- | :--- | :--- |
| Ingrid Paola Dzul Segovia | `src/lexer.c`: tokens, categorías, posiciones y errores léxicos | Pendiente de registrar |
| Jonathan Uriel Che Castillo | `src/parser.c`, `src/ast.c`: gramática, precedencias y representación AST | Pendiente de registrar |
| Josue Efrain Martin Pool | `src/compilador.c`, `Makefile`, `tests/hito1/`: integración, diagnóstico y pruebas | Pendiente de registrar |

### Punto de vista de Ingrid — propuesta para que ella la revise

> Al estudiar la parte léxica de Lenguajito, me parece importante cómo el compilador reconoce palabras reservadas, números, identificadores y operadores. También considero útil que registre la línea y la columna, porque así podemos encontrar dónde aparece un error. Quiero verificar estos resultados directamente con los ejemplos antes de confirmar mi revisión.

**Comentario o corrección personal de Ingrid:** [editar aquí]  
**Prueba realizada / commit:** [completar]

### Punto de vista de Jonathan — propuesta para que él la revise

> Lo que más me llama la atención del parser y el AST es que reconocer tokens no basta: el compilador necesita organizarlos de acuerdo con las reglas del lenguaje. El ejemplo `1 + 2 * 3` muestra cómo se respeta la prioridad de las operaciones. Para explicar esta etapa, necesito comprobar los árboles esperados y los errores de sintaxis.

**Comentario o corrección personal de Jonathan:** [editar aquí]  
**Prueba realizada / commit:** [completar]

### Punto de vista de Josue — propuesta para que él la revise

> Considero que lo más importante del primer hito es comprender cómo se conectan el lexer, el parser y el AST para analizar un programa. El comando `make test-hito1` ayuda a verificar que las salidas coincidan con los casos del profesor. Antes de la entrega, quiero repetir las pruebas y explicar los resultados, no solamente mostrar que aparecen en verde.

**Comentario o corrección personal de Josue:** [editar aquí]  
**Prueba realizada / commit:** [completar]

**Validación de opiniones:** pendientes de aprobación y ajuste por cada autor.

## 5. Bitácora C-R-E-O (IA)

Se utilizó asistencia de IA para preparar la implementación inicial y la documentación. La tabla distingue lo producido de lo aceptado o revisado realmente por el equipo.

| Contexto | Rol | Especificación | Output | ¿Qué aceptaron, corrigieron o rechazaron? |
| :--- | :--- | :--- | :--- | :--- |
| Preparación del Proyecto Integrador | Asistente de programación | C11, GCC con `-Wall -Wextra -Werror`, libc, parser manual y gramática del curso | Código modular integrado al repositorio | Integración inicial asistida por IA; aceptación del equipo pendiente |
| Léxico y tokens | Apoyo de implementación | `KW`, `ID`, `NUM`, `OP`, `DELIM`, `EOF`; formato `LINEA:COLUMNA TIPO lexema` | `src/lexer.c` y `--tokens` | Revisar tokens válidos, posiciones y caracteres inválidos antes de aprobar |
| Parser y AST | Apoyo de implementación | Análisis descendente recursivo, precedencias y AST del formato oficial | `src/parser.c`, `src/ast.c`, `--ast` | Las pruebas de AST están documentadas como superadas; auditoría individual pendiente |
| Diagnósticos y verificación | Asistente de pruebas | `S01` / `E01`, pruebas oficiales sin cambios y `make test-hito1` | Evidencia automatizada 4/4 en GitHub Actions | Registrar capturas y decisiones propias de los integrantes |

**Correcciones documentadas de la implementación asistida:** compatibilidad local con MinGW y validación del rango de literales; no se atribuyen a un integrante sin evidencia. No se deben inventar decisiones individuales.

**Registro de decisiones del equipo:**

- **Aceptamos:** [qué verificamos, cómo y por qué].
- **Corregimos:** [archivo, cambio y commit; si no hubo, escribir «Ninguna corrección propia registrada»].
- **Rechazamos:** [sugerencia y motivo; si no hubo, escribir «No se registraron rechazos»].
- **Consulta adicional a IA:** [fecha, contexto, respuesta y juicio del equipo, si aplica].

## 6. Demo

**Meta: 3 minutos para el Hito 1.**

1. **Ingrid (1 min):** explicar el lexer y ejecutar `./lenguajito tests/hito1/01_expresion.lj --tokens`.
2. **Jonathan (1 min):** explicar parser/AST y ejecutar `./lenguajito tests/hito1/01_expresion.lj --ast`.
3. **Josue (1 min):** mostrar `./lenguajito tests/hito1/04_invalida.lj --check` y `make test-hito1`, explicar el error `S01` y el resultado 4/4.

- [ ] Demo ensayada por los tres integrantes.
- [ ] Cada integrante puede explicar las partes esenciales del Hito 1.
- [x] Evidencias reales de ejecución en Windows y Linux anexadas al reporte actualizado.
- [ ] Opiniones personales revisadas por sus autores.
- [ ] Aportes y commits individuales registrados.
- [x] Tag `v0.1-hito1` existente verificado; su existencia no acredita aceptación individual ni demo.

**Conclusión conjunta propuesta para revisar:**

> Este primer hito permite comprender cómo el código fuente se separa en tokens y se organiza en un árbol sintáctico. Las pruebas nos ayudan a comprobar el funcionamiento frente a los ejemplos oficiales. Antes de presentar, queremos entender y verificar cada etapa para poder explicarla con nuestras propias palabras.


## Actualización con evidencias reales del Hito 1

Se incorporó el [reporte PDF](hito1/Reporte-Hito-1.pdf) con nueve capturas: pruebas específicas, tokens, tres AST, aceptación de un programa válido, errores S01/E01 y `make test-hito1` en GitHub Codespaces con 4/4 superadas. Las pruebas propias específicas pasaron 8/8 en Windows y pueden repetirse con `make test-hito1-propias` en Linux. No se atribuye a los integrantes una auditoría personal ni una presentación que todavía no se haya registrado.

La tabla de pruebas propuestas de la sección 3 conserva el historial de preparación. Los ocho casos efectivamente incorporados están en `pruebas/hito1/`; el reporte explica sus resultados. No se modificó `tests/`.

El tag existente apunta a la implementación previa; este commit añade documentación y comprobaciones. No se movió el tag. Los testimonios propuestos y campos personales anteriores siguen sujetos a revisión de sus autores.


### Revisión técnica posterior

La [ejecución de GitHub Actions del commit 069a723](https://github.com/josue6martin/tecnm-lenguajes-automatas-2/actions/runs/42265588230) confirmó 4/4 pruebas oficiales del Hito 1, 8/8 propias y 20/20 generales. Esto completa la verificación automática de las pruebas propias en Linux. La revisión y las opiniones personales de los integrantes siguen pendientes de confirmación por sus autores.

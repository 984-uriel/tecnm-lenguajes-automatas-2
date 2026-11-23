# Bitácora de Requerimientos y Auditoría IA — Sesión 16
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Firmas de Función y Parámetros  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Registro de prototipos y aridad  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir `verificar_llamada()` a mano, el punto principal fue distinguir claramente entre tres estados de retorno: `1` cuando el nombre existe y `aridad == nargs`, `0` cuando el nombre existe pero la cantidad de argumentos no coincide (`aridad != nargs`), y `-1` cuando el catálogo o el nombre son `NULL`, `total <= 0`, `nargs < 0` o el nombre no se encuentra en el catálogo. Además, se debe comparar `aridad` contra `nargs` (no contra `total`), permitir funciones de `0` parámetros únicamente cuando `nargs == 0`, y compilar sin advertencias bajo `-std=c11 -Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Coincidencia estricta de parámetros |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Estoy programando en C11 con GCC usando las banderas -std=c11 -Wall -Wextra -Werror. La estructura `Funcion` (con campos `const char *nombre` e `int aridad`) y la firma de `verificar_llamada` ya están definidas en `api_sesion16.h`. Este archivo se compilará junto con `harness_sesion16_c.c`, por lo que NO debe incluir `main()`.

Rol: Actúa como un ingeniero de software de sistemas y compiladores especializado en verificación de firmas y análisis semántico.

Especificación: Implementa `int verificar_llamada(const Funcion *catalogo, int total, const char *nombre, int nargs)` siguiendo estas reglas:
- Si `catalogo == NULL`, `nombre == NULL`, `total <= 0` o `nargs < 0`, devuelve `-1`.
- Recorre el arreglo `catalogo` de `0` a `total - 1` buscando coincidencia exacta de nombre (`strcmp(catalogo[i].nombre, nombre) == 0`).
- Si el nombre existe: devuelve `1` si `catalogo[i].aridad == nargs`, o `0` si la aridad no coincide (incluyendo el caso donde `aridad == 0` y solo `nargs == 0` es válido).
- Si el nombre no existe en todo el catálogo, devuelve `-1`.

Output: Entrega únicamente el código modular en C listo para `ia_sesion16_c.c` (sin `main()`) y un resumen breve de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion16.h"
#include <stddef.h>
#include <string.h>

int verificar_llamada(const Funcion *catalogo, int total, const char *nombre, int nargs) {
    if (!catalogo || !nombre || total <= 0 || nargs < 0) {
        return -1;
    }

    for (int idx = 0; idx < total; idx++) {
        const Funcion *entry = &catalogo[idx];
        if (entry->nombre && strcmp(entry->nombre, nombre) == 0) {
            return (entry->aridad == nargs) ? 1 : 0;
        }
    }

    return -1;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Llamadas con número incorrecto de args

### Salida del harness
```text
=== AUDITORIA SESION 16 - Registro de prototipos y aridad ===
Caso                               esperado:              obtenido:              resultado
[ 1] suma con 3 argumentos              esperado: 1            obtenido: 1            OK
[ 2] suma con 2 argumentos              esperado: 0            obtenido: 0            OK
[ 3] suma con 4 argumentos              esperado: 0            obtenido: 0            OK
[ 4] funcion no registrada              esperado: -1           obtenido: -1           OK
[ 5] main sin argumentos                esperado: 1            obtenido: 1            OK
[ 6] nombre NULL                        esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion16.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En implementaciones rápidas de verificación de llamadas suelen pasarse por alto entradas inválidas como nombre == NULL, catalogo == NULL o nargs < 0 (que deben devolver -1 y no 0), así como diferenciar limpiamente entre una función inexistente (-1) y una función encontrada con aridad distinta (0), incluyendo prototipos con 0 parámetros. Se exigió en el contrato C-R-E-O la validación previa de punteros nulos y valores inválidos, y se revisó que el retorno sea 1 solo ante coincidencia exacta de nombre y aridad, 0 ante aridad distinta y -1 si no existe el prototipo.

---
**Firma digital:** [984-uriel]

# Bitácora de Requerimientos y Auditoría IA — Sesión 22
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Generación de TAC para Expresiones  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Traductor de nodos AST a TAC  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al recorrer el árbol en postorden en C, el punto clave fue devolver hacia el nodo padre el nombre del temporal (`_t0`, `_t1`, ...) generado en los subárboles hijos o el identificador de la hoja (`valor`), cuidando no exceder `max` líneas ni los 64 bytes (`63` caracteres + `'\0'`) de cada fila de `lineas[][64]` y compilando sin advertencias bajo `-Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Conservación de precedencia de operadores |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[CONTEXTO]
Estoy cursando Lenguajes y Autómatas II (SCD-1016) en el track de C (estándar C11, compilado con gcc -std=c11 -Wall -Wextra -Werror). Debo implementar `int generar_tac(const Nodo *raiz, char lineas[][64], int max);` declarada en `api_sesion22.h` para traducir un árbol de sintaxis abstracta (AST) de expresiones aritmético-lógicas a código de tres direcciones (TAC), usando el tipo `Pool` y la función `nuevo_temporal(Pool *p, int *contador)` ya provistos, sin `main()`, sin variables globales y sin memoria dinámica.

[ROL]
Actúa como ingeniero de software de sistemas y compiladores especializado en traducción dirigida por sintaxis y generación de código intermedio.

[ESPECIFICACIÓN]
Implementa `generar_tac` respetando exactamente la firma en `api_sesion22.h`:
- Recorre el AST en postorden: primero resuelve subárbol izquierdo (`izq`), luego subárbol derecho (`der`) y al final procesa el operador binario (`tipo == 'B'`).
- Si un nodo es hoja (`tipo == 'H'`), no emite línea y devuelve su identificador/valor (`nodo->valor`).
- Si un nodo es operador binario (`tipo == 'B'`), solicita un temporal nuevo con `nuevo_temporal(&pool, &contador)` (empezando en `_t0`, `_t1`, ...) y formatea la línea `lineas[idx]` como `"_tX = <izq> <op> <der>"` (por ejemplo, `_t0 = b * c`).
- Respeta estrictamente el límite `max` de líneas y el ancho de 64 bytes por línea (máximo 63 caracteres útiles + terminador nulo), devolviendo la cantidad total de líneas emitidas (`0` si es una sola hoja o `raiz == NULL` o `max <= 0`).

[OUTPUT]
Entrega únicamente el código C modular listo para pegar en `ia_sesion22_c.c` (sin `main()`) y un breve análisis de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion22.h"
#include <stdio.h>
#include <stddef.h>

typedef struct {
    char nombre[64][16];
    int usados;
} Pool;

static const char *pedir_temporal(Pool *p, int *contador) {
    if (p->usados >= 64) {
        return "_t?";
    }
    (void)snprintf(p->nombre[p->usados], sizeof p->nombre[0], "_t%d", (*contador)++);
    return p->nombre[p->usados++];
}

static const char *emitir_desde_nodo(const Nodo *actual, char lineas[][64], int max,
                                     int *total, Pool *pool, int *contador) {
    if (actual == NULL) {
        return "_";
    }

    if (actual->tipo != 'B') {
        return (actual->valor[0] != '\0') ? actual->valor : "_";
    }

    const char *op_izq = emitir_desde_nodo(actual->izq, lineas, max, total, pool, contador);
    const char *op_der = emitir_desde_nodo(actual->der, lineas, max, total, pool, contador);
    const char *temp = pedir_temporal(pool, contador);

    if (*total < max) {
        (void)snprintf(lineas[*total], (size_t)64, "%s = %s %c %s",
                       temp, op_izq, actual->op, op_der);
        (*total)++;
    }

    return temp;
}

int generar_tac(const Nodo *raiz, char lineas[][64], int max) {
    if (raiz == NULL || lineas == NULL || max <= 0) {
        return 0;
    }

    Pool pool = {.usados = 0};
    int contador = 0;
    int total = 0;

    (void)emitir_desde_nodo(raiz, lineas, max, &total, &pool, &contador);
    return total;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Expresiones anidadas con paréntesis

### Salida del harness
```text
=== AUDITORIA SESION 22 - Traductor de AST a TAC con precedencia ===
Caso                               esperado:              obtenido:              resultado
[ 1] a + b * c                          esperado: _t0 = b * c  obtenido: _t0 = b * c  OK
[ 2] a + b * c (2a linea)
     esperado: _t1 = a + _t0
     obtenido: _t1 = a + _t0
     -> OK
[ 3] (a + b) * c
     esperado: _t1 = _t0 * c
     obtenido: _t1 = _t0 * c
     -> OK
[ 4] x * (y - z) + w
     esperado: _t2 = _t1 + w
     obtenido: _t2 = _t1 + w
     -> OK
[ 5] hoja sin operaciones               esperado: 0            obtenido: 0            OK
[ 6] (a + b) * (c - d)
     esperado: _t2 = _t0 * _t1
     obtenido: _t2 = _t0 * _t1
     -> OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion22.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Una propuesta inicial sin guardas explícitas suele omitir entradas nulas (raiz == NULL o lineas == NULL), capacidades max <= 0 y la comprobación *total < max antes de escribir en lineas[*total], lo que podría provocar escrituras fuera de límites en expresiones con más operadores que max. Se resolvió agregando la validación inicial if (raiz == NULL || lineas == NULL || max <= 0) return 0; y condicionando snprintf a *total < max, manteniendo complejidad temporal $O(N)$ respecto al número de nodos del AST.

---
**Firma digital:** [984-uriel]

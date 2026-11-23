# Bitácora de Requerimientos y Auditoría IA — Sesión 20
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Hito 2: Analizador Semántico y Scopes  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Integración de Tabla de Símbolos y AST  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al implementar `profundidad_de()` y `contar_visibles()` a mano, el reto principal fue distinguir entre argumento nulo (`-2`, sin desreferenciar punteros inválidos) y símbolo no encontrado (`-1`), respetar el *shadowing* devolviendo la profundidad del primer ámbito coincidente desde el bloque actual (`nivel = 0`) hacia arriba, y en `contar_visibles()` evitar contar duplicados cuando un mismo identificador aparece en varios niveles de la cadena de ámbitos, manteniendo un arreglo auxiliar con límite de capacidad y compilación limpia bajo `-std=c11 -Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Contrato de Validación Semántica Integral |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Estoy programando en C11 con GCC usando las banderas -std=c11 -Wall -Wextra -Werror. La estructura `Ambito` (con `nombre`, `simbolos[AMBITO_MAX]`, `total` y `padre`) y las firmas de `profundidad_de` y `contar_visibles` ya están declaradas en `api_sesion20.h`. Este archivo se compilará junto con `harness_sesion20_c.c`, por lo que NO debe incluir `main()`.

Rol: Actúa como un ingeniero de software de sistemas y compiladores especializado en análisis semántico, resolución de nombres y visibilidad léxica.

Especificación: Implementa exactamente dos funciones:
1. `int profundidad_de(const Ambito *actual, const char *nombre)`:
   - Si `actual == NULL` o `nombre == NULL`, devuelve `-2`.
   - Recorre desde `actual` (profundidad `0`) subiendo por `padre` (`1`, `2`, ...); en cada nivel revisa `simbolos[0 .. total - 1]` con `strcmp`.
   - Al encontrar el primer match, devuelve la profundidad actual (respetando el shadowing: gana el ámbito más interno).
   - Si recorre toda la cadena hasta `NULL` sin encontrarlo, devuelve `-1`.
2. `int contar_visibles(const Ambito *actual)`:
   - Si `actual == NULL`, devuelve `-2`.
   - Recorre `actual` y sus ancestros por `padre` y devuelve cuántos nombres de símbolos distintos son visibles, contando cada nombre una sola vez aunque esté repetido en varios ámbitos.

Output: Entrega únicamente el código modular en C listo para `ia_sesion20_c.c` (sin `main()`) y un análisis breve de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion20.h"
#include <stddef.h>
#include <string.h>

static int esta_en_lista(const char *const *lista, int cantidad, const char *candidato) {
    for (int k = 0; k < cantidad; k++) {
        if (lista[k] && strcmp(lista[k], candidato) == 0) {
            return 1;
        }
    }
    return 0;
}

int profundidad_de(const Ambito *actual, const char *nombre) {
    if (!actual || !nombre) {
        return -2;
    }

    int prof = 0;
    for (const Ambito *scope = actual; scope != NULL; scope = scope->padre) {
        if (esta_en_lista(scope->simbolos, scope->total, nombre)) {
            return prof;
        }
        prof++;
    }

    return -1;
}

int contar_visibles(const Ambito *actual) {
    if (!actual) {
        return -2;
    }

    const char *unicos[512];
    int n_unicos = 0;
    int cap = (int)(sizeof(unicos) / sizeof(unicos[0]));

    for (const Ambito *scope = actual; scope != NULL; scope = scope->padre) {
        for (int idx = 0; idx < scope->total; idx++) {
            const char *sim = scope->simbolos[idx];
            if (sim && !esta_en_lista(unicos, n_unicos, sim) && n_unicos < cap) {
                unicos[n_unicos++] = sim;
            }
        }
    }

    return n_unicos;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Casos complejos de shadowing y ámbito

### Salida del harness
```text
=== AUDITORIA SESION 20 - Resolucion de nombres y visibilidad ===
Caso                               esperado:              obtenido:              resultado
[ 1] nombre en el ambito actual         esperado: 0            obtenido: 0            OK
[ 2] nombre dos niveles arriba          esperado: 2            obtenido: 2            OK
[ 3] shadowing gana el interno          esperado: 0            obtenido: 0            OK
[ 4] nombre no visible                  esperado: -1           obtenido: -1           OK
[ 5] visibles sin repetir               esperado: 3            obtenido: 3            OK
[ 6] ambito NULL                        esperado: -2           obtenido: -2           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion20.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En soluciones generadas sin contrato estricto para análisis de ámbitos suele omitirse la distinción entre NULL (-2) y nombre inexistente (-1), o bien sumar todos los símbolos de todos los ámbitos sin deduplicar en contar_visibles(). Se especificó en el contrato C-R-E-O devolver -2 ante actual == NULL o nombre == NULL, -1 si no existe el símbolo y deduplicar mediante un auxiliar de búsqueda (esta_en_lista) antes de registrar cada símbolo visible.

---
**Firma digital:** [984-uriel]

# Bitácora de Requerimientos y Auditoría IA — Sesión 19
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Estructuras (struct) y Miembros  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Offsets de miembros en estructuras  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al calcular `offset_campo()` y `tamano_struct()` a mano, el punto crítico fue aplicar correctamente la alineación de cada campo a su propio ancho antes de asignar su offset y, al final de `tamano_struct()`, redondear el tamaño acumulado al múltiplo de la mayor alineación de la estructura (padding final). También hay que cuidar casos especiales como una estructura con `total == 0` (que debe devolver `0`), miembros inexistentes o tipos desconocidos (`-1`), y compilar sin advertencias bajo `-std=c11 -Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Alineación de memoria por palabras |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Estoy programando en C11 con GCC usando las banderas -std=c11 -Wall -Wextra -Werror. La estructura `Campo` (con campos `const char *nombre` y `const char *tipo`) y las firmas de `offset_campo` y `tamano_struct` están declaradas en `api_sesion19.h`. Este archivo se compilará junto con `harness_sesion19_c.c`, por lo que NO debe incluir `main()`.

Rol: Actúa como un ingeniero de software de sistemas y compiladores especializado en diseño de estructuras de datos y alineación en memoria.

Especificación: Implementa exactamente dos funciones:
1. `int offset_campo(const Campo *campos, int total, const char *nombre)`:
   - Si `campos == NULL`, `total <= 0`, `nombre == NULL`, `nombre` está vacío o algún campo tiene tipo inválido/desconocido, devuelve `-1`.
   - Para cada campo en orden (`0 .. total - 1`), obtiene el ancho de su tipo (`char` -> 1, `short` -> 2, `int`/`float` -> 4, `double`/`long` -> 8, `puntero` -> `sizeof(void *)`), redondea el cursor actual al siguiente múltiplo de ese ancho y, si `campos[i].nombre` coincide exactamente con `nombre`, devuelve ese offset.
   - Si el campo no existe, devuelve `-1`.
2. `int tamano_struct(const Campo *campos, int total)`:
   - Si `total == 0`, devuelve `0`.
   - Si `campos == NULL` o `total < 0` o algún campo tiene tipo inválido, devuelve `-1`.
   - Calcula el offset y fin de cada campo alineado a su ancho, rastrea la mayor alineación encontrada y al final redondea el tamaño acumulado al múltiplo de esa alineación mayor.

Output: Entrega únicamente el código modular en C listo para `ia_sesion19_c.c` (sin `main()`) y un resumen breve de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion19.h"
#include <stddef.h>
#include <string.h>

static int tamano_tipo_miembro(const char *tipo) {
    if (!tipo) return -1;
    if (strcmp(tipo, "char") == 0) return 1;
    if (strcmp(tipo, "short") == 0) return 2;
    if (strcmp(tipo, "int") == 0 || strcmp(tipo, "float") == 0) return 4;
    if (strcmp(tipo, "double") == 0 || strcmp(tipo, "long") == 0) return 8;
    if (strcmp(tipo, "puntero") == 0) return (int)sizeof(void *);
    return -1;
}

static int redondear_multiplo(int offset, int align) {
    if (align <= 1) return offset;
    int mod = offset % align;
    return (mod == 0) ? offset : (offset + (align - mod));
}

int offset_campo(const Campo *campos, int total, const char *nombre) {
    if (!campos || total <= 0 || !nombre || *nombre == '\0') {
        return -1;
    }

    int actual = 0;
    for (int idx = 0; idx < total; idx++) {
        int tam = tamano_tipo_miembro(campos[idx].tipo);
        if (tam <= 0) {
            return -1;
        }

        int offset = redondear_multiplo(actual, tam);
        if (campos[idx].nombre && strcmp(campos[idx].nombre, nombre) == 0) {
            return offset;
        }

        actual = offset + tam;
    }

    return -1;
}

int tamano_struct(const Campo *campos, int total) {
    if (total == 0) {
        return 0;
    }
    if (!campos || total < 0) {
        return -1;
    }

    int actual = 0;
    int mayor_align = 1;

    for (int idx = 0; idx < total; idx++) {
        int tam = tamano_tipo_miembro(campos[idx].tipo);
        if (tam <= 0) {
            return -1;
        }

        int offset = redondear_multiplo(actual, tam);
        actual = offset + tam;
        if (tam > mayor_align) {
            mayor_align = tam;
        }
    }

    return redondear_multiplo(actual, mayor_align);
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Acceso a miembros inexistentes

### Salida del harness
```text
=== AUDITORIA SESION 19 - Offsets de miembros y tamano de struct ===
Caso                               esperado:              obtenido:              resultado
[ 1] struct char e int: b               esperado: 4            obtenido: 4            OK
[ 2] struct de tres campos: c           esperado: 8            obtenido: 8            OK
[ 3] tamano total con padding           esperado: 12           obtenido: 12           OK
[ 4] double seguido de char             esperado: 16           obtenido: 16           OK
[ 5] miembro inexistente                esperado: -1           obtenido: -1           OK
[ 6] struct sin campos                  esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion19.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En cálculos de layout de estructuras en C suelen omitirse el padding final al múltiplo de la mayor alineación de la estructura (por ejemplo, cuando el último campo es más pequeño que un campo intermedio más ancho, como char + int + short), la estructura vacía (total == 0, que debe devolver 0 y no -1) y la devolución de -1 ante miembros inexistentes. Se explicitó en el contrato C-R-E-O el redondeo final por mayor_align y el manejo separado de total == 0 y campos ausentes.

---
**Firma digital:** [984-uriel]

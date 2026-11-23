# Bitácora de Requerimientos y Auditoría IA — Sesión 14
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Declaración vs Uso de Variables  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Detección de variables no declaradas  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir `declarar()` y `usar()` a mano, el punto principal fue mantener el invariante de declaración previa y evitar coincidencias parciales o redeclaraciones duplicadas. Hay que validar que `tabla` y `nombre` no sean `NULL` ni cadenas vacías (`nombre[0] == '\0'`), realizar una búsqueda exacta con `strcmp(...) == 0` para no confundir identificadores como `"x"` y `"xy"`, devolver `0` sin modificar `tabla->total` si el nombre ya estaba registrado, y comprobar `tabla->total < TABLA_MAX` antes de insertar, todo sin advertencias bajo `-std=c11 -Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Invariante de definición previa |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Estoy programando en C11 con GCC usando las banderas -std=c11 -Wall -Wextra -Werror. La estructura `Tabla`, la macro `TABLA_MAX` y las firmas de `declarar` y `usar` ya están definidas en `api_sesion14.h`. Este archivo se compilará junto con `harness_sesion14_c.c`, por lo que NO debe incluir función `main()`.

Rol: Actúa como un ingeniero de software de sistemas y compiladores especializado en análisis semántico y tablas de símbolos.

Especificación: Implementa exactamente dos funciones:
1. `int declarar(Tabla *tabla, const char *nombre)`:
   - Si `tabla == NULL`, `nombre == NULL` o `nombre` es una cadena vacía (`""`), devuelve `-1`.
   - Busca de forma exacta (`strcmp`) si `nombre` ya existe en `tabla->nombres[0 .. tabla->total - 1]`. Si ya existe, devuelve `0` sin modificar `tabla->total` ni insertar duplicados.
   - Si `tabla->total >= TABLA_MAX`, devuelve `-1`.
   - Si no existe y hay espacio, guarda `nombre` en `tabla->nombres[tabla->total]`, incrementa `tabla->total` en 1 y devuelve `1`.
2. `int usar(const Tabla *tabla, const char *nombre)`:
   - Si `tabla == NULL`, `nombre == NULL`, `nombre` está vacío o el nombre no fue declarado previamente con coincidencia exacta, devuelve `0`.
   - Devuelve `1` únicamente si `nombre` ya fue declarado en `tabla`.

Output: Entrega únicamente el código modular en C listo para `ia_sesion14_c.c` (sin `main()`) y un análisis breve de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion14.h"
#include <stddef.h>
#include <string.h>

int declarar(Tabla *tabla, const char *nombre) {
    if (!tabla || !nombre || *nombre == '\0') {
        return -1;
    }

    int pos = 0;
    while (pos < tabla->total) {
        if (tabla->nombres[pos] && strcmp(tabla->nombres[pos], nombre) == 0) {
            return 0;
        }
        pos++;
    }

    if (tabla->total >= TABLA_MAX) {
        return -1;
    }

    tabla->nombres[tabla->total++] = nombre;
    return 1;
}

int usar(const Tabla *tabla, const char *nombre) {
    if (!tabla || !nombre || *nombre == '\0') {
        return 0;
    }

    for (int pos = 0; pos < tabla->total; pos++) {
        if (tabla->nombres[pos] && strcmp(tabla->nombres[pos], nombre) == 0) {
            return 1;
        }
    }

    return 0;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Uso antes de declarar y redeclaración

### Salida del harness
```text
=== AUDITORIA SESION 14 - Declaracion previa y redeclaracion ===
Caso                               esperado:              obtenido:              resultado
[ 1] usar sin declarar                  esperado: 0            obtenido: 0            OK
[ 2] usar despues de declarar           esperado: 1            obtenido: 1            OK
[ 3] redeclarar el mismo nombre         esperado: 0            obtenido: 0            OK
[ 4] total tras redeclarar              esperado: 1            obtenido: 1            OK
[ 5] prefijo no es el mismo nombre      esperado: 0            obtenido: 0            OK
[ 6] declarar nombre vacio              esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion14.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En respuestas genéricas de tablas de símbolos en C suelen pasarse por alto tres casos borde: el nombre vacío (""), que debe retornar -1 en declarar() sin alterar tabla->total; el desbordamiento de capacidad (tabla->total >= TABLA_MAX), que debe retornar -1; y el uso de comparaciones parciales o prefijos (como confundir "x" con "xy"). Se exigió en la especificación C-R-E-O la validación explícita de punteros nulos y cadenas vacías, la verificación del límite TABLA_MAX y el uso estricto de strcmp(...) == 0 tanto en declarar() como en usar().

---
**Firma digital:** [984-uriel]

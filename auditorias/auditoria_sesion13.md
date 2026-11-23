# Bitácora de Requerimientos y Auditoría IA — Sesión 13
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Manejo de Ámbitos (Scope Global/Local)  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Pila de tablas de símbolos (Stack Scopes)  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al implementar `declarar()` y `buscar()` de forma manual, la principal restricción fue respetar la separación estricta entre el ámbito actual y los ámbitos ancestros sin confundir duplicados locales con *shadowing*. En `declarar()`, se debe validar primero si `ambito` o `nombre` son `NULL` o si `nombre` está vacío (`nombre[0] == '\0'`), verificar que el identificador no exista ya dentro del arreglo `nombres[0 .. total-1]` del ámbito actual (devolviendo `0` si ya está) y comprobar que `ambito->total < AMBITO_MAX` antes de insertar. En `buscar()`, el reto es avanzar por `actual = actual->padre` desde el ámbito actual hasta `NULL` y devolver `actual->nombre` (el nombre del ámbito donde se encontró), no el nombre del símbolo, cuidando que el compilador no emita advertencias bajo `-std=c11 -Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Ocultamiento de variables (Shadowing) |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Estoy desarrollando en C11 con GCC usando las banderas -std=c11 -Wall -Wextra -Werror. La estructura `Ambito`, la constante `AMBITO_MAX` y las firmas de `declarar` y `buscar` ya están definidas en `api_sesion13.h`. Este archivo se compilará junto con `harness_sesion13_c.c`, por lo que NO debe incluir `main()`.

Rol: Actúa como un ingeniero de software de sistemas y compiladores especializado en manejo de tablas de símbolos y ámbitos léxicos.

Especificación: Implementa exactamente dos funciones:
1. `int declarar(Ambito *ambito, const char *nombre)`:
   - Si `ambito == NULL`, `nombre == NULL` o `nombre` es una cadena vacía (`""`), devuelve `-1`.
   - No revisa ámbitos padre; el mismo nombre puede declararse en distintos niveles.
   - Si `nombre` ya existe en el arreglo `nombres` del `ambito` actual (hasta `ambito->total`), devuelve `0` sin modificar la estructura.
   - Si `ambito->total >= AMBITO_MAX`, devuelve `-1`.
   - Si se puede insertar, guarda `nombre` en `ambito->nombres[ambito->total]`, incrementa `ambito->total` y devuelve `1`.
2. `const char *buscar(const Ambito *ambito, const char *nombre)`:
   - Si `ambito == NULL`, `nombre == NULL` o `nombre` está vacío, devuelve `NULL`.
   - Busca `nombre` en el ámbito actual y, si no está, sube por `padre` hasta la raíz (`NULL`).
   - Respeta el ocultamiento (shadowing): devuelve el puntero `nombre` del ámbito más interno/cercano donde se encontró el identificador, o `NULL` si no existe en ninguna parte.

Output: Entrega únicamente el código en C limpio y modular listo para `ia_sesion13_c.c` (sin `main()`) y un resumen breve de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion13.h"
#include <stddef.h>
#include <string.h>

int declarar(Ambito *ambito, const char *nombre) {
    if (!ambito || !nombre || *nombre == '\0') {
        return -1;
    }

    for (int idx = 0; idx < ambito->total; idx++) {
        if (ambito->nombres[idx] && strcmp(ambito->nombres[idx], nombre) == 0) {
            return 0;
        }
    }

    if (ambito->total >= AMBITO_MAX) {
        return -1;
    }

    ambito->nombres[ambito->total++] = nombre;
    return 1;
}

const char *buscar(const Ambito *ambito, const char *nombre) {
    if (!ambito || !nombre || *nombre == '\0') {
        return NULL;
    }

    for (const Ambito *nivel = ambito; nivel != NULL; nivel = nivel->padre) {
        for (int idx = 0; idx < nivel->total; idx++) {
            if (nivel->nombres[idx] && strcmp(nivel->nombres[idx], nombre) == 0) {
                return nivel->nombre;
            }
        }
    }

    return NULL;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Búsqueda jerárquica de identificadores

### Salida del harness
```text
=== AUDITORIA SESION 13 - Pila de ambitos y busqueda jerarquica ===
Caso                               esperado:              obtenido:              resultado
[ 1] declarar(&g, "x")                  esperado: 1            obtenido: 1            OK
[ 2] buscar en el mismo ambito          esperado: global       obtenido: global       OK
[ 3] buscar subiendo al padre           esperado: global       obtenido: global       OK
[ 4] shadowing en el hijo               esperado: bloque       obtenido: bloque       OK
[ 5] tres niveles hasta la raiz         esperado: raiz         obtenido: raiz         OK
[ 6] nombre no declarado                esperado: (nulo)       obtenido: (nulo)       OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion13.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En una primera aproximación típica al pedir manejo de ámbitos, un modelo puede descuidar la validación de cadenas vacías (nombre[0] == '\0'), devolver el nombre del identificador en lugar del nombre del ámbito (nivel->nombre) o intentar comprobar duplicados en toda la cadena de padres al llamar a declarar(). Se verificó explícitamente en el contrato C-R-E-O y en la revisión del código que declarar() rechace punteros nulos, cadenas vacías y ámbitos llenos (>= AMBITO_MAX) devolviendo -1, devuelva 0 solo ante duplicados en el ámbito actual, y que buscar() recorra de hijo a padre devolviendo nivel->nombre o NULL sin modificar ninguna estructura.

---
**Firma digital:** [984-uriel]

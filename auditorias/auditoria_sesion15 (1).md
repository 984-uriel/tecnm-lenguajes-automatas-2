# Bitácora de Requerimientos y Auditoría IA — Sesión 15
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Bloques Anidados y Ciclo de Vida  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Operaciones Push y Pop de ámbito  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al programar `push_ambito()` y `pop_ambito()` a mano, el aspecto crítico fue la gestión correcta del heap y la detección de la raíz. En `push_ambito()`, hay que validar `actual != NULL` y `nombre != NULL`, reservar con `malloc(sizeof(Ambito))`, verificar que no falle y enlazar `nuevo->padre = actual`. En `pop_ambito()`, si `actual == NULL` o `actual->padre == NULL` (el ámbito raíz, que además en el harness está en el stack), se debe retornar `NULL` sin llamar a `free()`, y en caso de hacer `free(actual)`, se debe guardar primero `actual->padre` en un puntero auxiliar para evitar accesos a memoria ya liberada (*use-after-free*) y compilar limpios bajo `-std=c11 -Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Liberación de memoria al cerrar bloque |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Estoy programando en C11 con GCC usando las banderas -std=c11 -Wall -Wextra -Werror. La estructura `Ambito` y las firmas de `push_ambito` y `pop_ambito` ya están definidas en `api_sesion15.h`. Este archivo se compilará junto con `harness_sesion15_c.c`, por lo que NO debe incluir `main()`.

Rol: Actúa como un ingeniero de software de sistemas y compiladores especializado en gestión de memoria dinámica y ámbitos anidados.

Especificación: Implementa exactamente dos funciones:
1. `Ambito *push_ambito(Ambito *actual, const char *nombre)`:
   - Si `actual == NULL` o `nombre == NULL`, devuelve `NULL`.
   - Reserva dinámicamente un `Ambito` (`malloc`), verifica que no sea `NULL`, asigna `nombre` y enlaza `padre = actual`, devolviendo el nuevo puntero.
2. `Ambito *pop_ambito(Ambito *actual)`:
   - Si `actual == NULL` o `actual->padre == NULL`, devuelve `NULL` sin liberar memoria (la raíz no debe liberarse ni destruirse).
   - Guarda `Ambito *padre = actual->padre`, libera `actual` con `free(actual)` y devuelve `padre`.

Output: Entrega únicamente el código modular en C listo para `ia_sesion15_c.c` (sin `main()`) y un análisis breve de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion15.h"
#include <stddef.h>
#include <stdlib.h>

Ambito *push_ambito(Ambito *actual, const char *nombre) {
    if (!actual || !nombre) {
        return NULL;
    }

    Ambito *marco = malloc(sizeof(*marco));
    if (!marco) {
        return NULL;
    }

    marco->nombre = nombre;
    marco->padre = actual;
    return marco;
}

Ambito *pop_ambito(Ambito *actual) {
    if (!actual || !actual->padre) {
        return NULL;
    }

    Ambito *superior = actual->padre;
    free(actual);
    return superior;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Aislamiento de variables fuera de bloque

### Salida del harness
```text
=== AUDITORIA SESION 15 - Push y pop de ambitos ===
Caso                               esperado:              obtenido:              resultado
[ 1] nombre del ambito nuevo            esperado: if           obtenido: if           OK
[ 2] profundidad con dos push           esperado: 3            obtenido: 3            OK
[ 3] pop devuelve el padre              esperado: 1            obtenido: 1            OK
[ 4] pop sobre la raiz                  esperado: 1            obtenido: 1            OK
[ 5] push con padre NULL                esperado: 1            obtenido: 1            OK
[ 6] profundidad tras pop               esperado: 2            obtenido: 2            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion15.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Al generar funciones de push y pop sobre listas enlazadas o pilas de ámbitos, una propuesta descuidada suele omitir la comprobación de actual->padre == NULL en pop_ambito(), lo que provocaría un comportamiento indefinido o un intento de free() sobre un nodo raíz en pila/estático, o bien leer actual->padre después de free(actual). Se especificó en el contrato C-R-E-O y se comprobó en el código que pop_ambito() valide !actual || !actual->padre retornando NULL sin liberar, y que guarde el puntero padre antes de invocar free(actual).

---
**Firma digital:** [984-uriel]

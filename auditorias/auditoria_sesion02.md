## Bitácora de Requerimientos y Auditoría IA — Sesión 02

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Gestión en GitHub Web y Nodos Dinámicos  
**Docente Titular:** MCC. Iván Márquez Larios (ijmarquezl)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

### 1. Fase 1: El Artesano (Dificultad Manual)

**Reto:** Lista enlazada de tokens en Heap

Durante la implementación manual fue necesario comprender el uso de memoria dinámica mediante `malloc` y la construcción de una lista enlazada simple. La principal dificultad fue enlazar correctamente los nodos al final de la lista y liberar la memoria sin perder la referencia al siguiente nodo.

También fue importante considerar los casos borde, como listas vacías (`NULL`) para las funciones `contar_tokens()` y `liberar_lista()`, evitando errores de ejecución.

### 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

#### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
|------------------|-------------------|
| **Contexto (C)** | Lenguaje C, GCC, listas enlazadas y memoria dinámica |
| **Rol (R)** | Ingeniero de software especializado en estructuras dinámicas |
| **Especificación (E)** | Implementar crear_nodo, agregar_token, contar_tokens y liberar_lista usando malloc |
| **Output (O)** | Código C modular sin función main() y compatible con el harness |

#### 2.2 Prompt Estructurado

**Contexto:** Estoy realizando una práctica de Lenguajes y Autómatas II en C. Debo implementar una lista enlazada de tokens almacenados dinámicamente en el heap.

**Rol:** Actúa como un desarrollador experto en lenguaje C y estructuras de datos dinámicas.

**Especificación:** Implementa las funciones:

```c
NodoToken *crear_nodo(const char *lexema);
void agregar_token(NodoToken **cabeza, const char *lexema);
int contar_tokens(NodoToken *cabeza);
void liberar_lista(NodoToken *cabeza);
```
---
### 3. Código Devuelto por el Modelo

Se obtuvo una implementación funcional de lista enlazada utilizando memoria dinámica, recorrido de nodos mediante punteros y liberación segura de memoria con `free()`.

```c
#include "api_sesion02.h"
#include <stdlib.h>
#include <string.h>

NodoToken *crear_nodo(const char *l*xema) {
    NodoToken *nuevo = mal*oc*sizeof(NodoToken));

    if (nuevo*== NULL) {
        return NULL;
  * }

    strncpy(nuevo->lexema, lex*ma,*15);
    nuevo->lexema[15] = '\0';*    nuevo->siguiente*= NULL;

    return nuevo;
}

void*agregar_token(NodoToken **cabeza, ***st char *lexema) {
    NodoToken*****vo = crear_nodo(lexema);

   *** (nuevo == NULL) {
        retur****** }

    if (*cabeza == NULL**{
        *cabeza = nuevo;
***     return;
    }

    NodoToke***actual****cabeza;

    while (actual***iguiente != NULL) {
        actu***=***tual->siguiente;
    }

    actu***>siguiente = nuevo;
}

int conta***okens(N***Token *cabeza) {
    int***ntador = 0;

    while (cabe***!= NULL) {
        contador++;
 ***    cabeza***cabeza->siguiente;
    }

    re***n***ntador;
}

void liberar_lista(No***oken ****eza) {
    NodoToken *temp***    while (cabeza != NULL) {
   ***  temp = cabeza;
        cabeza ***abeza->siguiente;
        free(t***);
    }
}
```
---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Prevención de Use-After-Free y cadenas nulas

### Salida del harness
```text
=== AUDITORIA SESION 02 - Lista enlazada de tokens en heap ===

Caso                               esperado:              obtenido:              resultado

[ 1] crear_nodo("if")->lexema           esperado: if           obtenido: if           OK

[ 2] contar_tokens(3 tokens)            esperado: 3            obtenido: 3            OK

[ 3] orden: segundo token               esperado: dos          obtenido: dos          OK

[ 4] ultimo->siguiente == NULL          esperado: 1            obtenido: 1            OK

[ 5] contar_tokens(NULL)                esperado: 0            obtenido: 0            OK

[ 6] liberar_lista(NULL)                esperado: sin crash    obtenido: sin crash    OK


RESULTADO: 6/6 pruebas superadas

TODO EN VERDE. 
```
- [x] Caso base y límites superados sin *Crash*.
- [x] No existen accesos fuera de límites ni *Memory Leaks*.
- [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

**¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?**

La IA generó una implementación funcional de la lista enlazada, sin embargo fue necesario revisar manualmente algunos casos borde importantes:

1. Verificar que `malloc()` no devolviera `NULL` antes de utilizar la memoria reservada.
2. Confirmar que el último nodo de la lista enlazada apuntara correctamente a `NULL`.
3. Comprobar que `contar_tokens(NULL)` devolviera `0` cuando la lista estuviera vacía.
4. Verificar que `liberar_lista(NULL)` no provocara errores de ejecución.
5. Revisar que la función `liberar_lista()` almacenara primero la referencia al siguiente nodo antes de llamar a `free()`, evitando errores de tipo *Use-After-Free*.

Después de estas verificaciones, la implementación cumplió completamente con el contrato definido en `api_sesion02.h` y todas las pruebas del harness fueron superadas satisfactoriamente.

---

### Conclusión

Se implementó una lista enlazada de tokens utilizando memoria dinámica en el heap. La solución permite crear nodos, insertarlos manteniendo el orden, contar elementos y liberar toda la memoria reservada de manera segura. El programa cumplió todos los criterios de aceptación establecidos y obtuvo un resultado de **6/6 pruebas superadas** durante la auditoría.

---

**Firma digital:** [984-uriel]

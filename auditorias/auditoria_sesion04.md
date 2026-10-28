# Bitácora de Requerimientos y Auditoría IA — Sesión 04
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Anatomía de un Árbol Sintáctico (AST)  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Estructura de nodo binario (ASTNode)  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al estructurar el código de manera manual, la principal dificultad radica en asegurar una correcta reserva de memoria en el *heap* (`malloc`) inicializando adecuadamente los punteros de los hijos (`izq`, `der`) y el operador en `\0` para nodos numéricos, evitando comportamientos indefinidos o fallos de segmentación al momento de recorrer recursivamente el árbol.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Gramática E -> E + T | T con tipos de nodo |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado

```text
[Contexto] Estándar C99, compilador GCC bajo estrictas normas de compilación (-Wall -Wextra -Werror).
[Rol] Ingeniero de software experto en diseño de compiladores y parsing.
[Especificación] Implementa las funciones declaradas en api_sesion04.h: crear_numero(int v), crear_binario(char op, ASTNode *izq, ASTNode *der) y evaluar(ASTNode *n). El árbol sintáctico abstracto debe modelar nodos numéricos y binarios (+, -, *). La función evaluar debe retornar -1 si recibe un puntero NULL y procesar recursivamente las operaciones respetando la jerarquía y orden de los subárboles.
[Output] Únicamente el código fuente en C modular, limpio, sin función main() y libre de advertencias del compilador.
```

---

## 3. Código Devuelto por el Modelo

```c

#include "api_sesion04.h"
#include <stdlib.h>

ASTNode *crear_numero(int v) {
    ASTNode *n = (ASTNode *)malloc(sizeof(ASTNode));
    if (!n) return NULL;
    
    *n = (ASTNode){.tipo = NODO_NUM, .valor = v, .op = '\0', .izq = NULL, .der = NULL};
    return n;
}

ASTNode *crear_binario(char op, ASTNode *izq, ASTNode *der) {
    ASTNode *n = (ASTNode *)malloc(sizeof(ASTNode));
    if (!n) return NULL;
    
    *n = (ASTNode){.tipo = NODO_BINOP, .valor = 0, .op = op, .izq = izq, .der = der};
    return n;
}

int evaluar(ASTNode *n) {
    if (!n) return -1;
    if (n->tipo == NODO_NUM) return n->valor;
    
    int izq_val = evaluar(n->izq);
    int der_val = evaluar(n->der);
    
    if (n->op == '+') return izq_val + der_val;
    if (n->op == '-') return izq_val - der_val;
    if (n->op == '*') return izq_val * der_val;
    
    return -1;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Árbol vacío y evaluación de nodo raíz

### Salida del harness
```text
=== AUDITORIA SESION 04 - Nodo binario de AST ===
Caso                              esperado:             obtenido:             resultado
[ 1] evaluar(nodo 42)             esperado: 42          obtenido: 42          OK
[ 2] evaluar(3+5*2)               esperado: 13          obtenido: 13          OK
[ 3] hijo izquierdo de (* 5 2)    esperado: 5           obtenido: 5           OK
[ 4] operador guardado            esperado: *           obtenido: *           OK
[ 5] evaluar(NULL)                esperado: -1          obtenido: -1          OK
[ 6] evaluar(10-4)                esperado: 6           obtenido: 6           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE.
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> La IA inicialmente generó una estructura recursiva directa en evaluar, pero omitió verificar de inmediato la condición de seguridad para evaluar(NULL) al inicio de la función en algunas iteraciones, lo que habría provocado un fallo de segmentación (Segmentation Fault) al evaluar hojas faltantes o nodos vacíos. Se solucionó asegurando que la primera validación del método sea if (!n) return -1; antes de consultar los tipos de nodos o sus hijos.

---
**Firma digital:** [984-uriel]

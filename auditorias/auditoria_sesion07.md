# Bitácora de Requerimientos y Auditoría IA — Sesión 07
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Evaluación de Expresiones Aritméticas  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Evaluador recursivo de operaciones  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al codificar a mano, la principal dificultad residió en asegurar el orden correcto de evaluación recursiva y la propagación inmediata de los códigos de error (`*error = 1` para división/módulo entre cero y `*error = 2` para punteros nulos o nodos inválidos). Fue imprescindible comprobar la validez del puntero y del divisor antes de intentar realizar cualquier operación de división o módulo para evitar fallos de segmentación (*segmentation fault*) o cierres inesperados del programa en tiempo de ejecución.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Manejo estricto de división entre cero |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[Contexto]: Estoy desarrollando la fase de evaluación de expresiones aritméticas para un compilador en lenguaje C (estándar C11, compilado con -Wall -Wextra -Werror). El árbol de sintaxis abstracta (AST) está definido en "api_sesion07.h" con la estructura ASTNode.

[Rol]: Actúa como un Ingeniero de Software de Sistemas y Compiladores experto en C.

[Especificación]: Implementa la función recursiva "int evaluar(ASTNode *n, int *error);" respetando rigurosamente las siguientes reglas:
1. Si el puntero del árbol 'n' es NULL, asigna *error = 2 y retorna 0 sin desplomarse.
2. Si *error ya es distinto de 0, retorna 0 de inmediato sin seguir evaluando.
3. Si el nodo es NODO_NUM, retorna su valor.
4. Si el nodo es NODO_BINOP, evalúa recursivamente el hijo izquierdo y luego el derecho. Si ocurre un error en cualquier hijo, propágalo hacia arriba de inmediato.
5. Antes de realizar operaciones de división ('/') o módulo ('%'), verifica estáticamente si el valor del hijo derecho es 0. Si es 0, asigna *error = 1 y retorna 1.
6. Incluye los encabezados necesarios (como <stddef.h>) para definir la macro NULL.

[Output]: Proporciona únicamente el código fuente en C de la implementación de la función "evaluar", listo para compilar en ia_sesion07_c.c, sin incluir la función main().
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion07.h"
#include <stddef.h>

int evaluar(ASTNode *n, int *error) {
    if (error == NULL) return 0;
    
    if (n == NULL) {
        *error = 2;
        return 0;
    }

    if (*error != 0) return 0;

    if (n->tipo == NODO_NUM) {
        return n->valor;
    }

    if (n->tipo == NODO_BINOP) {
        int izq = evaluar(n->izq, error);
        if (*error != 0) return 0;

        int der = evaluar(n->der, error);
        if (*error != 0) return 0;

        if ((n->op == '/' || n->op == '%') && der == 0) {
            *error = 1;
            return 1;
        }

        switch (n->op) {
            case '+': return izq + der;
            case '-': return izq - der;
            case '*': return izq * der;
            case '/': return izq / der;
            case '%': return izq % der;
            default:
                *error = 2;
                return 0;
        }
    }

    *error = 2;
    return 0;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Expresiones complejas y división por 0

### Salida del harness
```text
=== AUDITORIA SESION 07 - Evaluador recursivo con deteccion de errores ===
Caso                                esperado:             obtenido:             resultado
[ 1] (2+3)*4                        esperado: 20           obtenido: 20           OK
[ 2] 7/0 -> error 1                 esperado: 1            obtenido: 1            OK
[ 3] 7%0 -> error 1                 esperado: 1            obtenido: 1            OK
[ 4] arbol NULL -> error 2          esperado: 2            obtenido: 2            OK
[ 5] 10-2-3                         esperado: 5            obtenido: 5            OK
[ 6] (8/2)+1                        esperado: 5            obtenido: 5            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE.
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En la primera iteración del código generado por la IA, se omitió la inclusión explícita de la biblioteca <stddef.h> (o <stdio.h>), la cual define la macro NULL. Debido a que el arnés de auditoría compila bajo reglas estrictas (-Wall -Wextra -Werror), la falta de este encabezado provocó un error fatal de compilación por identificador no declarado ('NULL' undeclared). El problema se resolvió agregando la directiva #include <stddef.h> al inicio del archivo ia_sesion07_c.c, con lo que la compilación y la suite de pruebas automatizadas pasaron al 100% en verde.

---
**Firma digital:** [984-uriel]

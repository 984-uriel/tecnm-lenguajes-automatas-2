# Bitácora de Requerimientos y Auditoría IA — Sesión 05
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Recorridos del AST y Notación Polaca  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Impresión en Postorden / RPN  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al implementar el recorrido de manera manual, el principal desafío fue controlar de forma estricta los límites del búfer (`cap`) y el manejo de los índices para evitar desbordamientos (*buffer overflow*) al concatenar los espacios y los tokens. Asimismo, se requirió asegurar que no quedaran espacios sobrantes al inicio o al final de la cadena resultante, manteniendo el orden estricto de visita: izquierdo, derecho y raíz.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Invariantes de recorrido recursivo |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[Contexto]: Estás programando un compilador en C99 bajo estándares estrictos de compilación (-Wall -Wextra -Werror). 
[Rol]: Eres un ingeniero de software especializado en la construcción de compiladores y manejo eficiente de memoria estática.
[Especificación]: Escribe la función `void postorden(ASTNode *n, char *buf, int cap);` que realice un recorrido postorden (izquierdo, derecho, raíz) de un Árbol de Sintaxis Abstracta (AST) y almacene la Notación Polaca Inversa (RPN) separada por espacios simples en el búfer proporcionado. Debes validar punteros nulos, respetar estrictamente la capacidad máxima del búfer y asegurar que termine en '\0' sin imprimir mediante printf.
[Output]: Únicamente el código en C de la función implementada y auxiliares necesarios, sin incluir la función main().
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion05.h"
#include <stdio.h>
#include <string.h>

static void generar_rpn(ASTNode *nodo, char *buffer, int capacidad, int *posicion) {
    if (nodo == NULL) return;

    /* Izquierdo -> Derecho -> Raíz */
    generar_rpn(nodo->izq, buffer, capacidad, posicion);
    generar_rpn(nodo->der, buffer, capacidad, posicion);

    if (*posicion >= capacidad - 1) return;

    char temp[32];
    if (nodo->tipo == NODO_NUM) {
        snprintf(temp, sizeof(temp), "%d", nodo->valor);
    } else {
        temp[0] = nodo->op;
        temp[1] = '\0';
    }

    int espacio = (*posicion > 0) ? 1 : 0;
    int t_len = strlen(temp);

    if (*posicion + espacio + t_len < capacidad) {
        if (espacio) {
            buffer[*posicion] = ' ';
            (*posicion)++;
        }
        strcpy(buffer + *posicion, temp);
        *posicion += t_len;
    }
}

void postorden(ASTNode *n, char *buf, int cap) {
    if (!buf || cap <= 0) return;
    buf[0] = '\0';
    
    if (!n) return;

    int idx = 0;
    generar_rpn(n, buf, cap, &idx);
    buf[idx] = '\0';
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Validación contra desbordamiento de pila

### Salida del harness
```text
=== AUDITORIA SESION 05 - Recorrido postorden y RPN ===
Caso                              esperado:             obtenido:             resultado
[ 1] hoja 42                        esperado: 42          obtenido: 42          OK
[ 2] 3+(5*2)                        esperado: 3 5 2 * +   obtenido: 3 5 2 * +   OK
[ 3] arbol NULL                     esperado:             obtenido:             OK
[ 4] (1+2)*(3+4)
     esperado: 1 2 + 3 4 + *
     obtenido: 1 2 + 3 4 + *
     -> OK
[ 5] 8-3                            esperado: 8 3 -       obtenido: 8 3 -       OK

RESULTADO: 5/5 pruebas superadas
TODO EN VERDE.
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
>La propuesta inicial generada por la IA no validaba de forma robusta si el búfer de entrada era nulo o si la capacidad asignada era menor o igual a cero (cap <= 0), lo cual podía desencadenar un fallo de segmentación ante llamadas con punteros inválidos. Esto se solucionó añadiendo una guarda explícita al inicio de la función principal postorden para asegurar la estabilidad ante entradas vacías o punteros nulos. 

---
**Firma digital:** [984-uriel]

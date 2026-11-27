# Bitácora de Requerimientos y Auditoría IA — Sesión 28
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Optimización: Constant Folding  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Plegado y simplificación de constantes  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir plegar() a mano en C, el reto principal es distinguir con precisión cuándo un operando es un entero completo (por ejemplo usando strtol y verificando el terminador '\0') frente a una variable o identificador como "x" o "y", respetando siempre la capacidad dst con snprintf. También se debe cuidar la división entre cero para no provocar excepciones ni plegar la instrucción, aplicar identidades algebraicas como x + 0, x * 1 y x * 0, y compilar limpio bajo -std=c11 -Wall -Wextra -Werror.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Reducción estática en compilación |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto:
Estoy programando en C11 con GCC (compilado con -std=c11 -Wall -Wextra -Werror) para la materia de Lenguajes y Autómatas II. La cabecera api_sesion28.h ya define la firma void plegar(char *dst, int cap, const char *res, const char *op, const char *a, const char *b) y no debe modificarse. El archivo se compilará junto con harness_sesion28_c.c, por lo que no debe incluir main().

Rol:
Actúa como un ingeniero de software de sistemas y compiladores especializado en optimización local de código de tres direcciones (TAC).

Especificación:
Implementa void plegar(...) en C siguiendo estas reglas:
1. Valida punteros no nulos y cap > 0 antes de escribir con snprintf en dst.
2. Si ambos operandos a y b son enteros completos válidos, pliega +, - y * calculando el resultado literal; en "/" solo pliega si el divisor es distinto de cero.
3. Aplica identidades algebraicas: x + 0 y 0 + x se reducen a x; x * 1 y 1 * x se reducen a x; x * 0 y 0 * x se reducen a 0.
4. Si no es posible plegar o el divisor es cero (como 10 / 0), emite la instrucción original "<res> = <a> <op> <b>".

Output:
Devuelve únicamente el código en C limpio y modular para ia_sesion28_c.c, incluyendo #include "api_sesion28.h" y las cabeceras estándar necesarias (<stdio.h>, <stdlib.h>, <string.h>, <ctype.h>), sin función main() y sin advertencias del compilador.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion28.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_int_completo(const char *str, long *out) {
    char *fin = NULL;
    long n;

    if (!str || *str == '\0' || isspace((unsigned char)*str)) {
        return 0;
    }

    n = strtol(str, &fin, 10);
    if (fin == str || *fin != '\0') {
        return 0;
    }

    if (out) {
        *out = n;
    }
    return 1;
}

void plegar(char *dst, int cap, const char *res, const char *op,
            const char *a, const char *b) {
    long lhs = 0;
    long rhs = 0;
    int ok_a;
    int ok_b;

    if (!dst || cap <= 0 || !res || !op || !a || !b) {
        return;
    }

    ok_a = parse_int_completo(a, &lhs);
    ok_b = parse_int_completo(b, &rhs);

    if (ok_a && ok_b) {
        if (strcmp(op, "+") == 0) {
            snprintf(dst, (size_t)cap, "%s = %ld", res, lhs + rhs);
            return;
        }
        if (strcmp(op, "-") == 0) {
            snprintf(dst, (size_t)cap, "%s = %ld", res, lhs - rhs);
            return;
        }
        if (strcmp(op, "*") == 0) {
            snprintf(dst, (size_t)cap, "%s = %ld", res, lhs * rhs);
            return;
        }
        if (strcmp(op, "/") == 0 && rhs != 0) {
            snprintf(dst, (size_t)cap, "%s = %ld", res, lhs / rhs);
            return;
        }
    }

    if (strcmp(op, "+") == 0) {
        if (ok_b && rhs == 0) {
            snprintf(dst, (size_t)cap, "%s = %s", res, a);
            return;
        }
        if (ok_a && lhs == 0) {
            snprintf(dst, (size_t)cap, "%s = %s", res, b);
            return;
        }
    } else if (strcmp(op, "*") == 0) {
        if ((ok_a && lhs == 0) || (ok_b && rhs == 0)) {
            snprintf(dst, (size_t)cap, "%s = 0", res);
            return;
        }
        if (ok_b && rhs == 1) {
            snprintf(dst, (size_t)cap, "%s = %s", res, a);
            return;
        }
        if (ok_a && lhs == 1) {
            snprintf(dst, (size_t)cap, "%s = %s", res, b);
            return;
        }
    }

    snprintf(dst, (size_t)cap, "%s = %s %s %s", res, a, op, b);
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Simplificación de expresiones constantes

### Salida del harness
```text
=== AUDITORIA SESION 28 - Plegado de constantes e identidades ===
Caso                               esperado:              obtenido:              resultado
[ 1] 3 + 5                              esperado: _t0 = 8      obtenido: _t0 = 8      OK
[ 2] 2 * 3                              esperado: _t0 = 6      obtenido: _t0 = 6      OK
[ 3] 7 - 10                             esperado: _t0 = -3     obtenido: _t0 = -3     OK
[ 4] x + 0                              esperado: _t0 = x      obtenido: _t0 = x      OK
[ 5] x + y (sin plegar)                 esperado: _t0 = x + y  obtenido: _t0 = x + y  OK
[ 6] 10 / 0 (no plegar)                 esperado: _t0 = 10 / 0 obtenido: _t0 = 10 / 0 OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion28.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Sin especificación explícita en el prompt, un modelo puede intentar evaluar división entre cero directamente o aceptar cadenas parcialmente numéricas como enteros completos. Lo resolví exigiendo en el contrato C-R-E-O el parseo estricto de enteros completos, la conservación de la instrucción original ante divisor cero (rhs == 0), la escritura acotada con snprintf y la comprobación de ambos binarios con el harness hasta obtener 6/6 pruebas superadas.

---
**Firma digital:** [984-uriel]

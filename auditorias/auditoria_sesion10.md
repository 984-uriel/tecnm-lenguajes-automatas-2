# Bitácora de Requerimientos y Auditoría IA — Sesión 10
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Hito 1: Mini-Evaluador de Expresiones  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Integración AST y Tipado Estático  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al codificar a mano, el principal reto fue manejar correctamente el avance del puntero de lectura (`Parser *p`) a través de las llamadas recursivas sin perder la posición original ni omitir caracteres imprevistos. También fue fundamental garantizar que los espacios en blanco se consumieran adecuadamente antes y después de validar los operandos u operadores, así como distinguir de forma estricta entre un error de división entre cero (`error = 1`) y un error de sintaxis incompleta o mala agrupación de paréntesis (`error = 2`).

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Contrato formal de Frontend Semántico |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Compiladores en C11/C99 con GCC usando las banderas -Wall -Wextra -Werror.
Rol: Ingeniero experto en sistemas y análisis sintáctico (compiladores).
Especificación: Implementa las funciones parser de un evaluador por descenso recursivo para expresiones aritméticas respetando la interfaz de api_sesion10.h. La función principal es long evaluar_expresion(const char *fuente, int *error). Debe manejar enteros, operadores (+, -, *, /), espacios en blanco y paréntesis. Si ocurre división entre cero debe asignar *error = 1. Si hay un error de sintaxis o caracteres no consumidos debe asignar *error = 2. No incluyas la función main().
Output: Código C limpio, eficiente y compatible con las firmas del contrato.
```

---

## 3. Código Devuelto por el Modelo
```c
/* ia_sesion10_c.c - Fase 2: El Cientifico
 * Mini-evaluador de expresiones (Hito 1) - Variante optimizada.
 */
#include "api_sesion10.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char *ptr;
    int err_code;
} LexParser;

static void omitir_espacios(LexParser *lp) {
    while (*lp->ptr == ' ') {
        lp->ptr++;
    }
}

static long parse_expr(LexParser *lp);

static long parse_primaria(LexParser *lp) {
    omitir_espacios(lp);
    if (*lp->ptr == '(') {
        lp->ptr++;
        long val = parse_expr(lp);
        omitir_espacios(lp);
        if (*lp->ptr == ')') {
            lp->ptr++;
        } else {
            lp->err_code = 2;
        }
        return val;
    }
    if (isdigit((unsigned char)*lp->ptr)) {
        long val = 0;
        while (isdigit((unsigned char)*lp->ptr)) {
            val = val * 10 + (*lp->ptr - '0');
            lp->ptr++;
        }
        return val;
    }
    lp->err_code = 2;
    return 0;
}

static long parse_termino(LexParser *lp) {
    long val = parse_primaria(lp);
    while (lp->err_code == 0) {
        omitir_espacios(lp);
        char op = *lp->ptr;
        if (op != '*' && op != '/') break;
        lp->ptr++;
        long der = parse_primaria(lp);
        if (lp->err_code != 0) break;
        if (op == '*') {
            val *= der;
        } else {
            if (der == 0) {
                lp->err_code = 1; // Error 1: Division entre cero
                return 0;
            }
            val /= der;
        }
    }
    return val;
}

static long parse_expr(LexParser *lp) {
    long val = parse_termino(lp);
    while (lp->err_code == 0) {
        omitir_espacios(lp);
        char op = *lp->ptr;
        if (op != '+' && op != '-') break;
        lp->ptr++;
        long der = parse_termino(lp);
        if (lp->err_code != 0) break;
        if (op == '+') {
            val += der;
        } else {
            val -= der;
        }
    }
    return val;
}

long evaluar_expresion(const char *fuente, int *error) {
    static int status_error;
    if (error == NULL) error = &status_error;
    *error = 0;
    if (!fuente) {
        *error = 2;
        return 0;
    }
    LexParser lp = { fuente, 0 };
    long resultado = parse_expr(&lp);
    omitir_espacios(&lp);
    if (lp.err_code == 0 && *lp.ptr != '\0') {
        lp.err_code = 2;
    }
    *error = lp.err_code;
    return (*error != 0) ? 0 : resultado;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Precedencia, paréntesis, división entre cero y sintaxis inválida

### Salida del harness
```text
=== AUDITORIA SESION 10 - Mini-evaluador de expresiones (Hito 1) ===
Caso                                esperada:              obtenido:              resultado
[ 1] "2+3*4"                        esperado: 14           obtenido: 14           OK
[ 2] "(2+3)*4"                      esperado: 20           obtenido: 20           OK
[ 3] "10/0" -> error 1              esperado: 1            obtenido: 1            OK
[ 4] "2+" -> error 2                esperado: 2            obtenido: 2            OK
[ 5] "   7 "                        esperado: 7            obtenido: 7            OK
[ 6] "5-10"                         esperado: -5           obtenido: -5           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE.
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> La IA inicialmente omitió el consumo de espacios en blanco finales al concluir la evaluación de la expresión. Esto ocasionaba que cadenas como `"  7 "` o `"2+3 "` conservaran caracteres finales no procesados y se marcaran incorrectamente con `error = 2`. Se solucionó añadiendo una llamada explícita a la función para omitir espacios en blanco justo antes de validar si el puntero llegó al caracter nulo `\0`.

---
**Firma digital:** [984-uriel]

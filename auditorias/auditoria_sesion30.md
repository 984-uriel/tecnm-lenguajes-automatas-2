# Bitácora de Requerimientos y Auditoría IA — Sesión 30
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Hito 3: Generador de TAC End-to-End  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Traductor completo de AST a TAC  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al traducir un AST binario a TAC optimizado en C, el reto principal es propagar correctamente el lugar (literal plegado o temporal _tK) de los subárboles hacia el nodo padre sin emitir instrucciones redundantes cuando ambos operandos ya se resolvieron a constantes o cuando aplica una identidad algebraica. También hay que cuidar el tamaño de los búferes intermedios frente a lineas[][64] y resultado[cap] para evitar truncamientos en snprintf bajo -std=c11 -Wall -Wextra -Werror, y no plegar divisiones entre cero.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Contrato de Emisión Lineal Optimizada |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto:
Estoy programando en C11 con GCC (compilado con -std=c11 -Wall -Wextra -Werror) para la materia de Lenguajes y Autómatas II. La cabecera api_sesion30.h define la estructura Nodo ('N' número, 'V' variable, 'B' operador binario con izq/der) y la firma int generar_tac_opt(const Nodo *raiz, char lineas[][64], int max, char *resultado, int cap). No debe incluirse main().

Rol:
Actúa como un ingeniero de software de sistemas y compiladores especializado en traducción dirigida por sintaxis, generación de código de tres direcciones (TAC) y optimización local.

Especificación:
Implementa generar_tac_opt(...) en C siguiendo estas reglas:
1. Evalúa recursivamente subárboles y devuelve en lugar el nombre del literal o temporal (_t0, _t1, ...) que contiene el valor resultante.
2. Si ambos operandos evaluados son literales enteros (incluyendo subárboles ya plegados a constante), pliega +, -, * y / (solo si el divisor != 0) en un literal sin emitir línea TAC; si el subárbol completo es constante, no emite líneas y escribe el valor final (ej. "45") en resultado.
3. Resuelve identidades algebraicas como x + 0 -> x, x * 1 -> x y x * 0 -> 0 sin emitir operación.
4. Acota los búferes locales y llamadas a snprintf para no provocar advertencias -Wformat-truncation bajo -Werror.

Output:
Devuelve únicamente el código en C limpio y modular para ia_sesion30_c.c, incluyendo #include "api_sesion30.h" y las cabeceras estándar necesarias (<ctype.h>, <stdio.h>, <stdlib.h>), sin función main() y sin advertencias del compilador.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion30.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ContextoOpt {
    char (*lineas)[64];
    int max;
    int emitidas;
    int temp_id;
} ContextoOpt;

static int parse_entero_lugar(const char *s, long *out_val) {
    char *end = NULL;
    long v;

    if (!s || *s == '\0' || isspace((unsigned char)*s)) {
        return 0;
    }

    v = strtol(s, &end, 10);
    if (end == s || *end != '\0') {
        return 0;
    }

    if (out_val) {
        *out_val = v;
    }
    return 1;
}

static void set_lugar(char *dst, int cap, const char *val) {
    if (!dst || cap <= 0 || !val) {
        return;
    }
    snprintf(dst, (size_t)cap, "%s", val);
}

static void push_tac(ContextoOpt *ctx, const char *instr) {
    if (!ctx || !ctx->lineas || ctx->emitidas < 0 || ctx->emitidas >= ctx->max) {
        return;
    }
    snprintf(ctx->lineas[ctx->emitidas], 64, "%s", instr);
    ctx->emitidas++;
}

static void bajar_expresion(const Nodo *nodo, ContextoOpt *ctx,
                            char *lugar_out, int cap_out) {
    char l_izq[16] = {0};
    char l_der[16] = {0};
    long v_izq = 0;
    long v_der = 0;
    int es_num_izq;
    int es_num_der;
    char temp_nombre[12];
    char linea_tac[64];

    if (!nodo || !lugar_out || cap_out <= 0) {
        return;
    }

    if (nodo->tipo == 'N' || nodo->tipo == 'V') {
        set_lugar(lugar_out, cap_out, nodo->valor);
        return;
    }

    if (nodo->tipo == 'B') {
        bajar_expresion(nodo->izq, ctx, l_izq, (int)sizeof(l_izq));
        bajar_expresion(nodo->der, ctx, l_der, (int)sizeof(l_der));

        es_num_izq = parse_entero_lugar(l_izq, &v_izq);
        es_num_der = parse_entero_lugar(l_der, &v_der);

        if (es_num_izq && es_num_der) {
            if (nodo->op == '+') {
                snprintf(lugar_out, (size_t)cap_out, "%ld", v_izq + v_der);
                return;
            }
            if (nodo->op == '-') {
                snprintf(lugar_out, (size_t)cap_out, "%ld", v_izq - v_der);
                return;
            }
            if (nodo->op == '*') {
                snprintf(lugar_out, (size_t)cap_out, "%ld", v_izq * v_der);
                return;
            }
            if (nodo->op == '/' && v_der != 0) {
                snprintf(lugar_out, (size_t)cap_out, "%ld", v_izq / v_der);
                return;
            }
        }

        if (nodo->op == '+') {
            if (es_num_der && v_der == 0) {
                set_lugar(lugar_out, cap_out, l_izq);
                return;
            }
            if (es_num_izq && v_izq == 0) {
                set_lugar(lugar_out, cap_out, l_der);
                return;
            }
        } else if (nodo->op == '*') {
            if ((es_num_izq && v_izq == 0) || (es_num_der && v_der == 0)) {
                set_lugar(lugar_out, cap_out, "0");
                return;
            }
            if (es_num_der && v_der == 1) {
                set_lugar(lugar_out, cap_out, l_izq);
                return;
            }
            if (es_num_izq && v_izq == 1) {
                set_lugar(lugar_out, cap_out, l_der);
                return;
            }
        }

        snprintf(temp_nombre, sizeof(temp_nombre), "_t%d", ctx->temp_id++);
        snprintf(linea_tac, sizeof(linea_tac), "%s = %s %c %s",
                 temp_nombre, l_izq, nodo->op, l_der);
        push_tac(ctx, linea_tac);
        set_lugar(lugar_out, cap_out, temp_nombre);
    }
}

int generar_tac_opt(const Nodo *raiz, char lineas[][64], int max,
                    char *resultado, int cap) {
    ContextoOpt ctx;

    if (resultado && cap > 0) {
        resultado[0] = '\0';
    }
    if (!raiz || !lineas || max <= 0 || !resultado || cap <= 0) {
        return 0;
    }

    ctx.lineas = lineas;
    ctx.max = max;
    ctx.emitidas = 0;
    ctx.temp_id = 0;

    bajar_expresion(raiz, &ctx, resultado, cap);
    return ctx.emitidas;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Suite de 5 programas complejos

### Salida del harness
```text
=== AUDITORIA SESION 30 - AST a TAC optimizado de punta a punta ===
Caso                               esperado:              obtenido:              resultado
[ 1] x + 2 * 3 (linea)                  esperado: _t0 = x + 6  obtenido: _t0 = x + 6  OK
[ 2] x + 2 * 3 (resultado)              esperado: _t0          obtenido: _t0          OK
[ 3] x * y + 2 * 3
     esperado: _t1 = _t0 + 6
     obtenido: _t1 = _t0 + 6
     -> OK
[ 4] todo constante: sin lineas         esperado: 0            obtenido: 0            OK
[ 5] todo constante: resultado          esperado: 45           obtenido: 45           OK
[ 6] x + 0 (identidad)                  esperado: x            obtenido: x            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion30.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Al compilar con -std=c11 -Wall -Wextra -Werror, GCC señaló posible truncamiento en snprintf al formatear instrucciones con búferes locales de 32 bytes hacia un destino de 64 bytes; además, al evaluar un subárbol binario plegado (como (2 + 3) * (4 + 5)), verificar solo nodo->tipo == 'N' omitía que el lugar del hijo ya contenía una constante plegada y emitía un temporal innecesario. Lo resolví ajustando los tamaños de búfer locales a 16 bytes, evaluando si la cadena del lugar resultante de cada hijo es un entero completo para propagar el plegado en árboles multinivel y comprobando ambos binarios con el harness hasta obtener 6/6 pruebas superadas.

---
**Firma digital:** [984-uriel]

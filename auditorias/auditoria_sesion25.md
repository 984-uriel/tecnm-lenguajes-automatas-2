# Bitácora de Requerimientos y Auditoría IA — Sesión 25
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Control de Flujo: Bucles while y for  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Estructura de ciclo en instrucciones TAC  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir `emitir_while()` y `emitir_for()` sin memoria dinámica ni variables globales, fue clave reservar dos etiquetas consecutivas (`l_ini` y `l_fin`) por ciclo avanzando `*etiquetas`, ubicar `post` antes de `GOTO L_ini` en `for`, omitir la línea cuando `inicio` o `post` es `NULL` y validar `*n < max` y 64 bytes por línea compilando sin advertencias bajo `-Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Garantía de actualización de iterador |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[CONTEXTO]
Estoy cursando Lenguajes y Autómatas II (SCD-1016) en el track de C (estándar C11, compilado con gcc -std=c11 -Wall -Wextra -Werror). Debo implementar `emitir_while` y `emitir_for` declaradas en `api_sesion25.h` para emitir el esqueleto de código de tres direcciones (TAC) de bucles en `lineas[][64]` a partir de `*n` y `*etiquetas`, sin `main()`, sin variables globales y sin memoria dinámica.

[ROL]
Actúa como ingeniero de software de sistemas y compiladores especializado en generación de código intermedio y estructuras de control.

[ESPECIFICACIÓN]
Implementa exactamente estas dos funciones respetando las firmas de `api_sesion25.h`:
1. `void emitir_while(char lineas[][64], int max, int *n, const char *cond, int *etiquetas);`
   - Reserva dos etiquetas (`L_ini` y `L_fin`) incrementando `*etiquetas` dos veces y emite en orden: `"L<ini>:"`, `"IFFALSE <cond> GOTO L<fin>"`, `"GOTO L<ini>"` y `"L<fin>:"`, actualizando `*n`.
2. `void emitir_for(char lineas[][64], int max, int *n, const char *inicio, const char *cond, const char *post, int *etiquetas);`
   - Reserva dos etiquetas (`L_ini` y `L_fin`) incrementando `*etiquetas` dos veces y emite: `inicio` (si no es `NULL` ni vacío), `"L<ini>:"`, `"IFFALSE <cond> GOTO L<fin>"`, `post` (si no es `NULL` ni vacío; si es `NULL`, se omite), `"GOTO L<ini>"` y `"L<fin>:"`, actualizando `*n`.
- Incluye `<stdarg.h>` si usas funciones variádicas, valida punteros nulos y `max <= 0`, y nunca escribas más de `max` líneas ni más de 63 caracteres útiles por línea.

[OUTPUT]
Entrega únicamente el código C modular listo para pegar en `ia_sesion25_c.c` (sin `main()`) y un breve análisis de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion25.h"
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

static void push_linea(char lineas[][64], int max, int *idx, const char *fmt, ...) {
    if (lineas == NULL || idx == NULL || fmt == NULL || *idx < 0 || *idx >= max) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    (void)vsnprintf(lineas[*idx], (size_t)64, fmt, args);
    va_end(args);
    (*idx)++;
}

void emitir_while(char lineas[][64], int max, int *n, const char *cond, int *etiquetas) {
    if (lineas == NULL || max <= 0 || n == NULL || etiquetas == NULL) {
        return;
    }

    int etiqueta_inicio = (*etiquetas)++;
    int etiqueta_fin = (*etiquetas)++;
    const char *expr_cond = (cond != NULL && cond[0] != '\0') ? cond : "_";

    push_linea(lineas, max, n, "L%d:", etiqueta_inicio);
    push_linea(lineas, max, n, "IFFALSE %s GOTO L%d", expr_cond, etiqueta_fin);
    push_linea(lineas, max, n, "GOTO L%d", etiqueta_inicio);
    push_linea(lineas, max, n, "L%d:", etiqueta_fin);
}

void emitir_for(char lineas[][64], int max, int *n, const char *inicio,
                const char *cond, const char *post, int *etiquetas) {
    if (lineas == NULL || max <= 0 || n == NULL || etiquetas == NULL) {
        return;
    }

    int etiqueta_inicio = (*etiquetas)++;
    int etiqueta_fin = (*etiquetas)++;
    const char *expr_cond = (cond != NULL && cond[0] != '\0') ? cond : "_";

    if (inicio != NULL && inicio[0] != '\0') {
        push_linea(lineas, max, n, "%s", inicio);
    }

    push_linea(lineas, max, n, "L%d:", etiqueta_inicio);
    push_linea(lineas, max, n, "IFFALSE %s GOTO L%d", expr_cond, etiqueta_fin);

    if (post != NULL && post[0] != '\0') {
        push_linea(lineas, max, n, "%s", post);
    }

    push_linea(lineas, max, n, "GOTO L%d", etiqueta_inicio);
    push_linea(lineas, max, n, "L%d:", etiqueta_fin);
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Bucles anidados y condiciones de parada

### Salida del harness
```text
=== AUDITORIA SESION 25 - Esqueleto TAC de while y for ===
Caso                               esperado:              obtenido:              resultado
[ 1] while: marca de inicio             esperado: L0:          obtenido: L0:          OK
[ 2] while: condicion
     esperado: IFFALSE a < b GOTO L1
     obtenido: IFFALSE a < b GOTO L1
     -> OK
[ 3] dos ciclos sin colision            esperado: L2:          obtenido: L2:          OK
[ 4] for: inicializacion                esperado: i = 0        obtenido: i = 0        OK
[ 5] for: actualizacion                 esperado: i = i + 1    obtenido: i = i + 1    OK
[ 6] for sin post (NULL)                esperado: 5            obtenido: 5            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion25.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Al usar va_list / vsnprintf en push_linea y omitir post == NULL o *idx < max, una propuesta inicial sin #include <stdarg.h> falla al compilar bajo -std=c11 -Wall -Wextra -Werror (va_list no declarado) y puede emitir líneas vacías o exceder max si no se filtra post != NULL && post[0] != '\0' y *idx < max. Se resolvió incluyendo <stdarg.h>, validando los punteros y post != NULL antes de emitir, y acotando *idx < max, manteniendo complejidad temporal O(1) y espacial O(1). 

---
**Firma digital:** [984-uriel]

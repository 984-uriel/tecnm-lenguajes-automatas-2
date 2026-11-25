# Bitácora de Requerimientos y Auditoría IA — Sesión 24
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Control de Flujo: Sentencia if-else  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Saltos condicionales y evaluación booleana  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir `emitir_condicion()` sin memoria dinámica ni variables globales, fue clave asociar `'&'` con `IFFALSE` y `'|'` con `IFTRUE` hacia la misma `etiqueta` de salida, emitir un segundo salto únicamente cuando `b != NULL` y haya espacio (`emitidas < max`), y formatear cada línea con `snprintf` sobre 64 bytes (`63` caracteres + `'\0'`) sin advertencias bajo `-Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Cortocircuito lógico en expresiones |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[CONTEXTO]
Estoy cursando Lenguajes y Autómatas II (SCD-1016) en el track de C (estándar C11, compilado con gcc -std=c11 -Wall -Wextra -Werror). Debo implementar `int emitir_condicion(char lineas[][64], int max, const char *a, const char *b, char op, const char *etiqueta);` declarada en `api_sesion24.h` para emitir saltos condicionales con cortocircuito hacia `etiqueta`, sin `main()`, sin variables globales y sin memoria dinámica.

[ROL]
Actúa como ingeniero de software de sistemas y compiladores especializado en generación de código intermedio y control de flujo.

[ESPECIFICACIÓN]
Implementa `emitir_condicion` respetando exactamente la firma de `api_sesion24.h`:
- Si `op == '&'`, usa el mnemónico `IFFALSE`; si `op == '|'`, usa `IFTRUE`.
- Escribe líneas con el formato `"<MNEMONICO> <operando> GOTO <etiqueta>"` (por ejemplo, `IFFALSE a GOTO L9` o `IFTRUE a GOTO L9`).
- Evalúa primero `a` y, solo si `b != NULL` (y no está vacío), emite el segundo salto para `b` hacia la misma `etiqueta`.
- Nunca escribas más de `max` líneas ni excedas los 64 bytes por línea; devuelve el número total de líneas emitidas (`0` si `lineas == NULL`, `max <= 0`, `a == NULL` o `etiqueta == NULL`).

[OUTPUT]
Entrega únicamente el código C modular listo para pegar en `ia_sesion24_c.c` (sin `main()`) y un breve análisis de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion24.h"
#include <stdio.h>
#include <stddef.h>

static const char *mnemonico_cortocircuito(char op) {
    if (op == '|') {
        return "IFTRUE";
    }
    return "IFFALSE";
}

static int escribir_salto(char lineas[][64], int max, int *idx,
                        const char *mnemonico, const char *operando,
                        const char *etiqueta) {
    if (*idx >= max || operando == NULL || operando[0] == '\0') {
        return 0;
    }

    (void)snprintf(lineas[*idx], (size_t)64, "%s %s GOTO %s",
                   mnemonico, operando, etiqueta);
    (*idx)++;
    return 1;
}

int emitir_condicion(char lineas[][64], int max, const char *a, const char *b,
                     char op, const char *etiqueta) {
    if (lineas == NULL || max <= 0 || a == NULL || etiqueta == NULL) {
        return 0;
    }

    const char *mnemonico = mnemonico_cortocircuito(op);
    int emitidas = 0;

    (void)escribir_salto(lineas, max, &emitidas, mnemonico, a, etiqueta);
    (void)escribir_salto(lineas, max, &emitidas, mnemonico, b, etiqueta);

    return emitidas;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Condiciones compuestas con AND y OR

### Salida del harness
```text
=== AUDITORIA SESION 24 - Saltos condicionales con cortocircuito ===
Caso                               esperado:              obtenido:              resultado
[ 1] a && b (primer salto)
     esperado: IFFALSE a GOTO L9
     obtenido: IFFALSE a GOTO L9
     -> OK
[ 2] a && b (segundo salto)
     esperado: IFFALSE b GOTO L9
     obtenido: IFFALSE b GOTO L9
     -> OK
[ 3] a && b (dos saltos)                esperado: 2            obtenido: 2            OK
[ 4] a || b (primer salto)
     esperado: IFTRUE a GOTO L9
     obtenido: IFTRUE a GOTO L9
     -> OK
[ 5] a || b (segundo salto)
     esperado: IFTRUE b GOTO L9
     obtenido: IFTRUE b GOTO L9
     -> OK
[ 6] condicion simple (b NULL)
     esperado: IFFALSE a GOTO L0
     obtenido: IFFALSE a GOTO L0
     -> OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion24.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Una generación inicial suele omitir el caso en que b == NULL (pasándolo directamente a %s y provocando segfault o emitiendo un salto espurio) o no comprobar el límite emitidas < max antes de escribir el segundo salto. Se resolvió validando b != NULL && b[0] != '\0' y *idx < max dentro de la auxiliar escribir_salto, manteniendo complejidad temporal O(1) y espacial O(1).

---
**Firma digital:** [984-uriel]

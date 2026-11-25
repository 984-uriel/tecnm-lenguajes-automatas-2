# Bitácora de Requerimientos y Auditoría IA — Sesión 21
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Fundamentos de Código de 3 Direcciones  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Representación en Cuádruplos (op,a,b,r)  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir las funciones en C sin memoria dinámica ni variables globales, el principal cuidado fue acotar siempre la escritura al tamaño `cap` del buffer `dst` mediante `snprintf` y convertir `cap` a `size_t` de forma segura para evitar advertencias bajo `-Wall -Wextra -Werror`. También fue clave respetar el orden de actualización de `*contador` (leer primero para emitir `_tK` y después incrementar) y mapear cualquier argumento `NULL` a `"_"` en instrucciones como asignaciones simples (`(=, a, _, x)`) o saltos (`(goto, L1, _, _)`).

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Generación de temporales únicos (_t0..) |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[CONTEXTO]
Estoy cursando Lenguajes y Autómatas II (SCD-1016) en el track de C (estándar C11, compilado con gcc -std=c11 -Wall -Wextra -Werror). Debo implementar funciones en C declaradas en `api_sesion21.h` para generar nombres de temporales únicos y formatear cuádruplos de código de tres direcciones (TAC) escribiendo en un buffer de destino con capacidad limitada (`cap`), sin usar memoria dinámica (`malloc`/`free`), sin variables globales y sin incluir `main()`.

[ROL]
Actúa como ingeniero de software de sistemas y compiladores, enfocado en código C limpio, seguro contra desbordamientos de buffer y compatible con compilación estricta sin advertencias.

[ESPECIFICACIÓN]
Implementa exactamente estas dos funciones respetando las firmas de `api_sesion21.h`:
1. `void nuevo_temporal(char *dst, int cap, int *contador);`
   - Usa el valor actual de `*contador` para escribir `_t<valor>` en `dst` (por ejemplo, `_t0` si `*contador == 0`, `_t7` si `*contador == 7`) y luego incrementa `*contador` en 1.
2. `void emitir_cuadruplo(char *dst, int cap, const char *op, const char *a1, const char *a2, const char *res);`
   - Escribe en `dst` el formato `(<op>, <a1>, <a2>, <res>)`.
   - Si cualquiera de los punteros `op`, `a1`, `a2` o `res` es `NULL`, debe escribirse como `"_"` (guion bajo), sin omitirse ni inventarse valores.
- Valida de forma segura punteros nulos y capacidades `cap <= 0` para evitar comportamientos indefinidos o casts inválidos.

[OUTPUT]
Entrega únicamente el código C modular listo para pegar en `ia_sesion21_c.c` (sin `main()`) y un breve análisis de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion21.h"
#include <stdio.h>
#include <stddef.h>

static const char *campo_o_guion(const char *valor) {
    return (valor != NULL) ? valor : "_";
}

void nuevo_temporal(char *dst, int cap, int *contador) {
    if (dst == NULL || cap <= 0 || contador == NULL) {
        return;
    }

    int actual = *contador;
    (void)snprintf(dst, (size_t)cap, "_t%d", actual);
    *contador = actual + 1;
}

void emitir_cuadruplo(char *dst, int cap, const char *op, const char *a1, const char *a2, const char *res) {
    if (dst == NULL || cap <= 0) {
        return;
    }

    (void)snprintf(
        dst,
        (size_t)cap,
        "(%s, %s, %s, %s)",
        campo_o_guion(op),
        campo_o_guion(a1),
        campo_o_guion(a2),
        campo_o_guion(res)
    );
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Expresiones aritméticas compuestas

### Salida del harness
```text
=== AUDITORIA SESION 21 - Cuadruplos y temporales unicos ===
Caso                               esperado:              obtenido:              resultado
[ 1] nuevo_temporal(_t0)                esperado: _t0          obtenido: _t0          OK
[ 2] nuevo_temporal(_t7)                esperado: _t7          obtenido: _t7          OK
[ 3] cuadruplo de a + b
     esperado: (+, a, b, _t0)
     obtenido: (+, a, b, _t0)
     -> OK
[ 4] cuadruplo de _t0 * c
     esperado: (*, _t0, c, _t1)
     obtenido: (*, _t0, c, _t1)
     -> OK
[ 5] asignacion simple                  esperado: (=, a, _, x) obtenido: (=, a, _, x) OK
[ 6] goto sin operandos
     esperado: (goto, L1, _, _)
     obtenido: (goto, L1, _, _)
     -> OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion21.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En una primera revisión sin restricciones explícitas de guarda, una propuesta típica de IA asume que dst, contador y cap > 0 siempre son válidos, o convierte cap directamente a size_t sin comprobar antes cap <= 0 (lo cual en un int negativo o cero puede provocar comportamientos no deseados o advertencias según el nivel de aviso). Se resolvió agregando validaciones tempranas (dst == NULL || cap <= 0 || contador == NULL en nuevo_temporal y dst == NULL || cap <= 0 en emitir_cuadruplo) y centralizando la sustitución de NULL por "_" en la auxiliar campo_o_guion, verificando que ambas funciones operan en tiempo $O(1)$ y espacio auxiliar $O(1)$.

---
**Firma digital:** [984-uriel]

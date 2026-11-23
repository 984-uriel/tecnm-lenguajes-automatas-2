# Bitácora de Requerimientos y Auditoría IA — Sesión 17
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Comprobación Semántica de Retornos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Validación de tipo de sentencia return  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al implementar `diagnosticar_retorno()` a mano, el punto clave fue respetar el orden de precedencia entre validaciones de entrada (`-1`), compatibilidad de tipos (`1`) y completitud de rutas (`2`), además de modelar correctamente la jerarquía de anchos de tipo (`void`, `bool`/`char`, `short`, `int`/`float`, `long`/`double`). Si se evalúa primero si faltan rutas con `return` antes de validar el tipo de la expresión, un `return` con tipo incompatible en una sola ruta podría reportarse como `2` en lugar de `1`. También hay que cuidar que una función `void` no acepte expresiones no-`void`, que un tipo destino más estrecho (como `int` recibiendo `double`) devuelva `1`, y que todo compile limpio con `-std=c11 -Wall -Wextra -Werror`. 

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Rutas de ejecución con retorno garantizado |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Estoy programando en C11 con GCC usando las banderas -std=c11 -Wall -Wextra -Werror. La firma de `diagnosticar_retorno` está declarada en `api_sesion17.h` y este archivo se compilará junto con `harness_sesion17_c.c`, por lo que NO debe incluir `main()`.

Rol: Actúa como un ingeniero de software de sistemas y compiladores especializado en comprobación semántica de tipos y análisis de flujo de control.

Especificación: Implementa `int diagnosticar_retorno(const char *tipo_funcion, const char *tipo_expresion, int rutas_totales, int rutas_con_return)` con estas reglas:
1. Devuelve `-1` si `tipo_funcion == NULL`, `tipo_expresion == NULL`, `rutas_totales <= 0`, `rutas_con_return < 0`, `rutas_con_return > rutas_totales` o algún tipo es desconocido/vacío.
2. Evalúa el tipo antes que las rutas: devuelve `1` si el tipo es incompatible (por ejemplo, función `"void"` devolviendo algo distinto de `"void"`, función no-`"void"` devolviendo `"void"`, o cuando el tipo de expresión es más ancho que el tipo de la función, como `"double"` en `"int"`). Se acepta cuando el tipo destino es igual o más ancho (como `"double"` aceptando `"int"`).
3. Devuelve `2` si los tipos son válidos pero `rutas_con_return < rutas_totales` (queda al menos una ruta sin retorno).
4. Devuelve `0` si los tipos son compatibles y todas las rutas (`rutas_con_return == rutas_totales`) tienen retorno.

Output: Entrega únicamente el código modular en C listo para `ia_sesion17_c.c` (sin `main()`) y un resumen breve de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion17.h"
#include <stddef.h>
#include <string.h>

static int nivel_promocion(const char *t) {
    if (!t || *t == '\0') return -1;
    if (strcmp(t, "void") == 0) return 0;
    if (strcmp(t, "bool") == 0 || strcmp(t, "char") == 0) return 1;
    if (strcmp(t, "short") == 0) return 2;
    if (strcmp(t, "int") == 0) return 3;
    if (strcmp(t, "float") == 0) return 4;
    if (strcmp(t, "long") == 0 || strcmp(t, "double") == 0) return 5;
    return -1;
}

int diagnosticar_retorno(const char *tipo_funcion, const char *tipo_expresion, int rutas_totales, int rutas_con_return) {
    if (!tipo_funcion || !tipo_expresion) {
        return -1;
    }
    if (rutas_totales <= 0 || rutas_con_return < 0 || rutas_con_return > rutas_totales) {
        return -1;
    }

    int n_fn = nivel_promocion(tipo_funcion);
    int n_ex = nivel_promocion(tipo_expresion);
    if (n_fn < 0 || n_ex < 0) {
        return -1;
    }

    if (n_fn == 0) {
        if (n_ex != 0) {
            return 1;
        }
    } else if (n_ex == 0 || n_ex > n_fn) {
        return 1;
    }

    if (rutas_con_return < rutas_totales) {
        return 2;
    }

    return 0;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Funciones sin retorno o retornos mixtos

### Salida del harness
```text
=== AUDITORIA SESION 17 - Validacion de retornos y rutas ===
Caso                               esperado:              obtenido:              resultado
[ 1] int con int y dos rutas            esperado: 0            obtenido: 0            OK
[ 2] void devuelve valor                esperado: 1            obtenido: 1            OK
[ 3] ruta sin retorno                   esperado: 2            obtenido: 2            OK
[ 4] double acepta int                  esperado: 0            obtenido: 0            OK
[ 5] int no acepta double               esperado: 1            obtenido: 1            OK
[ 6] cero rutas de ejecucion            esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion17.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En implementaciones rápidas de validación de retornos se suele omitir la asimetría en la promoción de tipos (permitir "int" en "double" como válido, pero rechazar "double" en "int" como error de tipo 1), el caso de funciones "void" recibiendo un valor distinto de "void", o revisar rutas antes que tipos. Se definió explícitamente en el contrato C-R-E-O la jerarquía de niveles (nivel_promocion), el orden estricto de diagnóstico (-1 → 1 → 2 → 0) y la validación de rutas_totales <= 0 o rutas_con_return > rutas_totales.

---
**Firma digital:** [984-uriel]

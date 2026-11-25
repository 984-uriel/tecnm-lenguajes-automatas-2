# Bitácora de Requerimientos y Auditoría IA — Sesión 23
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Etiquetas y Saltos Incondicionales  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Generador de etiquetas e instrucciones goto  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir ambas funciones sin memoria dinámica ni variables globales, fue clave comprobar que `etiqueta != NULL` antes de formatear para devolver cadena vacía (`dst[0] = '\0'`) sin causar `SIGSEGV`, distinguir `es_salto != 0` (`"GOTO %s"`) de `es_salto == 0` (`"%s:"`) y acotar cada escritura a `cap` bytes mediante `snprintf` compilando sin advertencias bajo `-Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Trazabilidad del flujo de control |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[CONTEXTO]
Estoy cursando Lenguajes y Autómatas II (SCD-1016) en el track de C (estándar C11, compilado con gcc -std=c11 -Wall -Wextra -Werror). Debo implementar las funciones declaradas en `api_sesion23.h` para generar etiquetas de flujo de control y emitir saltos incondicionales o marcas de destino en un buffer `dst` de capacidad `cap`, sin `main()`, sin variables globales y sin memoria dinámica.

[ROL]
Actúa como ingeniero de software de sistemas y compiladores especializado en generación de código intermedio y control de flujo.

[ESPECIFICACIÓN]
Implementa exactamente estas dos funciones respetando las firmas de `api_sesion23.h`:
1. `void nueva_etiqueta(char *dst, int cap, int *contador);`
   - Usa el valor actual de `*contador` para escribir `L<valor>` en `dst` (empezando en `L0`, `L5`, `L10`, etc.) y después incrementa `*contador` en 1.
2. `void emitir_flujo(char *dst, int cap, const char *etiqueta, int es_salto);`
   - Si `etiqueta == NULL`, escribe cadena vacía (`""`) en `dst` sin provocar fallo de segmentación.
   - Si `es_salto != 0`, escribe `"GOTO <etiqueta>"`.
   - Si `es_salto == 0`, escribe `"<etiqueta>:"`.
- Respeta en todo momento `cap` (`(size_t)cap` en `snprintf`) y valida punteros nulos o `cap <= 0`.

[OUTPUT]
Entrega únicamente el código C modular listo para pegar en `ia_sesion23_c.c`
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion23.h"
#include <stdio.h>
#include <stddef.h>

void nueva_etiqueta(char *dst, int cap, int *contador) {
    if (dst == NULL || cap <= 0 || contador == NULL) {
        return;
    }

    int idx = *contador;
    (void)snprintf(dst, (size_t)cap, "L%d", idx);
    *contador = idx + 1;
}

void emitir_flujo(char *dst, int cap, const char *etiqueta, int es_salto) {
    if (dst == NULL || cap <= 0) {
        return;
    }

    if (etiqueta == NULL) {
        dst[0] = '\0';
        return;
    }

    if (es_salto != 0) {
        (void)snprintf(dst, (size_t)cap, "GOTO %s", etiqueta);
    } else {
        (void)snprintf(dst, (size_t)cap, "%s:", etiqueta);
    }
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Saltos adelante y atrás estructurados

### Salida del harness
```text
=== AUDITORIA SESION 23 - Etiquetas y saltos incondicionales ===
Caso                               esperado:              obtenido:              resultado
[ 1] nueva_etiqueta(L0)                 esperado: L0           obtenido: L0           OK
[ 2] nueva_etiqueta(L5)                 esperado: L5           obtenido: L5           OK
[ 3] emitir_flujo(GOTO L2)              esperado: GOTO L2      obtenido: GOTO L2      OK
[ 4] emitir_flujo(marca L2)             esperado: L2:          obtenido: L2:          OK
[ 5] emitir_flujo(NULL)                 esperado:              obtenido:              OK
[ 6] nueva_etiqueta(L10)                esperado: L10          obtenido: L10          OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion23.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Una implementación inicial sin guardas completas suele omitir el caso etiqueta == NULL (pasándolo directo a %s y provocando comportamiento indefinido o segfault) o no validar dst == NULL y cap <= 0 antes de convertir cap a size_t. Se resolvió agregando la comprobación if (etiqueta == NULL) { dst[0] = '\0'; return; } y las guardas de puntero/capacidad, conservando complejidad temporal O(1) y espacial O(1).

---
**Firma digital:** [984-uriel]

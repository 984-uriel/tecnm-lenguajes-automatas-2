# Bitácora de Requerimientos y Auditoría IA — Sesión 18
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Arreglos y Verificación de Límites  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Cálculo de tamaño de arreglo en memoria  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir `bytes_arreglo()` e `indice_en_rango()` a mano, lo más importante fue validar dimensiones negativas (`elementos < 0` debe devolver `-1`), permitir `elementos == 0` con tipo válido devolviendo `0`, mapear exactamente los tipos permitidos (`char`, `short`, `int`, `float`, `double` y `puntero`) y promover la multiplicación a `long` (`ancho * (long)elementos`) para reducir el riesgo de desbordamiento en `int`. En `indice_en_rango()`, el cuidado principal es comprobar `indice >= 0 && elementos > 0 && indice < elementos` de forma que el límite superior válido sea `elementos - 1` (nunca `elementos`) y no ocurran comparaciones ambiguas con tipos sin signo bajo `-std=c11 -Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Indexación entera no negativa |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Estoy programando en C11 con GCC usando las banderas -std=c11 -Wall -Wextra -Werror. Las firmas de `bytes_arreglo` e `indice_en_rango` están declaradas en `api_sesion18.h` y este archivo se compilará junto con `harness_sesion18_c.c`, por lo que NO debe incluir `main()`.

Rol: Actúa como un ingeniero de software de sistemas y compiladores especializado en disposición de memoria y verificación de límites.

Especificación: Implementa exactamente dos funciones:
1. `long bytes_arreglo(const char *tipo, int elementos)`:
   - Devuelve `-1` si `tipo == NULL`, `elementos < 0` o `tipo` no coincide exactamente con uno de estos nombres: `"char"`, `"short"`, `"int"`, `"float"`, `"double"`, `"puntero"`.
   - Si `elementos == 0` y el tipo es válido, devuelve `0`.
   - Si es válido, devuelve `sizeof(<tipo>) * (long)elementos` (usando `sizeof(void *)` para `"puntero"`).
2. `int indice_en_rango(int indice, int elementos)`:
   - Devuelve `1` si `elementos > 0`, `indice >= 0` y `indice < elementos`.
   - Devuelve `0` en cualquier otro caso (incluyendo `indice < 0`, `indice >= elementos` o `elementos <= 0`).

Output: Entrega únicamente el código modular en C listo para `ia_sesion18_c.c` (sin `main()`) y un resumen breve de complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion18.h"
#include <stddef.h>
#include <string.h>

long bytes_arreglo(const char *tipo, int elementos) {
    if (!tipo || elementos < 0) {
        return -1;
    }

    long tam_elem = -1;
    if (strcmp(tipo, "char") == 0) tam_elem = (long)sizeof(char);
    else if (strcmp(tipo, "short") == 0) tam_elem = (long)sizeof(short);
    else if (strcmp(tipo, "int") == 0) tam_elem = (long)sizeof(int);
    else if (strcmp(tipo, "float") == 0) tam_elem = (long)sizeof(float);
    else if (strcmp(tipo, "double") == 0) tam_elem = (long)sizeof(double);
    else if (strcmp(tipo, "puntero") == 0) tam_elem = (long)sizeof(void *);

    if (tam_elem < 0) {
        return -1;
    }

    return tam_elem * (long)elementos;
}

int indice_en_rango(int indice, int elementos) {
    return (indice >= 0 && elementos > 0 && indice < elementos) ? 1 : 0;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Límites estáticos y dimensiones inválidas

### Salida del harness
```text
=== AUDITORIA SESION 18 - Tamano de arreglo y limites ===
Caso                               esperado:              obtenido:              resultado
[ 1] int[10]                            esperado: 40           obtenido: 40           OK
[ 2] double[3]                          esperado: 24           obtenido: 24           OK
[ 3] int[-4]                            esperado: -1           obtenido: -1           OK
[ 4] indice 0 de 5                      esperado: 1            obtenido: 1            OK
[ 5] indice 5 de 5                      esperado: 0            obtenido: 0            OK
[ 6] indice negativo                    esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion18.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Al pedir funciones de cálculo de tamaño de arreglos y validación de índices en C, un modelo puede omitir dimensiones negativas (elementos < 0), el borde exacto indice == elementos (fuera de rango), o usar tipos sin signo (size_t) en la comparación de índices negativos que alteran la lógica de límites. Se exigió en el contrato C-R-E-O validar elementos < 0 antes de calcular bytes, promover a long la multiplicación y verificar explícitamente indice >= 0 && elementos > 0 && indice < elementos.

---
**Firma digital:** [984-uriel]

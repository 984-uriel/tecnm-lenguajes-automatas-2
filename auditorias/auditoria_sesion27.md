# Bitácora de Requerimientos y Auditoría IA — Sesión 27
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Indexación de Arreglos en TAC  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Cálculo de desplazamientos base + offset  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al escribir emitir_acceso() y emitir_acceso_2d() a mano en C, la restricción principal es la gestión segura del búfer estático lineas[][64] y el control de límites con max para evitar desbordamientos o lecturas/escrituras fuera de rango. En arreglos 2D es necesario respetar el orden exacto de aplanamiento i * filas + j, multiplicarlo por ancho y encadenar temporales únicos (*temporales) sin colisiones ni reuso accidental, además de validar punteros NULL y evitar advertencias bajo -std=c11 -Wall -Wextra -Werror.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Aplanado de direcciones unidimensionales |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto:
Estoy programando en C11 con GCC (compilado con -std=c11 -Wall -Wextra -Werror) para la materia de Lenguajes y Autómatas II. La cabecera api_sesion27.h ya define las firmas exactas y no debe modificarse. El archivo se compilará junto con harness_sesion27_c.c, por lo que no debe incluir main().

Rol:
Actúa como un ingeniero de software de sistemas y compiladores especializado en generación de código de tres direcciones (TAC).

Especificación:
Implementa en C las funciones:
1. int emitir_acceso(char lineas[][64], int max, const char *base, const char *indice, int ancho, int *temporales);
   - Si base == NULL o hay punteros inválidos / max <= 0 / *temporales < 0, retorna 0 sin emitir instrucciones.
   - Emite "_tK = <indice> * <ancho>" y luego "_t(K+1) = <base>[_tK]", incrementando *temporales en cada temporal nuevo y devolviendo el número de líneas emitidas.
2. int emitir_acceso_2d(char lineas[][64], int max, const char *base, const char *i, const char *j, int filas, int ancho, int *temporales);
   - Si base == NULL o hay punteros inválidos / max <= 0 / *temporales < 0, retorna 0 sin emitir instrucciones.
   - Emite en orden: "_t0 = <i> * <filas>", "_t1 = _t0 + <j>", "_t2 = _t1 * <ancho>" y "_t3 = <base>[_t2]" (numerando desde *temporales y avanzando *temporales), devolviendo el total de líneas emitidas.

Output:
Devuelve únicamente el código en C limpio y modular para ia_sesion27_c.c, incluyendo #include "api_sesion27.h" y <stdio.h>, sin función main() y sin advertencias del compilador.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion27.h"
#include <stdio.h>

int emitir_acceso(char lineas[][64], int max, const char *base,
                  const char *indice, int ancho, int *temporales) {
    int emitidas = 0;
    int t_mul;
    int t_load;

    if (!lineas || !temporales || !base || !indice || max <= 0 || *temporales < 0) {
        return 0;
    }

    t_mul = (*temporales)++;
    if (emitidas < max) {
        snprintf(lineas[emitidas++], 64, "_t%d = %s * %d", t_mul, indice, ancho);
    }

    t_load = (*temporales)++;
    if (emitidas < max) {
        snprintf(lineas[emitidas++], 64, "_t%d = %s[_t%d]", t_load, base, t_mul);
    }

    return emitidas;
}

int emitir_acceso_2d(char lineas[][64], int max, const char *base,
                     const char *i, const char *j, int filas, int ancho,
                     int *temporales) {
    int emitidas = 0;
    int t0;
    int t1;
    int t2;
    int t3;

    if (!lineas || !temporales || !base || !i || !j || max <= 0 || *temporales < 0) {
        return 0;
    }

    t0 = (*temporales)++;
    if (emitidas < max) {
        snprintf(lineas[emitidas++], 64, "_t%d = %s * %d", t0, i, filas);
    }

    t1 = (*temporales)++;
    if (emitidas < max) {
        snprintf(lineas[emitidas++], 64, "_t%d = _t%d + %s", t1, t0, j);
    }

    t2 = (*temporales)++;
    if (emitidas < max) {
        snprintf(lineas[emitidas++], 64, "_t%d = _t%d * %d", t2, t1, ancho);
    }

    t3 = (*temporales)++;
    if (emitidas < max) {
        snprintf(lineas[emitidas++], 64, "_t%d = %s[_t%d]", t3, base, t2);
    }

    return emitidas;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Arreglos multidimensionales en TAC

### Salida del harness
```text
=== AUDITORIA SESION 27 - Desplazamientos para arreglos 1D y 2D ===
Caso                               esperado:              obtenido:              resultado
[ 1] 1D: i * 4                          esperado: _t0 = i * 4  obtenido: _t0 = i * 4  OK
[ 2] 1D: A[_t0]                         esperado: _t1 = A[_t0] obtenido: _t1 = A[_t0] OK
[ 3] 2D: i * 3                          esperado: _t0 = i * 3  obtenido: _t0 = i * 3  OK
[ 4] 2D: offset por ancho
     esperado: _t2 = _t1 * 4
     obtenido: _t2 = _t1 * 4
     -> OK
[ 5] 2D: M[_t2]                         esperado: _t3 = M[_t2] obtenido: _t3 = M[_t2] OK
[ 6] base NULL (sin acceso)             esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion27.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Sin restricciones explícitas de diseño, un modelo tiende a omitir el caso borde de base == NULL (intentando emitir acceso sobre un puntero nulo o retornando un código de error distinto de 0), invertir el orden de la fórmula 2D o no limitar escrituras cuando emitidas alcanza max. Lo resolví exigiendo en el contrato C-R-E-O el retorno 0 cuando base es NULL, fijando la secuencia i * filas + j seguida de multiplicación por ancho y lectura indexada, y validando ambos ejecutables con el harness hasta completar 6/6 pruebas superadas.

---
**Firma digital:** [984-uriel]

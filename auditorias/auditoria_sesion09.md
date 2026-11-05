# Bitácora de Requerimientos y Auditoría IA — Sesión 09
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Reporte Formal de Errores Semánticos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Nombre Completo]  
**Matrícula:** [No. de Control]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Diagnóstico con línea y columna  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al codificar a mano, el mayor desafío fue garantizar el manejo seguro del tamaño del buffer (cap) utilizando snprintf para evitar desbordamientos de memoria (buffer overflows), asegurando al mismo tiempo que el truncamiento en cadenas ajustadas (como el caso de cap=10) manejara correctamente los caracteres permitidos sin corromper la memoria subsiguiente ni omitir el byte nulo de terminación.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Estructura estándar de errores GCC |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[C] Contexto: Desarrollo en C99 / GCC utilizando el contrato de la cabecera "api_sesion09.h" para la materia de Lenguajes y Autómatas II (SCD-1016).
[R] Rol: Ingeniero de software especialista en sistemas y diseño de compiladores.
[E] Especificación: Implementa la función:
    void reportar(const char *archivo, int linea, int columna, const char *mensaje, char *salida, int cap);
    Debe armar la cadena exacta con el formato de diagnóstico estilo GCC: "archivo:linea:columna: error: mensaje". Debe utilizar de forma segura la capacidad `cap` del buffer recibido para prevenir desbordamientos de memoria y asegurar que siempre termine en '\0'.
[O] Output: Código en C puro y modular para ia_sesion09_c.c, sin función main(), cumpliendo estrictamente con las banderas de compilación -Wall -Wextra -Werror.
```

---

## 3. Código Devuelto por el Modelo
```c

#include "api_sesion09.h"
#include <stdio.h>
#include <string.h>

void reportar(const char *archivo, int linea, int columna, const char *mensaje, char *salida, int cap) {
    if (salida == NULL || cap <= 0) {
        return;
    }

    int escrito = snprintf(salida, (size_t)cap, "%s:%d:%d: error: %s", archivo, linea, columna, mensaje);
    
    if (escrito < 0) {
        salida[0] = '\0';
    } else if (escrito >= cap) {
        salida[cap - 1] = '\0';
    }
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Múltiples errores sin abortar parser

### Salida del harness
```text
=== AUDITORIA SESION 09 - Diagnostico con linea y columna ===
Caso                               esperado:              obtenido:              resultado
[ 1] main.c:14:5
     esperado: main.c:14:5: error: tipo incompatible
     obtenido: main.c:14:5: error: tipo incompatible
     -> OK
[ 2] a.c:1:1
     esperado: a.c:1:1: error: x
     obtenido: a.c:1:1: error: x
     -> OK
[ 3] linea 0
     esperado: x.c:0:0: error: y
     obtenido: x.c:0:0: error: y
     -> OK
[ 4] cap=10 trunca                     esperado: main.c:14    obtenido: main.c:14    OK

RESULTADO: 4/4 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion09.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> La IA inicialmente pudo omitir la validación de punteros nulos o capacidades negativas (salida == NULL || cap <= 0), lo cual expondría a la función ante fallos de segmentación si se invocara con parámetros vacíos. Se solucionó añadiendo guardas explícitas al inicio y manejando de forma segura los límites del buffer devueltos por snprintf.

---
**Firma digital:** [984-uriel]

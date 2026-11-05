# Bitácora de Requerimientos y Auditoría IA — Sesión 08
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Inferencia y Promoción de Tipos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Reglas de casteo implícito  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al codificar a mano, la principal restricción fue manejar correctamente la jerarquía de los tipos (`CHAR < INT < FLOAT`) aprovechando los valores numéricos del `enum` definido en `api_sesion08.h`. Fue necesario asegurar que la presencia del centinela `TIPO_ERROR` interrumpiera la promoción devolviendo inmediatamente `TIPO_ERROR`, además de mapear adecuadamente las cadenas de texto en `nombre_tipo` sin provocar fallos de segmentación o retornar valores nulos.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Promoción entera y ensanchamiento |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[C] Contexto: Desarrollo en C99 / GCC utilizando el contrato de la cabecera "api_sesion08.h" para la materia de Lenguajes y Autómatas II (SCD-1016).
[R] Rol: Ingeniero de software especialista en sistemas y diseño de compiladores.
[E] Especificación: Implementa las funciones:
    1. Tipo promover(Tipo a, Tipo b): Devuelve el tipo más ancho según la jerarquía CHAR < INT < FLOAT. Si alguno es TIPO_ERROR, retorna TIPO_ERROR.
    2. int hay_perdida(Tipo destino, Tipo origen): Retorna 1 si el tipo destino es más angosto que el origen, y 0 en caso contrario.
    3. const char *nombre_tipo(Tipo t): Retorna la cadena legible ("char", "int", "float", "error") asegurando protección contra desbordamientos.
[O] Output: Código en C puro y modular para ia_sesion08_c.c, sin función main(), cumpliendo de forma estricta con las banderas de compilación -Wall -Wextra -Werror.
```

---

## 3. Código Devuelto por el Modelo
```c

#include "api_sesion08.h"

Tipo promover(Tipo a, Tipo b) {
    if (a == TIPO_ERROR || b == TIPO_ERROR) {
        return TIPO_ERROR;
    }
    return (a >= b) ? a : b;
}

int hay_perdida(Tipo destino, Tipo origen) {
    return destino < origen;
}

const char *nombre_tipo(Tipo t) {
    static const char *nombres[] = {
        [TIPO_ERROR] = "error",
        [TIPO_CHAR]  = "char",
        [TIPO_INT]   = "int",
        [TIPO_FLOAT] = "float"
    };

    if (t >= TIPO_ERROR && t <= TIPO_FLOAT) {
        return nombres[t];
    }
    return "error";
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Mezcla de tipos enteros y flotantes

### Salida del harness
```text
=== AUDITORIA SESION 08 - Reglas de promocion y perdida ===
Caso                               esperado:              obtenido:              resultado
[ 1] promover(CHAR,INT)                 esperado: int          obtenido: int          OK
[ 2] promover(INT,FLOAT)                esperado: float        obtenido: float        OK
[ 3] promover(FLOAT,FLOAT)              esperado: float        obtenido: float        OK
[ 4] promover(ERROR,INT)                esperado: error        obtenido: error        OK
[ 5] hay_perdida(CHAR,INT)              esperado: 1            obtenido: 1            OK
[ 6] hay_perdida(INT,CHAR)              esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE.
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> La IA utilizó inicializadores designados para arreglos estáticos en nombre_tipo ([TIPO_CHAR] = "char"). Aunque es válido en C99, si se llega a recibir un valor numérico fuera del rango del enum, podría causar un acceso a memoria inválido. Se solucionó añadiendo una validación explícita if (t >= TIPO_ERROR && t <= TIPO_FLOAT) para asegurar que cualquier tipo fuera del rango devuelva con seguridad la cadena "error".

---
**Firma digital:** [984-uriel]

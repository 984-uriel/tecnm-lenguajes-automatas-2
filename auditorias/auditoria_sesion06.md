# Bitácora de Requerimientos y Auditoría IA — Sesión 06
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Sistema de Tipos Primitivos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Chequeo de compatibilidad de tipos  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
 > Al codificar a mano, fue necesario estructurar correctamente la jerarquía estricta de los tipos primitivos (`CHAR < INT < FLOAT`) mediante operadores de comparación relacional, asegurando que `TIPO_ERROR` actuara como una condición de bloqueo absoluta que impidiera cualquier asignación inválida.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Matriz de compatibilidad estática |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[Pega aquí el prompt formal que redactaste para la IA]
```

---

## 3. Código Devuelto por el Modelo
```c
/* ia_sesion06_c.c - Fase 2: El Cientifico */
#include "api_sesion06.h"

int compatible(Tipo destino, Tipo origen) {
    static const int tabla_compatibilidad[4][4] = {
        /*              ERROR  CHAR  INT  FLOAT */
        /* TIPO_ERROR */ {  0,    0,    0,    0  },
        /* TIPO_CHAR  */ {  0,    1,    0,    0  },
        /* TIPO_INT   */ {  0,    1,    1,    0  },
        /* TIPO_FLOAT */ {  0,    1,    1,    1  }
    };

    if (destino < 0 || destino > 3 || origen < 0 || origen > 3) {
        return 0;
    }
    return tabla_compatibilidad[destino][origen];
}

const char *nombre_tipo(Tipo t) {
    static const char *nombres[] = {
        "error",
        "char",
        "int",
        "float"
    };

    if (t < 0 || t > 3) {
        return "desconocido";
    }
    return nombres[t];
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Rechazo de asignaciones incompatibles

### Salida del harness
```text
=== AUDITORIA SESION 06 - Tabla de compatibilidad de tipos ===
Caso                                esperado:              obtenido:              resultado
[ 1] compatible(INT,INT)            esperado: 1            obtenido: 1            OK
[ 2] compatible(FLOAT,INT)          esperado: 1            obtenido: 1            OK
[ 3] compatible(INT,FLOAT)          esperado: 0            obtenido: 0            OK
[ 4] compatible(INT,ERROR)          esperado: 0            obtenido: 0            OK
[ 5] nombre_tipo(INT)               esperado: int          obtenido: int          OK
[ 6] nombre_tipo(ERROR)             esperado: error        obtenido: error        OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE.
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> La IA inicialmente omitió validar los límites estrictos de los arreglos estáticos y de la enumeración (OutOfBounds), lo cual podría haber provocado un comportamiento indefinido si se le pasaban valores fuera del rango esperado. Se solucionó añadiendo guardias de validación de límites (if (destino < 0 || destino > 3 || ...)) para asegurar la robustez defensiva del código ante entradas anómalas.

---
**Firma digital:** [984-uriel]

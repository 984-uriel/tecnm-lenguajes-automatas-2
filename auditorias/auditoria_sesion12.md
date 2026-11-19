# Bitácora de Requerimientos y Auditoría IA — Sesión 12
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Atributos de Símbolos y Memoria  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No. 24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Estructura de registro de símbolos  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al trabajar con `struct Simbolo` y alineación natural en C11, se debe distinguir entre el offset alineado donde comienza el símbolo (`simbolo->offset`) y el cursor que se retorna para el siguiente símbolo (`offset_alineado + ancho`), garantizando que el cursor nunca retroceda. También hay que validar `simbolo != NULL`, `simbolo->tipo != NULL`, `cursor >= 0` y cadenas de tipo desconocidas antes de operar, evitando desreferencias nulas o cálculos inválidos bajo `-Wall -Wextra -Werror`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Cálculo de desplazamientos y alineación |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[CONTEXTO]
Estoy desarrollando la Sesión 12 (Atributos de Símbolos y Memoria) de Lenguajes y Autómatas II (SCD-1016) en C11 con GCC usando -std=c11 -Wall -Wextra -Werror y un test harness externo que ya define main(). El archivo incluye "api_sesion12.h", con typedef struct Simbolo { const char *nombre; const char *tipo; int offset; } Simbolo; y las firmas int ancho_de_tipo(const char *tipo) e int asignar_offset(Simbolo *simbolo, int cursor).

[ROL]
Actúa como ingeniero de software de sistemas y compiladores experto en layout de memoria, alineación natural y C11.

[ESPECIFICACIÓN]
1. Implementa ancho_de_tipo(const char *tipo): devuelve 1 para "char", 2 para "short", 4 para "int", 4 para "float", 8 para "double", 8 para "puntero"; si tipo es NULL o cualquier otro valor, devuelve -1.
2. Implementa asignar_offset(Simbolo *simbolo, int cursor):
   - Si simbolo == NULL o cursor < 0, devuelve -1.
   - Calcula ancho = ancho_de_tipo(simbolo->tipo); si es inválido (<= 0), devuelve -1.
   - Redondea cursor hacia arriba al siguiente múltiplo de ancho (sin retroceder), guarda ese valor en simbolo->offset y devuelve offset_alineado + ancho.
3. No incluyas main() ni generes warnings bajo -Wall -Wextra -Werror.

[OUTPUT]
Entrega únicamente el código en C11 listo para pegarse en ia_sesion12_c.c, sin función main() ni texto adicional dentro del bloque.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion12.h"
#include <string.h>

static int redondear_arriba(int valor, int alineacion) {
    int bloques = (valor + alineacion - 1) / alineacion;
    return bloques * alineacion;
}

int ancho_de_tipo(const char *tipo) {
    if (tipo == NULL) {
        return -1;
    }

    const char *nombres[] = {
        "char", "short", "int", "float", "double", "puntero"
    };
    const int anchos[] = {
        1, 2, 4, 4, 8, 8
    };
    size_t total = sizeof(nombres) / sizeof(nombres[0]);

    for (size_t i = 0; i < total; i++) {
        if (strcmp(tipo, nombres[i]) == 0) {
            return anchos[i];
        }
    }
    return -1;
}

int asignar_offset(Simbolo *simbolo, int cursor) {
    if (simbolo == NULL || cursor < 0) {
        return -1;
    }

    int ancho = ancho_de_tipo(simbolo->tipo);
    if (ancho <= 0) {
        return -1;
    }

    int alineado = redondear_arriba(cursor, ancho);
    simbolo->offset = alineado;
    return alineado + ancho;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Alineación de tipos y tamaños en bytes

### Salida del harness
```text
=== AUDITORIA SESION 12 - Registro de simbolo y offsets alineados ===
Caso                               esperado:              obtenido:              resultado
[ 1] ancho_de_tipo("int")               esperado: 4            obtenido: 4            OK
[ 2] ancho_de_tipo("double")            esperado: 8            obtenido: 8            OK
[ 3] int en cursor 0                    esperado: 4            obtenido: 4            OK
[ 4] char e int: offset de b            esperado: 4            obtenido: 4            OK
[ 5] double desde cursor 4              esperado: 8            obtenido: 8            OK
[ 6] registro NULL                      esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion12.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Una implementación ingenua suele omitir simbolo == NULL o tipo == NULL (lo que provoca crash al leer simbolo->tipo o pasar NULL a strcmp), cursors negativos o tipos no soportados (<= 0), y devolver el offset alineado en lugar del siguiente cursor (alineado + ancho). Se añadieron comprobaciones explícitas de punteros y rangos antes de calcular la alineación y se validó con el harness bajo -Wall -Wextra -Werror hasta obtener 6/6 pruebas superadas.

---
**Firma digital:** [984-uriel]

# Bitácora de Requerimientos y Auditoría IA — Sesión 11
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Arquitectura de la Tabla de Símbolos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Tabla Hash con encadenamiento  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al implementar la tabla hash manualmente en C11, se debe validar primero que `tabla != NULL`, `nombre != NULL` y `cubetas > 0` para evitar desreferenciar punteros nulos o provocar una división entre cero al calcular `hash % cubetas`. También hay que asegurar que `nombre` se copie con límite en `char nombre[16]` garantizando el terminador nulo `'\0'`, buscar duplicados en todas las cubetas antes de reservar un nuevo `Simbolo` con `malloc` y encadenar colisiones sin perder referencias previas.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Función de dispersión y colisiones O(1) |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
[CONTEXTO]
Estoy desarrollando la Sesión 11 (Arquitectura de la Tabla de Símbolos) de Lenguajes y Autómatas II (SCD-1016) en C11 con GCC usando las flags -std=c11 -Wall -Wextra -Werror, junto con un test harness externo que ya define main(). El archivo incluye "api_sesion11.h", donde se declaran struct Simbolo (con char nombre[16], int tipo, int offset, struct Simbolo *sig) y las firmas exactas unsigned int hash_djb2(const char *clave) e int insertar_simbolo(Simbolo *tabla[], unsigned int cubetas, const char *nombre, int tipo, int offset).

[ROL]
Actúa como ingeniero de software de sistemas y compiladores experto en C11, gestión dinámica de memoria y estructuras de datos para tablas de símbolos.

[ESPECIFICACIÓN]
1. Implementa hash_djb2(const char *clave) iniciando en 5381 y acumulando hash * 33 + byte (casteando a unsigned char) por cada carácter hasta '\0'.
2. Implementa insertar_simbolo(Simbolo *tabla[], unsigned int cubetas, const char *nombre, int tipo, int offset):
   - Si tabla == NULL, nombre == NULL o cubetas == 0, devuelve -1 sin desreferenciar punteros ni dividir entre cero.
   - Si ya existe un símbolo con el mismo nombre en cualquier cubeta de la tabla, devuelve 0 y no crea nodos nuevos.
   - Si no existe, reserva memoria en el heap para Simbolo, copia nombre de forma segura dentro de char nombre[16] asegurando terminación nula '\0', asigna tipo y offset, y encadena el nodo en la cubeta hash_djb2(nombre) % cubetas sin sobrescribir entradas previas, devolviendo 1.
3. No incluyas función main() ni dejes etiquetas o variables sin usar que provoquen warnings bajo -Wall -Wextra -Werror.

[OUTPUT]
Entrega únicamente el código fuente en C11 listo para pegarse en ia_sesion11_c.c, sin función main() y sin texto introductorio o de relleno dentro del bloque de código.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion11.h"
#include <stdlib.h>
#include <string.h>

static int existe_en_tabla(Simbolo *tabla[], unsigned int cubetas, const char *nombre) {
    for (unsigned int b = 0u; b < cubetas; b++) {
        Simbolo *cursor = tabla[b];
        while (cursor != NULL) {
            if (strcmp(cursor->nombre, nombre) == 0) {
                return 1;
            }
            cursor = cursor->sig;
        }
    }
    return 0;
}

unsigned int hash_djb2(const char *clave) {
    unsigned int h = 5381u;
    if (clave == NULL) {
        return h;
    }

    for (size_t i = 0; clave[i] != '\0'; i++) {
        unsigned char byte = (unsigned char)clave[i];
        h = ((h << 5) + h) + (unsigned int)byte;
    }
    return h;
}

int insertar_simbolo(Simbolo *tabla[], unsigned int cubetas, const char *nombre, int tipo, int offset) {
    if (tabla == NULL || nombre == NULL || cubetas == 0u) {
        return -1;
    }

    if (existe_en_tabla(tabla, cubetas, nombre)) {
        return 0;
    }

    Simbolo *nuevo = (Simbolo *)malloc(sizeof(Simbolo));
    if (nuevo == NULL) {
        return -1;
    }

    for (size_t i = 0; i < sizeof(nuevo->nombre) - 1u; i++) {
        if (nombre[i] == '\0') {
            nuevo->nombre[i] = '\0';
            goto copia_lista;
        }
        nuevo->nombre[i] = nombre[i];
    }
    nuevo->nombre[sizeof(nuevo->nombre) - 1u] = '\0';

copia_lista:
    nuevo->tipo = tipo;
    nuevo->offset = offset;
    nuevo->sig = NULL;

    unsigned int pos = hash_djb2(nombre) % cubetas;
    if (tabla[pos] == NULL) {
        tabla[pos] = nuevo;
    } else {
        Simbolo *tail = tabla[pos];
        while (tail->sig != NULL) {
            tail = tail->sig;
        }
        tail->sig = nuevo;
    }

    return 1;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Manejo de claves duplicadas y colisiones

### Salida del harness
```text
=== AUDITORIA SESION 11 - Tabla hash con encadenamiento ===
Caso                               esperado:              obtenido:              resultado
[ 1] hash_djb2("")                      esperado: 5381         obtenido: 5381         OK
[ 2] hash_djb2("abc")                   esperado: 193485963    obtenido: 193485963    OK
[ 3] insertar x dos veces               esperado: 0            obtenido: 0            OK
[ 4] dos claves, una cubeta             esperado: 2            obtenido: 2            OK
[ 5] offset de beta en cadena           esperado: 4            obtenido: 4            OK
[ 6] insertar en tabla NULL             esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. 
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Al compilar ia_sesion11_c.c con -Wall -Wextra -Werror, GCC marcó error (error: label 'snprintf_like_copy' defined but not used [-Werror=unused-label]) porque quedó una etiqueta sin utilizar dentro de insertar_simbolo. Además, al diseñar la inserción se cuidó verificar claves repetidas en toda la tabla (no solo en una cubeta), validar cubetas == 0 y NULL, y acotar la copia a sizeof(nuevo->nombre) - 1 con terminación '\0'. Se eliminó la etiqueta residual, se guardó de nuevo ia_sesion11_c.c y se recompiló hasta obtener 6/6 pruebas superadas sin advertencias ni errores.

---
**Firma digital:** [984-uriel]

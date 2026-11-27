# Bitácora de Requerimientos y Auditoría IA — Sesión 26
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** TAC para Llamadas a Funciones  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Instrucciones param, call y return  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al implementar emitir_llamada() y emitir_return() a mano en C, la principal restricción es el control estricto de los límites del arreglo bidimensional lineas[][64] y del buffer dst de tamaño cap, evitando desbordamientos de búfer y accesos fuera de rango mediante *n < max. También fue clave contabilizar únicamente los argumentos no NULL (a1 y a2) para que el número emitido en CALL coincida con los PARAM reales, y asegurar que *n avance de forma acumulativa para no sobrescribir temporales como _t0 en llamadas anidadas tipo f(g(x)). Además, se debe validar punteros NULL y compilar sin advertencias bajo -std=c11 -Wall -Wextra -Werror.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Convención de paso de argumentos TAC |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto:
Estoy desarrollando en C11 con GCC (compilado con -std=c11 -Wall -Wextra -Werror) para la materia de Lenguajes y Autómatas II. El archivo de cabecera api_sesion26.h ya define las firmas exactas y no debe modificarse. El archivo de implementación se compilará junto con harness_sesion26_c.c, por lo que no debe incluir main().

Rol:
Actúa como un ingeniero de software de sistemas y compiladores especializado en generación de código de tres direcciones (TAC).

Especificación:
Implementa en C las funciones:
1. int emitir_llamada(char lineas[][64], int max, int *n, const char *funcion, const char *a1, const char *a2, const char *resultado);
   - Verifica punteros válidos y límites de capacidad antes de escribir.
   - Emite "PARAM <arg>" en orden para a1 y luego a2 solo si no son NULL, incrementando *n y el contador real de parámetros emitidos.
   - Emite "<resultado> = CALL <funcion>, <k>" si resultado no es NULL, o "CALL <funcion>, <k>" si resultado es NULL, donde <k> es la cantidad real de PARAM emitidos.
   - Avanza *n en cada línea agregada para conservar temporales en llamadas anidadas (por ejemplo f(g(x))).
2. void emitir_return(char *dst, int cap, const char *valor);
   - Si valor no es NULL (ni vacío), escribe "RETURN <valor>" en dst respetando cap con snprintf.
   - Si valor es NULL, escribe "RETURN".

Output:
Devuelve únicamente el código en C limpio, modular y listo para ia_sesion26_c.c (incluyendo #include "api_sesion26.h" y cabeceras estándar necesarias como <stdio.h>), sin función main() y sin advertencias del compilador.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion26.h"
#include <stdio.h>

int emitir_llamada(char lineas[][64], int max, int *n, const char *funcion,
                   const char *a1, const char *a2, const char *resultado) {
    const char *args[2];
    int num_params = 0;

    if (!lineas || !n || !funcion || max <= 0 || *n < 0) {
        return -1;
    }

    args[0] = a1;
    args[1] = a2;

    for (int i = 0; i < 2; i++) {
        if (args[i] != NULL && *n < max) {
            snprintf(lineas[*n], 64, "PARAM %s", args[i]);
            (*n)++;
            num_params++;
        }
    }

    if (*n < max) {
        if (resultado != NULL && resultado[0] != '\0') {
            snprintf(lineas[*n], 64, "%s = CALL %s, %d", resultado, funcion, num_params);
        } else {
            snprintf(lineas[*n], 64, "CALL %s, %d", funcion, num_params);
        }
        (*n)++;
    }

    return num_params;
}

void emitir_return(char *dst, int cap, const char *valor) {
    if (!dst || cap <= 0) {
        return;
    }

    if (valor != NULL && valor[0] != '\0') {
        snprintf(dst, (size_t)cap, "RETURN %s", valor);
    } else {
        snprintf(dst, (size_t)cap, "RETURN");
    }
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Llamadas anidadas f(g(x))

### Salida del harness
```text
=== AUDITORIA SESION 26 - Llamadas, parametros y retorno ===
Caso                               esperado:              obtenido:              resultado
[ 1] PARAM x                            esperado: PARAM x      obtenido: PARAM x      OK
[ 2] PARAM y                            esperado: PARAM y      obtenido: PARAM y      OK
[ 3] CALL con dos argumentos
     esperado: _t0 = CALL f, 2
     obtenido: _t0 = CALL f, 2
     -> OK
[ 4] llamada anidada f(g(x))
     esperado: _t1 = CALL f, 1
     obtenido: _t1 = CALL f, 1
     -> OK
[ 5] CALL sin argumentos
     esperado: _t0 = CALL p, 0
     obtenido: _t0 = CALL p, 0
     -> OK
[ 6] RETURN con valor                   esperado: RETURN _t0   obtenido: RETURN _t0   OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion26.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> En una primera aproximación al pedir emisión de TAC sin restricción explícita de orden y capacidad, un modelo puede invertir el orden de argumentos, contar parámetros nulos dentro del número de CALL o no acotar la escritura cuando el arreglo de líneas alcanza max. Lo resolví especificando en el contrato C-R-E-O el orden estricto (a1 luego a2), el conteo real de argumentos no NULL, la validación de punteros y límites con *n < max y snprintf, y verificando ambos binarios (test_artesano y test_ia) con el harness hasta obtener 6/6 pruebas superadas.

---
**Firma digital:** [984-uriel]

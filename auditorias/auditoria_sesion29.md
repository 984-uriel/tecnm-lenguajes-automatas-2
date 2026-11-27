# Bitácora de Requerimientos y Auditoría IA — Sesión 29
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Optimización: Dead Code Elimination  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No.24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Detección de código inalcanzable  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Al codificar eliminar_muerto() a mano en C, la restricción principal es compactar en el mismo arreglo lineas[][64] sin salir de los límites [0, n), conservando GOTO y RETURN, descartando lo que sigue hasta encontrar una etiqueta terminada en ':' y manejando listas vacías (n <= 0) sin leer fuera de rango. Además, al compilar con -std=c11 -Wall -Wextra -Werror, usar snprintf() leyendo y escribiendo sobre el mismo arreglo lineas dispara una advertencia por posible solapamiento de argumento restrict, tratada como error por -Werror.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC o Rust 2021 sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Análisis de flujo tras retornos y gotos |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto:
Estoy programando en C11 con GCC (compilado con -std=c11 -Wall -Wextra -Werror) para la materia de Lenguajes y Autómatas II. La cabecera api_sesion29.h ya define la firma int eliminar_muerto(char lineas[][64], int n) y no debe modificarse. El archivo se compilará junto con harness_sesion29_c.c, por lo que no debe incluir main().

Rol:
Actúa como un ingeniero de software de sistemas y compiladores especializado en optimización de código intermedio (TAC) y eliminación de código inalcanzable.

Especificación:
Implementa int eliminar_muerto(char lineas[][64], int n) en C siguiendo estas reglas:
1. Si lineas es NULL o n <= 0, retorna 0 sin leer memoria fuera del arreglo.
2. Conserva la instrucción actual si no está en zona muerta o si es una etiqueta (termina en ':'), reactivando el flujo alcanzable al encontrar una etiqueta.
3. Conserva las instrucciones GOTO y RETURN, marcando como inalcanzables las siguientes instrucciones hasta encontrar la próxima etiqueta.
4. Compacta en el mismo arreglo conservando el orden original y devuelve la cantidad de instrucciones resultantes.
5. Evita usar snprintf() sobre lineas[destino] leyendo desde lineas[origen] para no provocar advertencias de solapamiento restrict con -Werror.

Output:
Devuelve únicamente el código en C limpio y modular para ia_sesion29_c.c, incluyendo #include "api_sesion29.h" y las cabeceras estándar necesarias (<stddef.h>, <string.h>), sin función main() y sin advertencias del compilador.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion29.h"
#include <stddef.h>
#include <string.h>

static int linea_es_etiqueta(const char *linea) {
    size_t n;
    if (!linea) {
        return 0;
    }
    n = strlen(linea);
    return (n > 0 && linea[n - 1] == ':');
}

static int linea_es_salto_o_retorno(const char *linea) {
    if (!linea) {
        return 0;
    }
    if (strcmp(linea, "RETURN") == 0 || strncmp(linea, "RETURN ", 7) == 0) {
        return 1;
    }
    if (strncmp(linea, "GOTO ", 5) == 0 || strcmp(linea, "GOTO") == 0) {
        return 1;
    }
    return 0;
}

static void mover_cadena_64(char destino[64], const char *origen) {
    size_t pos = 0;
    if (!origen) {
        destino[0] = '\0';
        return;
    }
    while (pos < 63 && origen[pos] != '\0') {
        destino[pos] = origen[pos];
        pos++;
    }
    destino[pos] = '\0';
}

int eliminar_muerto(char lineas[][64], int n) {
    int idx = 0;
    int descartando = 0;

    if (!lineas || n <= 0) {
        return 0;
    }

    for (int i = 0; i < n; i++) {
        if (linea_es_etiqueta(lineas[i])) {
            descartando = 0;
        }

        if (!descartando) {
            if (idx != i) {
                mover_cadena_64(lineas[idx], lineas[i]);
            }
            idx++;

            if (linea_es_salto_o_retorno(lineas[i])) {
                descartando = 1;
            }
        }
    }

    return idx;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Eliminación de bloques inalcanzables

### Salida del harness
```text
=== AUDITORIA SESION 29 - Eliminacion de codigo inalcanzable ===
Caso                               esperado:              obtenido:              resultado
[ 1] tras GOTO: nuevo tamano            esperado: 4            obtenido: 4            OK
[ 2] tras GOTO: etiqueta viva           esperado: L1:          obtenido: L1:          OK
[ 3] tras RETURN: nuevo tamano          esperado: 4            obtenido: 4            OK
[ 4] tras RETURN: etiqueta viva         esperado: L5:          obtenido: L5:          OK
[ 5] sin codigo muerto                  esperado: 3            obtenido: 3            OK
[ 6] lista vacia                        esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion29.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Al compactar el arreglo en sitio con snprintf(lineas[idx], 64, "%s", lineas[i]), GCC marcó error bajo -Wall -Wextra -Werror por posible solapamiento de argumento restrict sobre el mismo arreglo lineas, además de que en casos límite debe cuidarse que una etiqueta reactive el código alcanzable y que n <= 0 retorne 0 sin leer fuera del arreglo. Lo solucioné reemplazando la copia por un helper carácter por carácter acotado a 63 caracteres + '\0', especificando el caso en el contrato C-R-E-O y verificando ambos ejecutables con el harness hasta obtener 6/6 pruebas superadas.

---
**Firma digital:** [984-uriel]

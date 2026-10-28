# Bitácora de Requerimientos y Auditoría IA — Sesión 03
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Google Colab, Mágicos y Flujo de Entrega  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** [Jonathan Uriel Che Castillo]  
**Matrícula:** [No. 24531349]  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---
# 1. Fase 1: El Artesano (Dificultad Manual)

**Reto:** Manipulación de archivos y llamadas OS

*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

Durante la implementación manual del programa fue necesario respetar exactamente las firmas declaradas en `api_sesion03.h`, ya que cualquier modificación provocaba errores de compilación con el harness. También fue importante manejar correctamente la apertura y cierre de archivos mediante `fopen()` y `fclose()`, validando errores en cada operación.

Otro aspecto relevante fue el conteo de líneas en archivos que no terminan con el carácter de salto de línea (`\n`), ya que una implementación que únicamente cuenta saltos de línea puede producir resultados incorrectos. Además, para la función `ejecutar()` fue necesario obtener el código de salida real del proceso utilizando `WIFEXITED()` y `WEXITSTATUS()`.

---
# 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

## 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
|------------------|-------------------|
| **Contexto (C)** | C11 / GCC sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software especializado en sistemas operativos y compiladores |
| **Especificación (E)** | Manipulación de archivos, conteo de líneas y obtención de códigos de retorno POSIX |
| **Output (O)** | Código modular en C compatible con el harness de pruebas |

## 2.2 Prompt Estructurado

**Contexto:** Estoy desarrollando una práctica en C que utiliza el archivo `api_sesion03.h` y un harness de pruebas que compila con GCC usando `-std=c11 -Wall -Wextra -Werror`.

**Rol:** Actúa como un ingeniero de software especializado en lenguaje C, sistemas operativos y programación de bajo nivel.

**Especificación:**

- Implementa `int escribir_texto(const char *ruta, const char *texto);`
- Debe escribir el contenido recibido en un archivo y devolver 0 en éxito o -1 en error.
- Implementa `int contar_lineas(const char *ruta);`
- Debe contar correctamente las líneas de un archivo.
- Debe devolver -1 si el archivo no existe o no puede abrirse.
- Debe considerar correctamente archivos que no terminan con `\n`.
- Implementa `int ejecutar(const char *comando);`
- Debe ejecutar comandos usando `system()`.
- Debe devolver el código de salida real mediante `WEXITSTATUS()`.
- Debe utilizar `WIFEXITED()` para validar la terminación normal del proceso.
- No incluir la función `main()`.
- Respetar exactamente las firmas declaradas en `api_sesion03.h`.
- El código debe compilar sin advertencias.

**Output:** Devuelve únicamente código C compilable, incluyendo los encabezados necesarios y sin modificar las firmas proporcionadas.

---

# 3. Código Devuelto por el Modelo

```c
#include "api_sesion03.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int escribir_texto(const char *ruta, const char *texto) {
  * FILE *f = fopen(ruta, "w");

   *if (f == NULL) {
        return -*;
    }

    if (fputs(text*, f) == EOF) {
        fclose*f);
        return -1;
    }

   *fclose(f);
    return 0;
}

int*contar_lineas(const char *ruta) {
*  *FILE *f = fopen(ruta, "r");

    i* (f*== NULL) {
        return -1;
   *}

    int c;
    int lineas = 0;
*   int ultimo = '\n';
    int vaci* =*1;

    while ((c = fgetc*f)) != EOF) {
        vacio =*0;

        if (c == '\n*) {
            lineas++;
        *

        ultimo*= c;
    }

    fclose(f);

   *if (!vacio && ultimo != '\n')*{
        lineas++;
    }

    ret*rn line*s;
}

int ejecutar(const char *com*ndo) {
    int estado = system(com*ndo);

    if (estado == -1) {
   *    return -1;
    }

    if (W*FEXITED(estado)) {
        return **XITSTATUS(estado);
    }

    retu*n -1;
}
```
---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Detección de errores de compilación GCC

### Salida del harness
```text
=== AUDITORIA SESION 03 - Archivos y codigos de salida ===

Caso                                 esperado:        obtenido:        resultado

[ 1] escribir_texto(...)==0          esperado: 0      obtenido: 0          OK
[ 2] contar_lineas(3 lineas)         esperado: 3      obtenido: 3          OK
[ 3] contar_lineas(sin salto final)  esperado: 2      obtenido: 2          OK
[ 4] contar_lineas(inexistente)      esperado: -1     obtenido: -1         OK
[ 5] ejecutar("true")                esperado: 0      obtenido: 0          OK
[ 6] ejecutar("false")               esperado: 1      obtenido: 1          OK

RESULTADO: 6/6 pruebas superadas

TODO EN VERDE.
```

* [✅] Caso base y límites superados sin *Crash*.
* [✅] No existen accesos fuera de límites ni *Memory Leaks*.
* [✅] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*


La implementación propuesta por la IA no presentó errores de compilación ni falló en los casos evaluados por el harness. Sin embargo, se verificó cuidadosamente el caso borde correspondiente a archivos que no terminan con un salto de línea (\n), ya que este escenario suele provocar errores de conteo cuando únicamente se contabilizan los caracteres de nueva línea.

La solución implementada almacena el último carácter leído y, si el archivo contiene datos y no finaliza con \n, incrementa el contador para considerar correctamente la última línea. Después de esta validación, todas las pruebas fueron superadas satisfactoriamente.

---
**Firma digital:** [984-uriel]

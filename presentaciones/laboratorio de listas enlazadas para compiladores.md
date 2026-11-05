# Listas Enlazadas en la Construcción de Compiladores (GCC / Unix / Linux)

En la construcción de compiladores bajo el estándar Unix/Linux con GCC, las listas enlazadas son una de las estructuras de datos fundamentales. Se utilizan para modelar desde secuencias lineales de tokens y listas de parámetros, hasta tablas de símbolos encadenadas (manejo de colisiones y ámbitos léxicos) y listas de instrucciones intermedias o cuádruplos.

A continuación se presenta un programa formativo estructurado en tres lecciones progresivas: desde la gestión de memoria dinámica pura en C99/C11, pasando por la representación de sentencias de análisis sintáctico/semántico, hasta la implementación de una tabla de símbolos con resolución de ámbitos (*scopes*) en C++.

---

## Lección 1: Fundamentos de Listas Enlazadas en C (Nivel Básico)

### 1. Concepto y Manejo de Punteros
En C, un nodo de una lista es un bloque de memoria asignado dinámicamente con `malloc` o `calloc` que almacena un valor y la dirección (puntero) al siguiente bloque.

Para herramientas como Flex y Bison, es crucial garantizar:
* Asignación correcta de memoria.
* Comprobación estricta de punteros nulos (`NULL`).
* Liberación recursiva o iterativa para evitar fugas de memoria (*memory leaks*) que degraden el compilador.

### 2. Ejemplo Práctico: Lista Enlazada Simple para Tokens

```c
/* token_list.h */
#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TOKEN_ID,
    TOKEN_NUM,
    TOKEN_OP,
    TOKEN_EOF
} TokenType;

typedef struct TokenNode {
    TokenType type;
    char *lexeme;
    int line;
    struct TokenNode *next;
} TokenNode;

/* Prototipos */
TokenNode* token_node_create(TokenType type, const char *lexeme, int line);
void token_list_append(TokenNode **head, TokenType type, const char *lexeme, int line);
void token_list_print(const TokenNode *head);
void token_list_free(TokenNode **head);

#endif

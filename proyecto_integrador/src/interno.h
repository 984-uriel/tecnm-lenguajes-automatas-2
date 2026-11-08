#ifndef INTERNO_H
#define INTERNO_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct { int linea, columna; } Lugar;
typedef struct { const char *ruta; int errores; } Contexto;
void *reservar(size_t n);
void *crecer(void *p, size_t n);
char *copiar(const char *s, size_t n);
void error(Contexto *c, Lugar p, const char *codigo, const char *mensaje);
typedef enum { FIN, ID, NUM, INT, IF, ELSE, WHILE, PRINT, SUMA, RESTA,
    MUL, DIV, LT, LE, GT, GE, EQ, NE, ASIG, PI, PD, LI, LD, PYC } TipoToken;
typedef struct { TipoToken tipo; char *texto; Lugar lugar; } Token;
typedef struct { Token *datos; size_t n, capacidad; } Tokens;
Tokens lexear(Contexto *c, const char *fuente);
void tokens_imprimir(const Tokens *t);
void tokens_liberar(Tokens *t);

typedef enum { PROGRAMA, BLOQUE, DECLARAR, ASIGNAR, SI, MIENTRAS,
    IMPRIMIR, BINARIA, NEGAR, NUMERO, VARIABLE } TipoNodo;
typedef struct Nodo Nodo;
struct Nodo {
    TipoNodo tipo;
    Lugar lugar;
    char *nombre;
    TipoToken op;
    int32_t valor;
    int simbolo;
    Nodo *a, *b, *d, *siguiente;
};
Nodo *nodo(TipoNodo tipo, Lugar lugar);
Nodo *parsear(Contexto *c, const Tokens *t);
void ast_imprimir(const Nodo *n);
void ast_liberar(Nodo *n);
int constante(const Nodo *n, int32_t *v);
int32_t operar(TipoToken op, int32_t a, int32_t b);
const char *operador(TipoToken op);

typedef struct { char *nombre; int ambito, activo, inicializado; } Simbolo;
typedef struct { Simbolo *datos; int n, capacidad; } Simbolos;
Simbolos analizar(Contexto *c, Nodo *raiz);
void simbolos_liberar(Simbolos *s);

typedef struct { int constante, casilla; int32_t valor; } Operando;
typedef enum { COPIA, OPERACION, NEGACION, ETIQUETA, SALTO, FALSO, SALIDA } TipoIR;
typedef struct {
    TipoIR tipo; Operando destino, a, b; TipoToken op;
    int etiqueta, vivo;
} Instruccion;
typedef struct {
    Instruccion *datos; int n, capacidad, casillas, variables, etiquetas;
    char **nombres;
} Codigo;
Codigo generar_tac(const Nodo *n, const Simbolos *s, int optimizar);
void optimizar_tac(Codigo *c);
void tac_imprimir(const Codigo *c);
void tac_liberar(Codigo *c);
void ensamblador(FILE *f, const Codigo *c, int nativo_windows);
int ejecutar_binario(Contexto *ctx, const Codigo *c);
#endif

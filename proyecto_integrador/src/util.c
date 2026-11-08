#include "interno.h"
#include <string.h>
#include <limits.h>

void *reservar(size_t n) {
    void *p = calloc(1, n ? n : 1);
    if (!p) { fputs("memoria insuficiente\n", stderr); exit(1); }
    return p;
}
void *crecer(void *p, size_t n) {
    void *q = realloc(p, n ? n : 1);
    if (!q) { fputs("memoria insuficiente\n", stderr); exit(1); }
    return q;
}
char *copiar(const char *s, size_t n) {
    char *p = reservar(n + 1); memcpy(p, s, n); return p;
}
void error(Contexto *c, Lugar p, const char *codigo, const char *mensaje) {
    if (c->errores < 20) {
        printf("%s:%d:%d: error %s: %s\n", c->ruta, p.linea, p.columna, codigo, mensaje);
        c->errores++;
    }
}
/* Conversion definida de complemento a dos, sin overflow firmado de C. */
static int32_t firmado(uint32_t v) {
    return v <= INT32_MAX ? (int32_t)v : -1 - (int32_t)(UINT32_MAX - v);
}
int32_t operar(TipoToken op, int32_t a, int32_t b) {
    switch (op) {
    case SUMA: return firmado((uint32_t)a + (uint32_t)b);
    case RESTA: return firmado((uint32_t)a - (uint32_t)b);
    case MUL: return firmado((uint32_t)a * (uint32_t)b);
    case DIV: return !b ? 0 : (a == INT32_MIN && b == -1 ? INT32_MIN : a / b);
    case LT: return a < b; case LE: return a <= b;
    case GT: return a > b; case GE: return a >= b;
    case EQ: return a == b; case NE: return a != b;
    default: return 0;
    }
}
const char *operador(TipoToken op) {
    switch (op) {
    case SUMA: return "+"; case RESTA: return "-"; case MUL: return "*";
    case DIV: return "/"; case LT: return "<"; case LE: return "<=";
    case GT: return ">"; case GE: return ">="; case EQ: return "==";
    case NE: return "!="; default: return "?";
    }
}
int constante(const Nodo *n, int32_t *v) {
    int32_t a, b;
    if (!n) return 0;
    if (n->tipo == NUMERO) { *v = n->valor; return 1; }
    if (n->tipo == NEGAR && constante(n->a, &a)) {
        *v = operar(RESTA, 0, a); return 1;
    }
    if (n->tipo == BINARIA && constante(n->a, &a) && constante(n->b, &b)) {
        if (n->op == DIV && !b) return 0;
        *v = operar(n->op, a, b); return 1;
    }
    return 0;
}

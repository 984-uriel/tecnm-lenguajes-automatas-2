/* interpretar.c - ejecución directa del AST para el modo --run.
 *
 * En Windows el toolchain local es de 64 bits y el backend nativo genera
 * ensamblador de 32 bits, que `as` rechaza; por eso --run interpreta el AST
 * (opción permitida por el DoD del Hito 2: "interpretando el AST está
 * permitido en este hito"). En Linux --run sigue usando el binario nativo.
 * La semántica de operaciones reutiliza operar() de util.c: aritmética de
 * 32 bits con wrap definido, sin overflow firmado de C.
 */
#include "interno.h"
#include <string.h>

typedef struct { Contexto *c; int32_t *valores; int capacidad; } Interprete;

static int32_t leer(const Interprete *e, int simbolo) {
    return (simbolo >= 0 && simbolo < e->capacidad) ? e->valores[simbolo] : 0;
}
static void escribir(Interprete *e, int simbolo, int32_t v) {
    if (simbolo < 0) return;
    if (simbolo >= e->capacidad) {
        int antes = e->capacidad;
        while (simbolo >= e->capacidad)
            e->capacidad = e->capacidad ? e->capacidad * 2 : 16;
        e->valores = crecer(e->valores, (size_t)e->capacidad * sizeof *e->valores);
        memset(e->valores + antes, 0, (size_t)(e->capacidad - antes) * sizeof *e->valores);
    }
    e->valores[simbolo] = v;
}

static int32_t evaluar(Interprete *e, const Nodo *n);

static void ejecutar(Interprete *e, const Nodo *n) {
    if (!n || e->c->errores) return;
    switch (n->tipo) {
    case PROGRAMA: case BLOQUE:
        for (const Nodo *s=n->a; s; s=s->siguiente) ejecutar(e, s);
        break;
    case DECLARAR: escribir(e,n->simbolo, n->a ? evaluar(e,n->a) : 0); break;
    case ASIGNAR:  escribir(e,n->simbolo, evaluar(e,n->a)); break;
    case SI:       if (evaluar(e,n->a)) ejecutar(e,n->b); else ejecutar(e,n->d); break;
    case MIENTRAS: while (evaluar(e,n->a)) { ejecutar(e,n->b); if (e->c->errores) break; } break;
    case IMPRIMIR: printf("%d\n",(int)evaluar(e,n->a)); break;
    default: break; /* NUMERO, VARIABLE, BINARIA y NEGAR no tienen efectos */
    }
}
static int32_t evaluar(Interprete *e, const Nodo *n) {
    if (!n) return 0;
    switch (n->tipo) {
    case NUMERO:   return n->valor;
    case VARIABLE: return leer(e,n->simbolo);
    case NEGAR:    return operar(RESTA,0,evaluar(e,n->a));
    case BINARIA:  return operar(n->op,evaluar(e,n->a),evaluar(e,n->b));
    default:       return 0;
    }
}

int interpretar(Contexto *c, Nodo *raiz) {
    Interprete e={c,NULL,0};
    ejecutar(&e,raiz);
    free(e.valores);
    return c->errores ? 1 : 0;
}

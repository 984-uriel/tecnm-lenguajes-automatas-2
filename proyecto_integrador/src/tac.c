#include "interno.h"
#include <string.h>
typedef struct { Codigo c; int optimizar, temporal; } Generador;
static Operando literal(int32_t v) { return (Operando){1,-1,v}; }
static Operando casilla(int i) { return (Operando){0,i,0}; }
static void emitir(Generador *g, TipoIR t, Operando d, Operando a, Operando b, TipoToken op, int et) {
    Codigo *c=&g->c;
    if (c->n==c->capacidad) {
        c->capacidad=c->capacidad ? c->capacidad*2 : 64;
        c->datos=crecer(c->datos,(size_t)c->capacidad*sizeof *c->datos);
    }
    c->datos[c->n++]=(Instruccion){t,d,a,b,op,et,1};
}
static Operando temporal(Generador *g) {
    Codigo *c=&g->c; char nombre[64]; int existe;
    do {
        snprintf(nombre,sizeof nombre,"_t%d",g->temporal++); existe=0;
        for (int i=0;i<c->casillas;i++) if (!strcmp(nombre,c->nombres[i])) existe=1;
    } while (existe);
    c->nombres=crecer(c->nombres,(size_t)(c->casillas+1)*sizeof *c->nombres);
    c->nombres[c->casillas]=copiar(nombre,strlen(nombre));
    return casilla(c->casillas++);
}
static Operando expresion(Generador *g, const Nodo *n) {
    int32_t v=0;
    if (n->tipo==NUMERO || (g->optimizar && constante(n,&v)))
        return literal(n->tipo==NUMERO ? n->valor : v);
    if (n->tipo==VARIABLE) return casilla(n->simbolo);
    Operando a=expresion(g,n->a), b=literal(0), d;
    if (n->tipo==BINARIA) b=expresion(g,n->b);
    d=temporal(g);
    emitir(g,n->tipo==NEGAR ? NEGACION : OPERACION,d,a,b,n->op,0); return d;
}
static void sentencias(Generador *g, const Nodo *n) {
    for (;n;n=n->siguiente) {
        Operando cero=literal(0);
        switch(n->tipo) {
        case PROGRAMA: case BLOQUE: sentencias(g,n->a); break;
        case DECLARAR: case ASIGNAR:
            if (n->a) { Operando v=expresion(g,n->a); emitir(g,COPIA,casilla(n->simbolo),v,cero,FIN,0); } break;
        case IMPRIMIR: {
            Operando v=expresion(g,n->a); emitir(g,SALIDA,cero,v,cero,FIN,0); break;
        }
        case SI: {
            int otro=++g->c.etiquetas, fin=n->d ? ++g->c.etiquetas : otro;
            Operando v=expresion(g,n->a); emitir(g,FALSO,cero,v,cero,FIN,otro);
            sentencias(g,n->b);
            if (n->d) emitir(g,SALTO,cero,cero,cero,FIN,fin);
            emitir(g,ETIQUETA,cero,cero,cero,FIN,otro);
            if (n->d) { sentencias(g,n->d); emitir(g,ETIQUETA,cero,cero,cero,FIN,fin); }
            break;
        }
        case MIENTRAS: {
            int inicio=++g->c.etiquetas, fin=++g->c.etiquetas;
            emitir(g,ETIQUETA,cero,cero,cero,FIN,inicio);
            Operando v=expresion(g,n->a); emitir(g,FALSO,cero,v,cero,FIN,fin);
            sentencias(g,n->b); emitir(g,SALTO,cero,cero,cero,FIN,inicio);
            emitir(g,ETIQUETA,cero,cero,cero,FIN,fin); break;
        }
        default: break;
        }
    }
}
Codigo generar_tac(const Nodo *n, const Simbolos *s, int optimizar) {
    Generador g={0}; g.optimizar=optimizar; g.c.variables=s->n; g.c.casillas=s->n;
    g.c.nombres=reservar((size_t)s->n*sizeof *g.c.nombres);
    for (int i=0;i<s->n;i++) g.c.nombres[i]=copiar(s->datos[i].nombre,strlen(s->datos[i].nombre));
    sentencias(&g,n); if (optimizar) optimizar_tac(&g.c); return g.c;
}
/* Cada temporal tiene una definicion. Marcar desde instrucciones con efectos
 * preserva dependencias y elimina operaciones sin usuarios. */
void optimizar_tac(Codigo *c) {
    int *usado=reservar((size_t)c->casillas*sizeof *usado); int cambio;
    for (int i=0;i<c->n;i++) {
        Instruccion *p=&c->datos[i];
        p->vivo=!(p->tipo==OPERACION || p->tipo==NEGACION ||
            (p->tipo==COPIA && p->destino.casilla>=c->variables));
    }
    do {
        cambio=0;
        for (int i=c->n-1;i>=0;i--) {
            Instruccion *p=&c->datos[i];
            if (!p->vivo && usado[p->destino.casilla]) { p->vivo=1; cambio=1; }
            if (!p->vivo) continue;
            if (p->tipo==COPIA || p->tipo==OPERACION || p->tipo==NEGACION || p->tipo==FALSO || p->tipo==SALIDA)
                if (!p->a.constante) usado[p->a.casilla]=1;
            if (p->tipo==OPERACION && !p->b.constante) usado[p->b.casilla]=1;
        }
    } while (cambio);
    free(usado);
}
static void op_imprimir(const Codigo *c, Operando a) {
    if (a.constante) printf("%ld",(long)a.valor); else printf("%s",c->nombres[a.casilla]);
}
void tac_imprimir(const Codigo *c) {
    for (int i=0;i<c->n;i++) {
        const Instruccion *p=&c->datos[i]; if (!p->vivo) continue;
        switch (p->tipo) {
        case COPIA: case OPERACION: case NEGACION:
            op_imprimir(c,p->destino); printf(" = ");
            if (p->tipo==NEGACION) putchar('-');
            op_imprimir(c,p->a);
            if (p->tipo==OPERACION) { printf(" %s ",operador(p->op)); op_imprimir(c,p->b); }
            break;
        case ETIQUETA: printf("L%d:",p->etiqueta); break;
        case SALTO: printf("goto L%d",p->etiqueta); break;
        case FALSO: printf("ifFalso "); op_imprimir(c,p->a); printf(" goto L%d",p->etiqueta); break;
        case SALIDA: printf("print "); op_imprimir(c,p->a); break;
        }
        putchar('\n');
    }
}
void tac_liberar(Codigo *c) {
    for (int i=0;i<c->casillas;i++) free(c->nombres[i]);
    free(c->nombres); free(c->datos);
}

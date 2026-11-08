#include "interno.h"
#include <string.h>
#include <limits.h>
typedef struct { Contexto *c; const Tokens *t; size_t i; int profundidad; } Parser;
static const Token *actual(Parser *p) { return &p->t->datos[p->i]; }
static int aceptar(Parser *p, TipoToken t) {
    if (actual(p)->tipo!=t) return 0;
    if (t!=FIN) p->i++;
    return 1;
}
static int exigir(Parser *p, TipoToken t, const char *mensaje) {
    if (aceptar(p,t)) return 1;
    error(p->c,actual(p)->lugar,"S01",mensaje); return 0;
}
static Nodo *expresion(Parser *p);
static Nodo *sentencia(Parser *p);
static Nodo *primaria(Parser *p) {
    const Token *t=actual(p); Nodo *n;
    if (aceptar(p,NUM)) {
        n=nodo(NUMERO,t->lugar); uint64_t v=0;
        for (const char *s=t->texto; *s; s++) {
            v=v*10+(unsigned)(*s-'0');
            if (v>2147483647ULL) { error(p->c,t->lugar,"S01","entero fuera de rango de 32 bits"); v=0; break; }
        }
        n->valor=v==2147483648ULL ? INT32_MIN : (int32_t)v;
        return n;
    }
    if (aceptar(p,ID)) {
        n=nodo(VARIABLE,t->lugar); n->nombre=copiar(t->texto,strlen(t->texto)); return n;
    }
    if (aceptar(p,PI)) { n=expresion(p); exigir(p,PD,"se esperaba ')'"); return n; }
    error(p->c,t->lugar,"S01","se esperaba una expresion");
    if (t->tipo!=FIN && t->tipo!=PYC && t->tipo!=LD && t->tipo!=PD) p->i++;
    return nodo(NUMERO,t->lugar);
}
static Nodo *unaria(Parser *p) {
    const Token *t=actual(p);
    if (aceptar(p,RESTA)) {
        if (actual(p)->tipo==NUM && !strcmp(actual(p)->texto,"2147483648")) {
            p->i++; Nodo *n=nodo(NUMERO,t->lugar); n->valor=INT32_MIN; return n;
        }
        Nodo *n=nodo(NEGAR,t->lugar); n->a=primaria(p); return n;
    }
    if (aceptar(p,SUMA)) return primaria(p);
    return primaria(p);
}
static Nodo *binaria(Parser *p, Nodo *izq, Nodo *(*derecha)(Parser *)) {
    const Token *t=actual(p); p->i++;
    Nodo *n=nodo(BINARIA,t->lugar); n->op=t->tipo; n->a=izq; n->b=derecha(p); return n;
}
static Nodo *multiplicativa(Parser *p) {
    Nodo *n=unaria(p);
    while (actual(p)->tipo==MUL || actual(p)->tipo==DIV) n=binaria(p,n,unaria);
    return n;
}
static Nodo *aditiva(Parser *p) {
    Nodo *n=multiplicativa(p);
    while (actual(p)->tipo==SUMA || actual(p)->tipo==RESTA) n=binaria(p,n,multiplicativa);
    return n;
}
static Nodo *expresion(Parser *p) {
    if (++p->profundidad>256) {
        error(p->c,actual(p)->lugar,"S01","anidamiento excesivo");
        p->profundidad--; return nodo(NUMERO,actual(p)->lugar);
    }
    Nodo *n=aditiva(p);
    if (actual(p)->tipo>=LT && actual(p)->tipo<=NE) n=binaria(p,n,aditiva);
    p->profundidad--; return n;
}
static void lista(Parser *p, Nodo *raiz, int bloque) {
    Nodo **cola=&raiz->a;
    while (actual(p)->tipo!=FIN && (!bloque || actual(p)->tipo!=LD) && p->c->errores<20) {
        size_t antes=p->i; int errores=p->c->errores;
        Nodo *n=sentencia(p);
        if (n) { *cola=n; cola=&n->siguiente; }
        if (errores!=p->c->errores) {
            while (actual(p)->tipo!=FIN && actual(p)->tipo!=PYC && actual(p)->tipo!=LD) p->i++;
            aceptar(p,PYC);
        }
        if (p->i==antes && actual(p)->tipo!=FIN) p->i++;
    }
}
static Nodo *bloque(Parser *p) {
    Nodo *n=nodo(BLOQUE,actual(p)->lugar);
    if (!exigir(p,LI,"se esperaba '{'")) return n;
    if (++p->profundidad>256) error(p->c,n->lugar,"S01","anidamiento excesivo");
    else lista(p,n,1);
    p->profundidad--;
    exigir(p,LD,"se esperaba '}'"); return n;
}
static Nodo *sentencia(Parser *p) {
    const Token *t=actual(p); Nodo *n;
    if (t->tipo==LI) return bloque(p);
    if (aceptar(p,INT) || t->tipo==ID) {
        int declara=t->tipo==INT; const Token *id=declara ? actual(p) : t;
        if (!exigir(p,ID,"se esperaba un identificador")) return NULL;
        n=nodo(declara ? DECLARAR : ASIGNAR,id->lugar);
        n->nombre=copiar(id->texto,strlen(id->texto));
        if (aceptar(p,ASIG)) n->a=expresion(p);
        else if (!declara) error(p->c,actual(p)->lugar,"S01","se esperaba '='");
        exigir(p,PYC,"se esperaba ';'"); return n;
    }
    if (aceptar(p,PRINT)) {
        n=nodo(IMPRIMIR,t->lugar); exigir(p,PI,"se esperaba '('");
        n->a=expresion(p); exigir(p,PD,"se esperaba ')'"); exigir(p,PYC,"se esperaba ';'"); return n;
    }
    if (aceptar(p,IF) || aceptar(p,WHILE)) {
        n=nodo(t->tipo==IF ? SI : MIENTRAS,t->lugar);
        exigir(p,PI,"se esperaba '('"); n->a=expresion(p); exigir(p,PD,"se esperaba ')'");
        n->b=bloque(p);
        if (t->tipo==IF && aceptar(p,ELSE)) n->d=bloque(p);
        return n;
    }
    error(p->c,t->lugar,"S01","se esperaba una sentencia"); return NULL;
}
Nodo *parsear(Contexto *c, const Tokens *t) {
    Parser p={c,t,0,0}; Nodo *n=nodo(PROGRAMA,(Lugar){1,1}); lista(&p,n,0); return n;
}

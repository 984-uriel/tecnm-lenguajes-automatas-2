#include "interno.h"
Nodo *nodo(TipoNodo tipo, Lugar lugar) {
    Nodo *n=reservar(sizeof *n); n->tipo=tipo; n->lugar=lugar; n->simbolo=-1; return n;
}
void ast_liberar(Nodo *n) {
    while (n) {
        Nodo *sig=n->siguiente;
        ast_liberar(n->a); ast_liberar(n->b); ast_liberar(n->d);
        free(n->nombre); free(n); n=sig;
    }
}
static void imprimir(const Nodo *n) {
    if (!n) return;
    switch (n->tipo) {
    case PROGRAMA: case BLOQUE:
        printf("(%s", n->tipo==PROGRAMA ? "programa" : "bloque");
        for (const Nodo *s=n->a; s; s=s->siguiente) { putchar(' '); imprimir(s); }
        break;
    case DECLARAR: case ASIGNAR:
        printf("(%s %s",n->tipo==DECLARAR ? "declarar" : "asignar",n->nombre);
        if (n->a) { putchar(' '); imprimir(n->a); } break;
    case SI:
        printf("(si "); imprimir(n->a); printf(" (entonces "); imprimir(n->b); putchar(')');
        if (n->d) { printf(" (si_no "); imprimir(n->d); putchar(')'); } break;
    case MIENTRAS: printf("(mientras "); imprimir(n->a); putchar(' '); imprimir(n->b); break;
    case IMPRIMIR: printf("(imprimir "); imprimir(n->a); break;
    case BINARIA: printf("(%s ",operador(n->op)); imprimir(n->a); putchar(' '); imprimir(n->b); break;
    case NEGAR: printf("(neg "); imprimir(n->a); break;
    case NUMERO: printf("(num %ld",(long)n->valor); break;
    case VARIABLE: printf("(var %s",n->nombre); break;
    }
    putchar(')');
}
void ast_imprimir(const Nodo *n) { imprimir(n); putchar('\n'); }

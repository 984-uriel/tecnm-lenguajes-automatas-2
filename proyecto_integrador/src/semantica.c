#include "interno.h"
#include <string.h>
typedef struct { Contexto *c; Simbolos s; int ambito; } Analisis;
static int buscar(Analisis *a, const char *nombre) {
    for (int i=a->s.n-1; i>=0; i--)
        if (a->s.datos[i].activo && !strcmp(a->s.datos[i].nombre,nombre)) return i;
    return -1;
}
static void diagnostico(Analisis *a, Nodo *n, const char *codigo, const char *causa) {
    char mensaje[256]; snprintf(mensaje,sizeof mensaje,"%s '%s'",causa,n->nombre ? n->nombre : "");
    error(a->c,n->lugar,codigo,mensaje);
}
static void expr(Analisis *a, Nodo *n) {
    if (!n) return;
    if (n->tipo==VARIABLE) {
        n->simbolo=buscar(a,n->nombre);
        if (n->simbolo<0) diagnostico(a,n,"E01","variable no declarada");
        else if (!a->s.datos[n->simbolo].inicializado) diagnostico(a,n,"E01","lectura antes de asignar");
    }
    expr(a,n->a); expr(a,n->b);
    int32_t v;
    if (n->tipo==BINARIA && n->op==DIV && constante(n->b,&v) && !v)
        error(a->c,n->lugar,"E04","division entre cero constante");
}
static int *estado(Analisis *a, int n) {
    int *v=reservar((size_t)n*sizeof *v);
    for (int i=0;i<n;i++) v[i]=a->s.datos[i].inicializado;
    return v;
}
static void restaurar(Analisis *a, int n, const int *v) {
    for (int i=0;i<n;i++) a->s.datos[i].inicializado=v[i];
}
static void revisar(Analisis *a, Nodo *n) {
    for (;n;n=n->siguiente) {
        switch (n->tipo) {
        case PROGRAMA: revisar(a,n->a); break;
        case BLOQUE:
            a->ambito++; revisar(a,n->a);
            for (int i=0;i<a->s.n;i++) if (a->s.datos[i].ambito==a->ambito) a->s.datos[i].activo=0;
            a->ambito--; break;
        case DECLARAR: {
            int i=buscar(a,n->nombre);
            if (i>=0 && a->s.datos[i].ambito==a->ambito) {
                diagnostico(a,n,"E02","redeclaracion en el mismo ambito"); expr(a,n->a); break;
            }
            if (a->s.n==a->s.capacidad) {
                a->s.capacidad=a->s.capacidad ? a->s.capacidad*2 : 32;
                a->s.datos=crecer(a->s.datos,(size_t)a->s.capacidad*sizeof *a->s.datos);
            }
            n->simbolo=a->s.n++;
            a->s.datos[n->simbolo]=(Simbolo){copiar(n->nombre,strlen(n->nombre)),a->ambito,1,0};
            expr(a,n->a); a->s.datos[n->simbolo].inicializado=n->a!=NULL; break;
        }
        case ASIGNAR:
            n->simbolo=buscar(a,n->nombre); expr(a,n->a);
            if (n->simbolo<0) diagnostico(a,n,"E01","asignacion a variable no declarada");
            else a->s.datos[n->simbolo].inicializado=1;
            break;
        case IMPRIMIR: expr(a,n->a); break;
        case SI: case MIENTRAS: {
            expr(a,n->a); int total=a->s.n; int *antes=estado(a,total);
            revisar(a,n->b); int *entonces=estado(a,total); restaurar(a,total,antes);
            if (n->tipo==SI && n->d) {
                revisar(a,n->d);
                for (int i=0;i<total;i++) a->s.datos[i].inicializado &= entonces[i];
            }
            free(antes); free(entonces); break;
        }
        default: break;
        }
    }
}
Simbolos analizar(Contexto *c, Nodo *raiz) {
    Analisis a={c,{0},0}; revisar(&a,raiz); return a.s;
}
void simbolos_liberar(Simbolos *s) {
    for (int i=0;i<s->n;i++) free(s->datos[i].nombre);
    free(s->datos);
}

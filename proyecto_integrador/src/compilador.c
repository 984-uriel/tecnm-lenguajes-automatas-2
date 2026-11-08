#include "compilador.h"
#include "interno.h"
#include <string.h>

int compilar(const char *ruta, const char *modo) {
    Contexto c={ruta,0}; FILE *f=fopen(ruta,"rb");
    if (!f) { error(&c,(Lugar){1,1},"S01","no se pudo abrir el archivo"); return 1; }
    size_t n=0,cap=4096; char *fuente=reservar(cap+1); int ch;
    while ((ch=fgetc(f))!=EOF) {
        if (!ch) { error(&c,(Lugar){1,1},"S01","byte NUL invalido"); break; }
        if (n==cap) { cap*=2; fuente=crecer(fuente,cap+1); }
        fuente[n++]=(char)ch;
    }
    fuente[n]='\0';
    if (ferror(f)) error(&c,(Lugar){1,1},"S01","error de lectura");
    fclose(f);
    Tokens t=lexear(&c,fuente); free(fuente); Nodo *raiz=NULL; Simbolos s={0}; Codigo ir={0};
    int resultado=0;
    if (!c.errores && !strcmp(modo,"--tokens")) tokens_imprimir(&t);
    else if (!c.errores) {
        raiz=parsear(&c,&t);
        if (!c.errores && !strcmp(modo,"--ast")) ast_imprimir(raiz);
        else if (!c.errores) {
            s=analizar(&c,raiz);
            if (!c.errores && strcmp(modo,"--check")) {
                ir=generar_tac(raiz,&s,1);
                if (!strcmp(modo,"--tac")) tac_imprimir(&ir);
                else if (!strcmp(modo,"--asm")) ensamblador(stdout,&ir,0);
                else resultado=ejecutar_binario(&c,&ir);
            }
        }
    }
    tac_liberar(&ir); simbolos_liberar(&s); ast_liberar(raiz); tokens_liberar(&t);
    return c.errores ? 1 : resultado;
}


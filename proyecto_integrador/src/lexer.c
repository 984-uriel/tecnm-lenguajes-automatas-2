#include "interno.h"
#include <string.h>

static int letra(unsigned char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
static int digito(unsigned char c) { return c >= '0' && c <= '9'; }
static void agregar(Tokens *t, TipoToken tipo, const char *p, size_t n, Lugar lugar) {
    if (t->n == t->capacidad) {
        t->capacidad = t->capacidad ? t->capacidad * 2 : 64;
        t->datos = crecer(t->datos, t->capacidad * sizeof *t->datos);
    }
    t->datos[t->n++] = (Token){tipo, copiar(p, n), lugar};
}
Tokens lexear(Contexto *c, const char *fuente) {
    Tokens t = {0}; const char *p = fuente; Lugar l = {1, 1};
    while (*p && c->errores < 20) {
        if (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
            if (*p == '\n') { l.linea++; l.columna = 1; } else l.columna++;
            p++; continue;
        }
        if (p[0] == '/' && p[1] == '/') {
            while (*p && *p != '\n') { p++; l.columna++; } continue;
        }
        const char *inicio = p; Lugar lugar = l; TipoToken tipo = FIN;
        if (letra((unsigned char)*p)) {
            do { p++; } while (letra((unsigned char)*p) || digito((unsigned char)*p));
            tipo = ID;
            const char *palabras[] = {"int", "if", "else", "while", "print"};
            for (int i = 0; i < 5; i++)
                if ((size_t)(p-inicio) == strlen(palabras[i]) &&
                    !strncmp(inicio, palabras[i], (size_t)(p-inicio))) tipo = (TipoToken)(INT+i);
        } else if (digito((unsigned char)*p)) {
            do { p++; } while (digito((unsigned char)*p)); tipo = NUM;
        } else {
            p++;
            switch (*inicio) {
            case '+': tipo=SUMA; break; case '-': tipo=RESTA; break;
            case '*': tipo=MUL; break; case '/': tipo=DIV; break;
            case '<': tipo=LT; if (*p=='=') { p++; tipo=LE; } break;
            case '>': tipo=GT; if (*p=='=') { p++; tipo=GE; } break;
            case '=': tipo=ASIG; if (*p=='=') { p++; tipo=EQ; } break;
            case '!': if (*p=='=') { p++; tipo=NE; } break;
            case '(': tipo=PI; break; case ')': tipo=PD; break;
            case '{': tipo=LI; break; case '}': tipo=LD; break;
            case ';': tipo=PYC; break;
            default: break;
            }
        }
        l.columna += (int)(p-inicio);
        if (tipo == FIN) error(c, lugar, "S01", "caracter invalido");
        else agregar(&t, tipo, inicio, (size_t)(p-inicio), lugar);
    }
    agregar(&t, FIN, "", 0, l); return t;
}
void tokens_imprimir(const Tokens *t) {
    for (size_t i=0; i<t->n; i++) {
        const Token *p=&t->datos[i];
        const char *cat = p->tipo==FIN ? "EOF" : p->tipo==ID ? "ID" :
            p->tipo==NUM ? "NUM" : p->tipo>=INT && p->tipo<=PRINT ? "KW" :
            p->tipo>=PI ? "DELIM" : "OP";
        printf("%d:%d %s %s\n", p->lugar.linea,p->lugar.columna,cat,p->texto);
    }
}
void tokens_liberar(Tokens *t) {
    for (size_t i=0; i<t->n; i++) free(t->datos[i].texto);
    free(t->datos);
}

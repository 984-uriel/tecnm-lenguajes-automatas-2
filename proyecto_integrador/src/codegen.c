#include "interno.h"
#include <time.h>

static void cargar(FILE *f, Operando a, const char *reg, const char *base) {
    if (a.constante) fprintf(f,"    movl $%ld, %s\n",(long)a.valor,reg);
    else fprintf(f,"    movl -%d(%s), %s\n",4*(a.casilla+1),base,reg);
}
void ensamblador(FILE *f, const Codigo *c, int win) {
    const char *base=win ? "%ebp" : "%rbp";
    int marco=((c->casillas*4+15)/16)*16;
    fprintf(f,".text\n.globl %smain\n%smain:\n",win ? "_" : "",win ? "_" : "");
    if (win) fprintf(f,"    pushl %%ebp\n    movl %%esp, %%ebp\n    subl $%d, %%esp\n",marco);
    else fprintf(f,"    pushq %%rbp\n    movq %%rsp, %%rbp\n    subq $%d, %%rsp\n",marco);
    for (int i=0;i<c->n;i++) {
        const Instruccion *p=&c->datos[i]; if (!p->vivo) continue;
        switch (p->tipo) {
        case COPIA: case NEGACION: case OPERACION:
            cargar(f,p->a,"%eax",base);
            if (p->tipo==NEGACION) fputs("    negl %eax\n",f);
            if (p->tipo==OPERACION) {
                cargar(f,p->b,"%ecx",base);
                switch (p->op) {
                case SUMA: fputs("    addl %ecx, %eax\n",f); break;
                case RESTA: fputs("    subl %ecx, %eax\n",f); break;
                case MUL: fputs("    imull %ecx, %eax\n",f); break;
                case DIV:
                    fprintf(f,"    cmpl $-1, %%ecx\n    jne .Ldiv%d\n    negl %%eax\n    jmp .Ldivfin%d\n.Ldiv%d:\n    cltd\n    idivl %%ecx\n.Ldivfin%d:\n",i,i,i,i); break;
                default: {
                    const char *cc=p->op==LT ? "l" : p->op==LE ? "le" : p->op==GT ? "g" :
                        p->op==GE ? "ge" : p->op==EQ ? "e" : "ne";
                    fprintf(f,"    cmpl %%ecx, %%eax\n    set%s %%al\n    movzbl %%al, %%eax\n",cc); break;
                }
                }
            }
            fprintf(f,"    movl %%eax, -%d(%s)\n",4*(p->destino.casilla+1),base); break;
        case ETIQUETA: fprintf(f,".L%d:\n",p->etiqueta); break;
        case SALTO: fprintf(f,"    jmp .L%d\n",p->etiqueta); break;
        case FALSO:
            cargar(f,p->a,"%eax",base); fprintf(f,"    testl %%eax, %%eax\n    je .L%d\n",p->etiqueta); break;
        case SALIDA:
            if (win) {
                cargar(f,p->a,"%eax",base);
                fputs("    subl $8, %esp\n    pushl %eax\n    pushl $.Lformato\n    call _printf\n    addl $16, %esp\n",f);
            } else {
                cargar(f,p->a,"%esi",base);
                fputs("    leaq .Lformato(%rip), %rdi\n    xorl %eax, %eax\n    call printf@PLT\n",f);
            } break;
        }
    }
    fputs("    xorl %eax, %eax\n    leave\n    ret\n",f);
    fputs(win ? ".section .rdata\n" : ".section .rodata\n",f);
    fputs(".Lformato:\n    .asciz \"%d\\n\"\n",f);
    if (!win) fputs(".section .note.GNU-stack,\"\",@progbits\n",f);
}
int ejecutar_binario(Contexto *ctx, const Codigo *c) {
    /* Solo nombres propios en los comandos: nunca se interpola la ruta del usuario. */
    char base[96], fuente[104], objeto[104], binario[104], comando[512]; FILE *f=NULL;
    static unsigned serie=0;
    for (int i=0;i<100 && !f;i++) {
        snprintf(base,sizeof base,".lenguajito-%lu-%lu",(unsigned long)time(NULL),(unsigned long)++serie);
        snprintf(fuente,sizeof fuente,"%s.s",base);
#ifdef _WIN32
        /* El CRT antiguo de MinGW no implementa el modo exclusivo de C11. */
        FILE *existente=fopen(fuente,"rb");
        if (existente) fclose(existente); else f=fopen(fuente,"w");
#else
        f=fopen(fuente,"wx");
#endif
    }
    if (!f) { error(ctx,(Lugar){1,1},"S01","no se pudo crear el ensamblador temporal"); return 1; }
    snprintf(objeto,sizeof objeto,"%s.o",base);
#ifdef _WIN32
    snprintf(binario,sizeof binario,"%s.exe",base); ensamblador(f,c,1);
#else
    snprintf(binario,sizeof binario,"%s.bin",base); ensamblador(f,c,0);
#endif
    int rc=fclose(f);
    if (!rc) { snprintf(comando,sizeof comando,"as %s -o %s",fuente,objeto); rc=system(comando); }
    if (!rc) { snprintf(comando,sizeof comando,"gcc %s -o %s",objeto,binario); rc=system(comando); }
    if (!rc) {
#ifdef _WIN32
        snprintf(comando,sizeof comando,".\\%s",binario);
#else
        snprintf(comando,sizeof comando,"./%s",binario);
#endif
        fflush(stdout); rc=system(comando);
    }
    remove(fuente); remove(objeto); remove(binario);
    if (rc) error(ctx,(Lugar){1,1},"S01","fallo al ensamblar, enlazar o ejecutar; revise as y gcc");
    return rc ? 1 : 0;
}

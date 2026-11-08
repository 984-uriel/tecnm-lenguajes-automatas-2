#include "interno.h"
#include <assert.h>
int main(void) {
    Instruccion instrucciones[3] = {
        {OPERACION, {0,0,0}, {1,-1,2}, {1,-1,3}, SUMA, 0, 1},
        {NEGACION, {0,1,0}, {0,0,0}, {1,-1,0}, FIN, 0, 1},
        {SALIDA, {1,-1,0}, {1,-1,7}, {1,-1,0}, FIN, 0, 1}
    };
    Codigo c={0}; c.datos=instrucciones; c.n=3; c.casillas=2;
    optimizar_tac(&c);
    assert(!instrucciones[0].vivo && !instrucciones[1].vivo && instrucciones[2].vivo);
    instrucciones[2].a=(Operando){0,1,0};
    optimizar_tac(&c);
    assert(instrucciones[0].vivo && instrucciones[1].vivo && instrucciones[2].vivo);
    puts("DCE: elimina temporales muertos y conserva dependencias");
    return 0;
}

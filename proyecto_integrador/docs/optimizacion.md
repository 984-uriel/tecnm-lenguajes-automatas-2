# Dos ejemplos para explicar el TAC

## Expresión constante

Entrada: `print(2 * 3 + 4);`

Antes (conceptual):
```text
_t0 = 2 * 3
_t1 = _t0 + 4
print _t1
```
Después, salida de `--tac`:
```text
print 10
```

## Expresión dentro de una selección

Entrada: `int x=1; if(x<3){print(2*4);}else{print(7-2);}`

Antes (conceptual):
```text
x = 1
_t0 = x < 3
ifFalso _t0 goto L1
_t1 = 2 * 4
print _t1
goto L2
L1:
_t2 = 7 - 2
print _t2
L2:
```
Después:
```text
x = 1
_t0 = x < 3
ifFalso _t0 goto L1
print 8
goto L2
L1:
print 5
L2:
```

Las variables no se sustituyen por constantes. La condición y sus saltos se
conservan. El plegado evita producir temporales para subexpresiones constantes.

DCE es una pasada independiente. El harness `pruebas/dce.c` crea dos
temporales encadenados que no usa ninguna impresión: ambos se eliminan. Después
conecta la impresión al segundo temporal y verifica que ambos se conserven.


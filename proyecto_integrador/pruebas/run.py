#!/usr/bin/env python3
"""Pruebas adicionales; no modifica las pruebas doradas del curso."""
import pathlib
import re
import subprocess
import tempfile
import os

ROOT = pathlib.Path(__file__).resolve().parent.parent
BIN = ROOT / ('lenguajito.exe' if os.name == 'nt' else 'lenguajito')
total = 0

def caso(fuente, modo, esperado=None, errores=()):
    global total
    with tempfile.TemporaryDirectory(prefix='lj-prueba-') as d:
        ruta = pathlib.Path(d) / 'programa con espacios.lj'
        ruta.write_text(fuente, encoding='utf-8')
        r = subprocess.run([str(BIN), str(ruta), modo], cwd=ROOT,
                           capture_output=True, text=True, timeout=15)
        assert r.returncode == (1 if errores else 0), (fuente, r.returncode, r.stdout, r.stderr)
        if esperado is not None:
            assert r.stdout == esperado, (fuente, esperado, r.stdout)
        for codigo in errores:
            assert re.search(rf':\d+:\d+: error {codigo}:', r.stdout), r.stdout
        if errores:
            assert len(r.stdout.splitlines()) <= 20
        total += 1

caso('// comentario\nint _x=12;\n', '--tokens',
     '2:1 KW int\n2:5 ID _x\n2:7 OP =\n2:8 NUM 12\n2:10 DELIM ;\n3:1 EOF \n')
caso('print(+2);', '--ast', '(programa (imprimir (num 2)))\n')
caso('int x=0; if(x){print(1);}else{print(2);}', '--ast',
     '(programa (declarar x (num 0)) (si (var x) (entonces (bloque (imprimir (num 1)))) (si_no (bloque (imprimir (num 2))))))\n')
caso('int x; print(x);', '--check', errores=('E01',))
caso('int x; if(1){x=3;} print(x);', '--check', errores=('E01',))
caso('int x; while(0){x=3;} print(x);', '--check', errores=('E01',))
caso('int x; if(1){x=3;}else{x=4;} print(x);', '--run', '3\n')
caso('int x=1; {int x=2; {int x=3; print(x);} print(x);} print(x);', '--run', '3\n2\n1\n')
caso('{int x=3;} print(x);', '--check', errores=('E01',))
caso('int x=x;', '--check', errores=('E01',))
caso('int x=1; {int x=x;}', '--check', errores=('E01',))
caso('int x=1; int x=2; print(y); print(2/(3-3));', '--check', errores=('E02','E01','E04'))
caso('int x=;', '--check', errores=('S01',))
caso('if(1) print(2);', '--check', errores=('S01',))
caso('print(1<2<3);', '--check', errores=('S01',))
caso('print(--1);', '--check', errores=('S01',))
caso('print(1!2);', '--tokens', errores=('S01',))
caso('print(2147483648);', '--check', errores=('S01',))
caso('int a=-2147483648; int b=-1; print(a/b); print(a-1);', '--run', '-2147483648\n2147483647\n')
caso('int a=-7; int b=2; print(a/b); print(a<b); print(a!=b); print(a==a); print(-a);', '--run', '-3\n1\n1\n1\n7\n')
caso('int _t0=4; int _t1=3; print(_t0+_t1);', '--run', '7\n')
caso('int x=0; while(x<3){int y=2; print(x*y); x=x+1;}', '--run', '0\n2\n4\n')
caso('print((2*3+4)/2);', '--tac', 'print 5\n')
caso('', '--ast', '(programa)\n')
caso('', '--run', '')
caso('print(x);\n'*25, '--check', errores=('E01',))

# Casos de archivo del equipo: se agregan sin modificar las pruebas doradas.
for archivo in sorted((ROOT / 'tests' / 'propias').glob('*.lj')):
    salida = archivo.with_suffix('.out')
    diagnostico = archivo.with_suffix('.err')
    if salida.exists():
        caso(archivo.read_text(encoding='utf-8'), '--run', salida.read_text(encoding='utf-8'))
    elif diagnostico.exists():
        linea, codigo = diagnostico.read_text(encoding='utf-8').split()
        caso(archivo.read_text(encoding='utf-8'), '--check', errores=(codigo,))

# Uso incorrecto debe devolver 2; ruta de archivo sin modo usa --check.
for args in ([], ['x', '--otro'], ['x', '--check', 'extra']):
    r = subprocess.run([str(BIN), *args], capture_output=True, timeout=10)
    assert r.returncode == 2
    total += 1
print(f'Propias: {total}/{total} pruebas superadas')

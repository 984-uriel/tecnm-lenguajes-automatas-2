#!/bin/sh
set -eu
cd "$(dirname "$0")"
make
make test
printf '\n=== TOKENS ===\n'
./lenguajito tests/hito1/01_expresion.lj --tokens
printf '\n=== AST ===\n'
./lenguajito tests/hito1/01_expresion.lj --ast
printf '\n=== ERROR ESPERADO E02 ===\n'
if ./lenguajito tests/invalidos/02_redeclaracion.lj --check; then
    echo 'ERROR: se acepto un programa invalido'; exit 1
else
    resultado=$?
    test "$resultado" -eq 1
fi
printf '\n=== PLEGADO DE CONSTANTES ===\n'
./lenguajito tests/tac/01_folding.lj --tac
printf '\n=== CICLOS ANIDADOS ===\n'
./lenguajito tests/validos/08_anidado.lj --run
printf '\n=== ELF Y ENSAMBLADOR ===\n'
make demo-bin
readelf -h build/anidado

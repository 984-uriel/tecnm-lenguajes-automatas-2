# Guion de presentación

Es un guion para ensayar, no una constancia de presentación realizada.

## Hitos 1–3 (3 minutos)

1. Mostrar `make test-hitoN`.
2. Hito 1: explicar los tokens y AST de los tres programas de tests/hito1.
3. Hito 2: ejecutar shadowing, mostrar E01/E02 y explicar inicialización.
4. Hito 3: explicar los dos ejemplos de optimizacion.md y el harness de DCE.
5. Cada persona explica sus archivos y una decisión que revisó personalmente.

## Hito 4 (5 minutos, Linux)

- Minuto 0–1: `make test`; señalar los resultados reales.
- Minuto 1–2: `./lenguajito tests/validos/08_anidado.lj --run`.
  Salida: 1, 2, 4, 3, 6, 9 (un número por línea).
- Minuto 2–3: `./lenguajito tests/invalidos/02_redeclaracion.lj --check`.
  Explicar E02 y el ámbito de la variable.
- Minuto 3–4: `./lenguajito tests/tac/01_folding.lj --tac`.
  Explicar por qué imprime directamente 10.
- Minuto 4–5: `make demo-bin` y `readelf -h build/anidado`.
  Mostrar prólogo, llamada printf y ret en build/anidado.s.

Preguntas para preparar: ¿cómo se conserva la precedencia?, ¿qué pasa si solo
una rama inicializa x?, ¿por qué cada variable oculta tiene otra casilla?,
¿qué operaciones conserva DCE?, ¿por qué se alinea la pila antes de printf?


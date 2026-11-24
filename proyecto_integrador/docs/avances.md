# Avances del proyecto

Equipo: Ingrid Paola Dzul Segovia, Jonathan Uriel Che Castillo y Josue Efrain Martin Pool.
Repositorio: https://github.com/josue6martin/tecnm-lenguajes-automatas-2

Registro técnico preparado el 2026-11-05 con asistencia de IA. No representa
avances ya presentados en clase. El equipo debe agregar fecha, commit y evidencia
de cada presentación real; no se han creado tags de entregas anteriores.

| Sesión | Comando que demuestra el avance | Estado / qué sigue |
| :--- | :--- | :--- |
| 05 | `make` | Estructura C11 lista; revisar gramática y reparto |
| 08 | `./lenguajito tests/validos/01_suma.lj --tokens` | Lexer listo; explicar posiciones y caracteres inválidos (revisado y verificado por Ingrid el 2026-11-09, commits `f3271b8` y `3a4eac9` — caso `prefijo_kw`) |
| 15 | `./lenguajito tests/validos/05_shadowing.lj --ast` | Parser y semántica listos; explicar ámbitos |
| 25 | `./lenguajito tests/validos/03_if.lj --tac` | TAC listo; explicar saltos y optimizaciones |
| 35 | `make demo-bin` | ELF x86-64 ejecutado en CI; preparar demo del equipo |

Qué falla o falta: ver `validacion.md`; las presentaciones en clase siguen sin
registrar. Commit de integración: `cd714e4` (implementación inicial con IA).
Commits individuales ya existentes en `main`: Ingrid `f3271b8`, `e346f39`,
`dccc14a`, `5c0d0c6`, `3a4eac9` (revisión del lexer, registro, alineación del
reporte, avances y caso de prueba `prefijo_kw`); Jonathan
`56b2f77`, `b95b1cf` (pruebas propias y registro); Josue `cd714e4`, `069a723`,
`bf172b7`, `aa315d1` (implementación, reporte y fixes). Fechas y evidencia de
cada presentación real en clase: pendientes de registrar por el equipo.


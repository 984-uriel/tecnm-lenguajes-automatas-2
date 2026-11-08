# Validación

Fecha: 2026-11-05.

- GCC MinGW 6.3: compilación C11 con -Wall -Wextra -Werror -O2, sin advertencias.
- verificar-windows.ps1: 20/20 comprobaciones contra archivos dorados.
- Harness pruebas/dce.c: elimina cadena de temporales muertos y conserva la viva.
- Linux Ubuntu 24.04 x86-64: Hito 1 4/4, Hito 2 14/14, Hito 3 15/15,
  Hito 4 16/16; suite dorada completa 20/20.
- Pruebas adicionales: 32/32 tanto en Windows como en Linux, incluyendo
  los tres archivos de tests/propias/.
- AddressSanitizer y UndefinedBehaviorSanitizer: 20/20 doradas y 32/32 propias,
  sin errores detectados.
- file y readelf confirmaron ELF64 x86-64; el binario de la demo imprimió
  1, 2, 4, 3, 6, 9 en líneas separadas.
- Evidencia: https://github.com/josue6martin/tecnm-lenguajes-automatas-2/actions/runs/42097233258
- Commit de implementación validado: cd714e459c4cb5c1ba07984c115cc17d89f23bef.
- Validación posterior de 32 casos: https://github.com/josue6martin/tecnm-lenguajes-automatas-2/actions/runs/42097565212
- main integrado y validado: https://github.com/josue6martin/tecnm-lenguajes-automatas-2/actions/runs/42099229913
- Revisión posterior local: se repitieron con éxito 20/20 doradas y 32/32 propias.
- Artefacto con ELF y ensamblador: lenguajito-demo-linux-x86-64, disponible en
  la ejecución anterior (los artefactos tienen la retención de GitHub Actions).
- Los archivos originales de tests/ se conservan; las pruebas adicionales viven
  en pruebas/ y en tests/propias/.

Reproducir en Linux: make clean, make, make test y make demo-bin.
La salida de Windows no acredita un ELF ni una demo realizada por el equipo.

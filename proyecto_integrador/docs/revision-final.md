# Revisión final de entrega

Fecha: 2026-11-05. Requisitos contrastados con PROYECTO.md y la gramática del fork.

## Parte técnica comprobada

| Requisito | Evidencia / estado |
| :--- | :--- |
| C11, libc y parser manual | Fuentes modulares en src/, compilación sin advertencias |
| Tokens y AST | Pruebas y ensayo guiado correctos |
| Ámbitos, inicialización y diagnósticos | Pruebas de E01/E02/E04/S01 y casos adicionales |
| TAC y dos optimizaciones | Plegado y harness de temporales muertos; dos ejemplos en optimizacion.md |
| Backend AT&T, as/gcc y ejecución real | CI Linux y demo de ELF64 x86-64 |
| Cuatro hitos | 4/4, 14/14, 15/15 y 16/16 en Linux |
| Suite completa | 20/20 originales y 32/32 adicionales; harness DCE aprobado |
| Memoria y comportamiento indefinido | AddressSanitizer y UndefinedBehaviorSanitizer aprobados |
| Pruebas doradas intactas | Comparación de hashes contra el estado inicial del repositorio |
| Documentación y equipo | README, diseño, bitácoras, guiones y los tres integrantes publicados |
| Publicación | Implementación integrada a main; cuatro tags existentes |
| Ensayo local | practicar-windows.ps1 ejecutado sin pausas desde la carpeta raíz, con éxito |

Evidencia Linux de la corrección más reciente:
https://github.com/josue6martin/tecnm-lenguajes-automatas-2/actions/runs/42103789812

Commit técnico auditado: bf172b70b9da405c8601f8bfad04ddd61c2ff18b.
El workflow guarda un artefacto llamado lenguajito-demo-linux-x86-64 con el ELF
y su ensamblador. El binario se regenera mediante make demo-bin.

## Tags publicados

v0.1-hito1, v0.2-hito2, v0.3-hito3 y v1.0-hito4 apuntan a
3e09970377805c528cccd4936de765530bf96d00. Todos se crearon en el cierre final;
no constituyen evidencia de avances históricos en cuatro fechas distintas.
main contiene además la corrección del directorio de trabajo del verificador
Windows. El código del compilador de los tags no cambió con esa corrección.

## Requisitos académicos aún no acreditados

- Los tres integrantes deben revisar el código y registrar qué aceptaron,
  corrigieron o rechazaron de la propuesta de IA. Las bitácoras existentes
  declaran la generación inicial y la verificación automática.
- El reparto del README es una propuesta: faltan aportaciones y commits reales
  de cada persona. No se han inventado autorías ni commits del equipo.
- Falta evidencia de avances y presentaciones en las sesiones del curso.
  Un documento preparado ahora no acredita presentaciones pasadas.
- Deben realizar las demos y poder explicar cualquier módulo del compilador.
- La demostración oficial del ELF requiere Linux; el ensayo Windows usa PE.

Conclusión: implementación técnica publicada y verificada, lista para practicar.
No se declara cumplido el 100% académico hasta que el equipo complete esas
actividades. La decisión de aceptar avances tardíos corresponde al docente.

Para ensayar: docs/exposicion-equipo.md, practicar-windows.ps1 y practicar-linux.sh.

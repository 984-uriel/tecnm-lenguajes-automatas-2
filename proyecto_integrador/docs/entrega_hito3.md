# Entrega técnica · Hito 3

Equipo: Ingrid Paola Dzul Segovia, Jonathan Uriel Che Castillo y Josue Efrain Martin Pool.
Repositorio: https://github.com/josue6martin/tecnm-lenguajes-automatas-2
Fecha de preparación: 2026-11-05.
Tag previsto: `v0.3-hito3` (pendiente de aceptación del equipo).

## Alcance y pruebas

TAC y optimización. Consultar README, diseno.md y validacion.md.
Comando oficial: `make test-hito3`. Pruebas propias: `make test-propias`.
Evidencia Linux: 15/15 en [CI](https://github.com/josue6martin/tecnm-lenguajes-automatas-2/actions/runs/42097233258),
commit cd714e459c4cb5c1ba07984c115cc17d89f23bef. No se atribuye a una
sesión anterior ni se declara una presentación realizada.

## Reparto

La tabla de README propone áreas de revisión para los tres integrantes.
La implementación inicial fue generada con IA a solicitud de Josue.
Revisión humana, commits individuales y aceptación: pendientes del equipo.

## Bitácora C-R-E-O

| Contexto | Rol | Especificación | Output y auditoría |
| :--- | :--- | :--- | :--- |
| Solicitud de completar el proyecto integrador del fork | Asistente de programación | C11, libc, parser manual, gramática original, pruebas sin cambios | Código modular, pruebas y documentación; revisión humana pendiente |
| TAC y optimización | Implementador y verificador automático | Respetar interfaces, modos y códigos del curso | Compilación sin warnings y pruebas Windows descritas en validacion.md; verificar CI Linux |

Correcciones detectadas durante la implementación asistida: el modo exclusivo
`fopen("wx")` no funciona en el CRT antiguo de MinGW; se aisló una compatibilidad
local. Se corrigió la validación de literales positivos fuera de 32 bits.
Se preservó el contenido exacto de las pruebas descargadas. Se agregó análisis
conservador de asignación definida y una prueba específica de dependencias DCE.
Estas son correcciones del asistente; no una auditoría atribuida a los alumnos.

## Cierre por el equipo

- [ ] Leer y aceptar o corregir la implementación, anotando decisiones propias.
- [x] Ejecutar el comando oficial en CI y adjuntar el enlace a su salida.
- [ ] Añadir aportaciones individuales verificables.
- [ ] Registrar avance real, commit y fecha.
- [ ] Ensayar la demo de 3 minutos.
- [ ] Crear y subir el tag cuando la entrega esté aceptada.


# Bitácora C R E O del Hito 1

Preparación: 6 de noviembre de 2026. Asistencia: Codex. La auditoría humana se registra después de revisar y ejecutar, no se da por hecha.

## Solicitud de esta preparación

- **Contexto:** existe una implementación de Lenguajito con etapas posteriores. El usuario indicó que su profesor solo requiere el Hito 1 y pidió reporte, pruebas y los componentes de la entrega.
- **Rol:** asistente para revisar los requisitos locales, organizar la entrega y preparar documentación y verificación.
- **Especificación:** cumplir `PROYECTO.md`, Hito 1; conservar C11, parser manual y pruebas doradas sin modificaciones; demostrar tokens, tres AST y S01/E01; documentar límites y resultados reales.
- **Output:** README centrado en el Hito 1, reporte, ocho pruebas propias, verificador de PowerShell, ejecutor Python de pruebas propias, objetivo Make adicional, guion de tres minutos y lista de cierre.

## Decisiones de la preparación asistida

Se conservaron los módulos existentes: eliminarlos exigiría cambiar la integración y no es necesario para evaluar este hito. No se modificaron `src/` ni `tests/`. Se separaron pruebas oficiales y propias. Se evitó presentar los 20/20 de una captura previa como si fueran la salida del nuevo verificador. No se copiaron afirmaciones previas de CI como evidencia actual, pues no se han verificado en esta preparación.

La implementación C preexistente también se había preparado con asistencia de IA, según los documentos locales. Este registro no atribuye esa implementación a los integrantes.

## Resultado y auditoría disponibles

La captura proporcionada por el usuario confirma GCC 6.3.0 y 20/20 de la verificación general en su PowerShell. La sesión automática no logró iniciar los ejecutables MinGW de 32 bits por un error de carga de DLL. Posteriormente, el usuario ejecutó el verificador específico en PowerShell y aportó la captura de 4/4 oficiales y 8/8 propias superadas, más siete capturas de los resultados individuales. Las ocho se incluyen en el reporte. Después, el usuario ejecutó `make` y `make test-hito1` en GitHub Codespaces. La novena captura documenta 4/4 pruebas oficiales superadas en Linux y se incorporó al reporte.

## Registro que completa cada integrante

Para cada revisión, registrar nombre, fecha, archivo o función revisada, explicación propia, decisión de aceptar o corregir, motivo, cambio realizado, comando ejecutado, resultado y commit real. Si no se rechazó ninguna propuesta de IA, indicarlo con la razón; no inventar rechazos ni correcciones.

Estado al preparar el paquete: revisión humana y aportaciones individuales pendientes de documentar.


## Auditoría técnica de la integración

Se revisó el commit `069a723b7523d2d6ea0be812f22d555e7c5aae42`. GitHub Actions terminó con éxito e incluyó 4/4 oficiales, 8/8 propias del Hito 1 y 20/20 generales. Los 11 archivos de fuentes y los 46 archivos existentes de pruebas se compararon con GitHub sin diferencias. Se corrigieron enlaces ausentes del paquete, instrucciones de cierre desactualizadas y el reparto propuesto del README para que coincida con el guion del Hito 1. Esta auditoría técnica del asistente no se atribuye como revisión personal de los integrantes.

# 1. Crear el archivo de presentación de Sesión 02
mkdir -p presentaciones laboratorios
cat << 'EOF' > presentaciones/sesion02.html
<!doctype html>
<html lang="es">
<head>
  <meta charset="utf-8">
  <title>Sesión 02: Taller de Fork, Compilación en C y Control con Git | Lenguajes y Autómatas II</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/reveal.js@4.5.0/dist/reveal.css">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/reveal.js@4.5.0/dist/theme/black.css">
</head>
<body>
  <div class="reveal">
    <div class="slides">
      <section>
        <h2>Sesión 02</h2>
        <h3>Taller de Compilación con GCC y Flujo de Trabajo en Git</h3>
        <p><strong>Lenguajes y Autómatas II (SCD-1016)</strong></p>
        <p>Prof. Iván Márquez Larios</p>
      </section>

      <section>
        <h3>El Taller del Artesano: Compilación Nativa</h3>
        <ul>
          <li>Un compilador toma código fuente (texto) y produce código máquina (binario).</li>
          <li>Herramienta estándar en Linux: <code>gcc</code> (GNU Compiler Collection).</li>
          <li><strong>Regla técnica:</strong> A Git solo subimos código fuente (<code>.c</code>, <code>.h</code>, <code>Makefile</code>), <em>nunca</em> binarios ejecutables.</li>
        </ul>
      </section>

      <section>
        <h3>Ciclo de Compilación y Ejecución</h3>
        <pre><code class="bash">
# 1. Compilar con advertencias estrictas
gcc -Wall -Wextra scanner_base.c -o scanner_base

# 2. Ejecutar el binario generado
./scanner_base
        </code></pre>
      </section>

      <section>
        <h3>El Flujo Sagrado de Git</h3>
        <ol>
          <li><code>echo "scanner_base" &gt;&gt; .gitignore</code> (Evitar subir ejecutables).</li>
          <li><code>git status</code> (Inspeccionar cambios).</li>
          <li><code>git add scanner_base.c .gitignore</code> (Preparar fuentes).</li>
          <li><code>git commit -m "feat: ..."</code> (Registrar avance).</li>
          <li><code>git push origin main</code> (Sincronizar a GitHub).</li>
        </ol>
      </section>
    </div>
  </div>
  <script src="https://cdn.jsdelivr.net/npm/reveal.js@4.5.0/dist/reveal.js"></script>
  <script>Reveal.initialize();</script>
</body>
</html>
EOF

# 2. Crear la guía del laboratorio de compilación
cat << 'EOF' > laboratorios/lab01-compilacion-git.md
# Laboratorio 01: Compilación en C con GCC y Gestión de Versiones con Git

**Asignatura:** Lenguajes y Autómatas II (SCD-1016)  
**Profesor:** M. en C. Iván José Márquez Larios

---

## Objetivo
Escribir un programa básico en lenguaje C que simule el reconocimiento inicial de tokens, compilarlo en Linux mediante GCC con directivas de advertencia (`-Wall -Wextra`), configurar el archivo `.gitignore` para descartar binarios y sincronizar los cambios en GitHub.

---

## Instrucciones Paso a Paso

### Paso 1: Crear el código fuente
Crea un archivo llamado `token_scanner.c`:

```c
#include <stdio.h>

typedef enum {
    TOKEN_ID,
    TOKEN_ASIGNACION,
    TOKEN_NUMERO,
    TOKEN_FIN
} TipoToken;

void imprimir_token(TipoToken tipo, const char* lexema) {
    const char* nombres[] = {"ID", "ASIGNACION", "NUMERO", "FIN"};
    printf("[Token Encontrado] Tipo: %-12s | Lexema: '%s'\n", nombres[tipo], lexema);
}

int main(void) {
    printf("=== Simulador de Entrada Léxica ===\n");
    imprimir_token(TOKEN_ID, "contador");
    imprimir_token(TOKEN_ASIGNACION, "=");
    imprimir_token(TOKEN_NUMERO, "100");
    imprimir_token(TOKEN_FIN, ";");
    return 0;
}

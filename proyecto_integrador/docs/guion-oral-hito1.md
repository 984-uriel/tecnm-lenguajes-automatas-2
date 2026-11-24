# Guion oral · Hito 1 — para leer en voz alta

Texto completo de la demo en común (3 minutos). Entre paréntesis van las acciones.
Si alguien se queda sin ideas, usa una **frase de rescate** (sección final) y
continúa; nunca guardar silencio. Ensayar con `practicar-hito1-windows.ps1`.

---

## Apertura — quien arranque (15 segundos)

> "Hola, somos el equipo: Ingrid Dzul, Jonathan Che y Josue Martin. Nuestro
> proyecto es **Lenguajito**, un compilador escrito en C para un lenguaje mínimo
> de variables enteras. En este primer hito implementamos el **análisis léxico**,
> el **análisis sintáctico** y el **árbol sintáctico**. Vamos a mostrarlo por
> etapas."

---

## Ingrid — Lexer (55 segundos)

(Ejecutar:)
```powershell
.\lenguajito.exe tests\hito1\01_expresion.lj --tokens
```

> "La primera etapa es el lexer, en el archivo `lexer.c`. Recibe el código fuente
> y lo divide en **tokens**, las unidades básicas del lenguaje. Cada token se
> imprime con su posición: **línea, columna, categoría y lexema**. Por ejemplo,
> `1:1 KW int` quiere decir: línea uno, columna uno, palabra reservada, lexema
> `int`. Las categorías son `KW` para palabras reservadas, `ID` para
> identificadores, `NUM` para números, `OP` para operadores, `DELIM` para
> delimitadores, y `EOF` para el fin de archivo, que sale **una sola vez al
> final**. Los espacios y los comentarios con dos rayitas no generan tokens. Y
> cualquier carácter que no pertenezca al lenguaje produce un error **S01** con su
> posición exacta. Por ejemplo, miren este caso:"

(Ejecutar:)
```powershell
.\lenguajito.exe pruebas\hito1\caracter_invalido.lj --check
```

> "Ahí está el S01 con su línea y columna. Pasamos al parser."

---

## Jonathan — Parser y AST (60 segundos)

(Ejecutar:)
```powershell
.\lenguajito.exe tests\hito1\01_expresion.lj --ast
```

> "La segunda etapa es el parser, en `parser.c`, que construye el **AST** en
> `ast.c`. Se llama **descendente** porque empieza en la regla `programa` de la
> gramática y va bajando hasta las expresiones; **recursivo** porque cada función
> llama a la siguiente y, al encontrar un paréntesis, `primaria` vuelve a llamar a
> `expresión`; y **a mano** porque está escrito función por función en C, sin
> `flex` ni `bison`, que no están permitidos en el proyecto.
>
> La precedencia está en la gramática. Miren: `int x = 1 + 2 * 3` produce un nodo
> **suma** con la **multiplicación** como hijo derecho, o sea, primero se resuelve
> `2 * 3`. Si la precedencia estuviera mal, la multiplicación quedaría arriba de
> la suma."

(Ejecutar:)
```powershell
.\lenguajito.exe tests\hito1\02_asignacion.lj --ast
```

> "Con paréntesis cambia la estructura: ahora la suma es hijo izquierdo de la
> multiplicación. También tenemos pruebas propias: el `-5` queda como `neg`, y en
> `1 + 2 < 4` la suma se resuelve antes que la comparación, como pide la
> gramática."

(Ejecutar, opcional:)
```powershell
.\lenguajito.exe pruebas\hito1\unario.lj --ast
```

---

## Josue — Integración y pruebas (50 segundos)

(Ejecutar:)
```powershell
.\lenguajito.exe tests\hito1\04_invalida.lj --check
.\lenguajito.exe pruebas\hito1\no_declarada.lj --check
```

> "La tercera etapa es la integración: `compilador.c` conecta el lexer, el parser
> y la semántica, y reparte cada modo de la línea de comandos. El modo `--check`
> valida el programa y reporta errores con el formato exacto: **archivo, línea,
> columna, error y código**. El primero es un **S01**: falta la expresión después
> del signo igual. El segundo es un **E01**: variable no declarada. Los dos
> terminan con código de salida uno; un programa válido termina en cero.
>
> Las cuatro pruebas oficiales pasan, más ocho propias, y el CI de GitHub Actions
> queda en verde. Las pruebas doradas del curso **no se modificaron**."

(Ejecutar para el verde:)
```powershell
.\verificar-hito1-windows.ps1
```

---

## Cierre — cualquiera (15 segundos)

> "Eso es el primer hito de Lenguajito: **léxico, sintaxis y AST**. Lo que sigue
> en el hito dos es la gramática completa con `if`, `while` y bloques, más la
> tabla de símbolos y los ámbitos. Gracias; preguntas."

---

## Frases de rescate (si se te van las ideas)

- **Para ganar tiempo:** "Les muestro la salida para que se vea mejor" → *ejecuta
  el comando y mientras corre, recuerda tu siguiente línea.*
- **Si olvidaste el qué:** "Esto está en el archivo `___`, que se encarga de
  `___`" (lexer = dividir en tokens; parser = organizar según la gramática; ast =
  imprimir el árbol; compilador = conectar todo).
- **Si olvidaste el porqué:** "La regla del proyecto dice `___`, y por eso lo
  hicimos así" (parser manual, solo libc, `tests/` intactos, C11 con gcc).
- **Si te preguntan algo que no sabes:** "Buena pregunta; está documentado en
  `docs/diseno.md` y `docs/gramatica.md`, lo revisamos y te lo confirmamos." (Nunca
  inventar; después se responde con el documento a la mano.)
- **Transiciones seguras:** "Ahora Ingrid les muestra el lexer" · "Continuamos con
  el parser" · "Para cerrar, las pruebas".

## Antes de salir de la sala

1. Terminal abierta en `proyecto_integrador/` y `verificar-hito1-windows.ps1` en verde.
2. PDF del reporte y Actions verde listos en el celular (plan B).
3. Los tres ensayaron su parte una vez en voz alta, de pie.

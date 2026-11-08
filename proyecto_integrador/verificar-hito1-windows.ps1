param([switch]$SoloCompilar)
$ErrorActionPreference = 'Stop'
Push-Location $PSScriptRoot
try {
    if (-not (Get-Command gcc -ErrorAction SilentlyContinue)) {
        if (Test-Path 'C:\MinGW\bin\gcc.exe') { $env:PATH = 'C:\MinGW\bin;' + $env:PATH }
        else { throw 'No se encontro GCC. Agrega la carpeta bin de MinGW a PATH.' }
    }
    $sources = @(Get-ChildItem src\*.c | Select-Object -ExpandProperty FullName)
    & gcc -std=c11 -Wall -Wextra -Werror -O2 -o lenguajito.exe $sources
    if ($LASTEXITCODE -ne 0) { throw 'Fallo la compilacion.' }
    if ($SoloCompilar) { return }

    function Ejecutar([string]$ruta, [string]$modo) {
        $p = New-Object System.Diagnostics.Process
        $p.StartInfo.FileName = Join-Path $PSScriptRoot 'lenguajito.exe'
        $p.StartInfo.WorkingDirectory = $PSScriptRoot
        $p.StartInfo.Arguments = '"' + $ruta + '" ' + $modo
        $p.StartInfo.UseShellExecute = $false
        $p.StartInfo.RedirectStandardOutput = $true
        $p.StartInfo.RedirectStandardError = $true
        try {
            $null = $p.Start()
            $salida = $p.StandardOutput.ReadToEnd().Replace("`r`n", "`n")
            $errorTexto = $p.StandardError.ReadToEnd()
            $p.WaitForExit()
            return @{ Codigo = $p.ExitCode; Salida = $salida; Error = $errorTexto }
        } finally { $p.Dispose() }
    }
    function Igual([string]$nombre, [string]$archivo, [string]$modo, [string]$esperado) {
        $r = Ejecutar $archivo $modo
        if ($r.Codigo -ne 0 -or $r.Salida.TrimEnd() -cne $esperado.Replace("`r`n", "`n").TrimEnd()) {
            throw "$nombre fallo. Codigo: $($r.Codigo). Salida: $($r.Salida) $($r.Error)"
        }
        Write-Output "OK $nombre"
    }
    function ErrorEsperado([string]$nombre, [string]$archivo, [int]$linea, [int]$columna, [string]$codigo) {
        $r = Ejecutar $archivo '--check'
        $patron = '(?m)^' + [regex]::Escape($archivo) + ':' + $linea + ':' + $columna + ': error ' + $codigo + ':'
        if ($r.Codigo -ne 1 -or $r.Salida -notmatch $patron) {
            throw "$nombre fallo. Codigo: $($r.Codigo). Salida: $($r.Salida) $($r.Error)"
        }
        Write-Output "OK $nombre"
    }

    $oficiales = 0
    foreach ($f in Get-ChildItem tests\hito1\*.ast) {
        Igual "oficial AST $($f.BaseName)" ($f.FullName -replace '\.ast$', '.lj') '--ast' ([IO.File]::ReadAllText($f.FullName))
        $oficiales++
    }
    foreach ($f in Get-ChildItem tests\hito1\*.err) {
        $partes = ([IO.File]::ReadAllText($f.FullName)).Trim() -split '\s+'
        $archivo = $f.FullName -replace '\.err$', '.lj'
        $r = Ejecutar $archivo '--check'
        $patron = '(?m)^' + [regex]::Escape($archivo) + ':' + $partes[0] + ':\d+: error ' + $partes[1] + ':'
        if ($r.Codigo -ne 1 -or $r.Salida -notmatch $patron) { throw "Fallo oficial $($f.Name): $($r.Salida) $($r.Error)" }
        Write-Output "OK oficial diagnostico $($f.BaseName)"
        $oficiales++
    }
    if ($oficiales -ne 4) { throw "Se esperaban 4 pruebas oficiales, se encontraron $oficiales." }

    $base = 'pruebas\hito1'
    Igual 'tokens con posiciones comentario identificador y EOF' "$base\tokens.lj" '--tokens' ([IO.File]::ReadAllText((Join-Path $PSScriptRoot "$base\tokens.tokens")))
    Igual 'programa valido con declaracion y asignacion' "$base\valido.lj" '--check' ''
    ErrorEsperado 'variable no declarada' "$base\no_declarada.lj" 1 9 'E01'
    ErrorEsperado 'lectura antes de asignar' "$base\sin_valor.lj" 2 9 'E01'
    ErrorEsperado 'caracter invalido' "$base\caracter_invalido.lj" 1 9 'S01'
    ErrorEsperado 'punto y coma faltante' "$base\sin_punto_coma.lj" 2 1 'S01'
    foreach ($nombre in @('asociatividad', 'parentesis')) {
        Igual $nombre "$base\$nombre.lj" '--ast' ([IO.File]::ReadAllText((Join-Path $PSScriptRoot "$base\$nombre.ast")))
    }
    Write-Output 'Hito 1 Windows: 4/4 pruebas oficiales y 8/8 pruebas propias superadas.'
    Write-Output 'Para la evidencia oficial del curso ejecuta tambien make test-hito1 en Linux.'
} finally { Pop-Location }

param([switch]$SinPausas)
$ErrorActionPreference = 'Stop'
Push-Location $PSScriptRoot
try {
    $env:PATH = 'C:\MinGW\bin;' + $env:PATH
    & .\verificar-windows.ps1
    function Paso($titulo, $archivo, $modo, $codigo = 0) {
        Write-Host "`n=== $titulo ===" -ForegroundColor Cyan
        Write-Host ".\lenguajito.exe $archivo $modo"
        if (-not $SinPausas) { $null = Read-Host 'Explica el paso y pulsa Enter para ejecutarlo' }
        & .\lenguajito.exe $archivo $modo
        if ($LASTEXITCODE -ne $codigo) { throw "Codigo de salida inesperado en $titulo" }
    }
    Paso '1. Tokens: linea, columna, categoria y lexema' 'tests\hito1\01_expresion.lj' '--tokens'
    Paso '2. AST: la multiplicacion tiene precedencia' 'tests\hito1\01_expresion.lj' '--ast'
    Paso '3. Semantica: E02 es un error esperado de redeclaracion' 'tests\invalidos\02_redeclaracion.lj' '--check' 1
    Paso '4. Optimizacion: print 10 sin operaciones constantes' 'tests\tac\01_folding.lj' '--tac'
    Paso '5. Ejecucion: ciclos anidados; salida 1,2,4,3,6,9' 'tests\validos\08_anidado.lj' '--run'
    Write-Host "`nEnsayo terminado. Windows ejecuto un PE; para demostrar ELF usa practicar-linux.sh en Linux."
} finally { Pop-Location }

param([switch]$SinPausas)
$ErrorActionPreference = 'Stop'
Push-Location $PSScriptRoot
try {
    & .\verificar-hito1-windows.ps1
    function Paso($titulo, $archivo, $modo, $esperado = 0) {
        Write-Host "`n$titulo" -ForegroundColor Cyan
        Write-Host ".\lenguajito.exe $archivo $modo"
        if (-not $SinPausas) { $null = Read-Host 'Pulsa Enter para continuar' }
        & .\lenguajito.exe $archivo $modo
        if ($LASTEXITCODE -ne $esperado) { throw "Codigo inesperado en $titulo" }
    }
    Paso 'Tokens con linea y columna' 'tests\hito1\01_expresion.lj' '--tokens'
    Paso 'AST 1 precedencia de multiplicacion' 'tests\hito1\01_expresion.lj' '--ast'
    Paso 'AST 2 parentesis y asignacion' 'tests\hito1\02_asignacion.lj' '--ast'
    Paso 'AST 3 declaracion sin inicializador' 'tests\hito1\03_sin_init.lj' '--ast'
    Paso 'S01 expresion incompleta' 'tests\hito1\04_invalida.lj' '--check' 1
    Paso 'E01 variable no declarada' 'pruebas\hito1\no_declarada.lj' '--check' 1
    Write-Host 'Practica del Hito 1 terminada.'
} finally { Pop-Location }

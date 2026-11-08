param([switch]$SoloCompilar)
$ErrorActionPreference = 'Stop'
Push-Location $PSScriptRoot
try {
    if (-not (Get-Command gcc -ErrorAction SilentlyContinue)) {
        if (Test-Path 'C:\MinGW\bin\gcc.exe') { $env:PATH = 'C:\MinGW\bin;' + $env:PATH }
        else { throw 'Agrega GCC y GNU as a PATH.' }
    }
    $sources = Get-ChildItem src\*.c | Select-Object -ExpandProperty FullName
    & gcc -std=c11 -Wall -Wextra -Werror -O2 -o lenguajito.exe $sources
    if ($LASTEXITCODE -ne 0) { throw 'Fallo la compilacion' }
    if ($SoloCompilar) { return }
    $total = 0
    function Ejecutar($ruta, $modo) {
        $p = New-Object System.Diagnostics.Process
        $p.StartInfo.FileName = Join-Path $PSScriptRoot 'lenguajito.exe'
        $p.StartInfo.WorkingDirectory = $PSScriptRoot
        $p.StartInfo.Arguments = '"' + $ruta + '" ' + $modo
        $p.StartInfo.UseShellExecute = $false
        $p.StartInfo.RedirectStandardOutput = $true
        $p.StartInfo.RedirectStandardError = $true
        $null = $p.Start()
        $stdout = $p.StandardOutput.ReadToEnd().Replace("`r`n", "`n")
        $stderr = $p.StandardError.ReadToEnd()
        $p.WaitForExit()
        return @{ Codigo = $p.ExitCode; Salida = $stdout; Error = $stderr }
    }
    foreach ($file in Get-ChildItem tests\hito1\*.ast) {
        $r = Ejecutar ($file.FullName -replace '\.ast$', '.lj') '--ast'
        if ($r.Codigo -ne 0 -or $r.Salida.Trim() -cne ([IO.File]::ReadAllText($file.FullName)).Trim()) { throw "AST incorrecto: $file" }
        $total++
    }
    foreach ($file in Get-ChildItem tests\validos\*.lj) {
        $r = Ejecutar $file.FullName '--run'
        $expected = [IO.File]::ReadAllText(($file.FullName -replace '\.lj$', '.out'))
        if ($r.Codigo -ne 0 -or $r.Salida -cne $expected) { throw "Ejecucion incorrecta: $file : $($r.Salida) $($r.Error)" }
        $total++
    }
    $invalidos = @(Get-ChildItem tests\invalidos\*.err) + @(Get-ChildItem tests\hito1\*.err)
    foreach ($file in $invalidos) {
        $partes = ([IO.File]::ReadAllText($file.FullName)).Trim() -split '\s+'
        $r = Ejecutar ($file.FullName -replace '\.err$', '.lj') '--check'
        if ($r.Codigo -ne 1 -or $r.Salida -notmatch (':' + $partes[0] + ':\d+: error ' + $partes[1] + ':')) { throw "Diagnostico incorrecto: $file" }
        $total++
    }
    $r = Ejecutar 'tests\tac\01_folding.lj' '--tac'
    if ($r.Codigo -ne 0) { throw "No se pudo comprobar TAC: $($r.Salida) $($r.Error)" }
    if ($r.Salida -match '=\s*\d+\s*[-+*/]\s*\d+') { throw 'Fallo el plegado de constantes' }
    $total++
    $r = Ejecutar 'tests\validos\01_suma.lj' '--asm'
    if ($r.Codigo -ne 0 -or $r.Salida -notmatch '\.globl main' -or $r.Salida -notmatch '\bret\b') { throw 'Ensamblador incompleto' }
    $total++
    Write-Output "Windows: $total/$total pruebas sobre los archivos dorados superadas."
    Write-Output 'La validacion ELF debe ejecutarse en Linux con make test.'
} finally { Pop-Location }


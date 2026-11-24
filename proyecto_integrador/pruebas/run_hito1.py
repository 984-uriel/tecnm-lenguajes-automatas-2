"""Pruebas propias del Hito 1. No cambia las pruebas doradas."""
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent
BASE = ROOT / "pruebas" / "hito1"
BIN = ROOT / ("lenguajito.exe" if sys.platform == "win32" else "lenguajito")


def main():
    if not BIN.is_file():
        print("No existe el compilador. Ejecuta make primero.")
        return 1
    cases = [
        ("tokens", "--tokens", 0, "tokens.tokens"),
        ("prefijo_kw", "--tokens", 0, "prefijo_kw.tokens"),
        ("valido", "--check", 0, ""),
        ("no_declarada", "--check", 1, (1, 9, "E01")),
        ("sin_valor", "--check", 1, (2, 9, "E01")),
        ("caracter_invalido", "--check", 1, (1, 9, "S01")),
        ("sin_punto_coma", "--check", 1, (2, 1, "S01")),
        ("asociatividad", "--ast", 0, "asociatividad.ast"),
        ("parentesis", "--ast", 0, "parentesis.ast"),
        ("unario", "--ast", 0, "unario.ast"),
        ("comparacion", "--ast", 0, "comparacion.ast"),
    ]
    failures = 0
    for name, mode, code, expected in cases:
        path = BASE / (name + ".lj")
        result = subprocess.run([str(BIN), str(path), mode], capture_output=True, text=True)
        ok = result.returncode == code
        if isinstance(expected, tuple):
            line, col, diagnostic = expected
            pattern = rf"^{re.escape(str(path))}:{line}:{col}: error {diagnostic}:"
            ok = ok and re.search(pattern, result.stdout, re.MULTILINE) is not None
        else:
            text = (BASE / expected).read_text(encoding="utf-8") if expected else ""
            ok = ok and result.stdout.rstrip() == text.rstrip()
        print(("OK " if ok else "FALLO ") + name)
        if not ok:
            failures += 1
            print(f"Codigo {result.returncode}\n{result.stdout}\n{result.stderr}")
    print(f"Hito 1 pruebas propias: {len(cases) - failures}/{len(cases)} superadas")
    return int(failures != 0)


if __name__ == "__main__":
    raise SystemExit(main())

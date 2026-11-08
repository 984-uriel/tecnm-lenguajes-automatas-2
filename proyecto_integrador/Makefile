CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Werror -O2
SRC     := $(wildcard src/*.c)
HDR     := $(wildcard src/*.h)
BIN     := lenguajito

.PHONY: all test test-hito1 test-hito2 test-hito3 test-hito4 test-propias demo-bin clean

all: $(BIN)

$(BIN): $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $@ $(SRC)

test: $(BIN)
	python3 tests/run.py
	$(MAKE) test-propias
	$(MAKE) test-hito1-propias

test-propias: $(BIN)
	python3 pruebas/run.py
	mkdir -p build
	$(CC) $(CFLAGS) -Isrc -o build/test-dce pruebas/dce.c src/tac.c src/util.c
	./build/test-dce

demo-bin: $(BIN)
	mkdir -p build
	./$(BIN) tests/validos/08_anidado.lj --asm > build/anidado.s
	as build/anidado.s -o build/anidado.o
	$(CC) build/anidado.o -o build/anidado
	file build/anidado
	./build/anidado

test-hito1: $(BIN)
	python3 tests/run.py --hito1

test-hito2: $(BIN)
	python3 tests/run.py --hito2

test-hito3: $(BIN)
	python3 tests/run.py --hito3

test-hito4: $(BIN)
	python3 tests/run.py --hito4

clean:
	rm -f $(BIN)
	rm -rf build



.PHONY: test-hito1-propias
test-hito1-propias: $(BIN)
	python3 pruebas/run_hito1.py

# MLScript

## Wymagania
 1. **Java Runtime Environment (JRE)** - wymagane, aby uruchomić ANTLR
 2. **Kompilator C++** - musi wspierać **C++ 17**
 3. **CMake (v3.10+)**
 4. **ANTLR4 C++ Runtime**

## Struktura projektu
 - `src/` - współdzielona logika C++
 - `generated/` - zawiera pliki C++ wygenerowane przez ANTLR
 - `MLScript.g4` - gramatyka języka
 - `examples/` - przykładowe skrypty `.mls`

## Przygotowanie środowiska
 1. Generowanie plików lexera i parsera
    ```bash
    antlr4 -Dlanguage=Cpp -visitor -no-listener -o generated MLScript.g4
    ```
 2. Konfiguracja VS Code (opcjonalne)
 Żeby zapobiec `include error` należy dodać do `.vscode/c_cpp_properties.json` do sekcji `includePath` nastepujące katalogi:
 - `${workspaceFolder}/src/**`
 - `${workspaceFolder}/generated`
 - `/usr/local/include/antlr4-runtime`
 - `/usr/local/include`
 - `/usr/include`

## Budowanie kompilatora
 1. Utworzenie katalogu `build`:
    ```bash
    mkdir build && cd build
    ```
 2. Uruchomienie CMake:
    ```bash
    cmake ..
    ```
 3. Kompilacja
    ```bash
    make
    ```
Plik wykonywalny `mlscript` zostanie utworzony w katalogu `build`.

## Uruchomienie skryptu `.mls`
Aby transpilować plik MLScript do Pythona należy podać ścieżkę do pliku jako argument:
```bash
./build/mlscript examples/load_csv.mls
```
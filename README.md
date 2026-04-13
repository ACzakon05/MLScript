# MLScript

## Temat projektu
Interpreter języka dziedzinowego do automatycznego wykonywania zadań uczenia maszynowego.

## Autorzy
- Adam Czakon adamczakon@student.agh.edu.pl
- Kacper Filip kacperfilip@student.agh.edu.pl

## Opis projektu
MLScript to interpreter języka dziedzinowego zaprojektowanego do opisu i automatycznego wykonywania zadań uczenia maszynowego w prosty i czytelny sposób. Użytkownik definiuje dane oraz cel analizy za pomocą krótkich komend tekstowych, bez konieczności pisania kodu w językach ogólnego przeznaczenia. System analizuje te informacje i na ich podstawie dobiera odpowiedni model, obsługując zarówno regresję, klasyfikację, jak i klasteryzację. Projekt wspiera dwa tryby działania: automatyczny, w którym model jest wybierany samodzielnie, oraz manualny, umożliwiający użytkownikowi pełną kontrolę nad wyborem algorytmu i jego parametrów. Architektura systemu jest hybrydowa – parser i interpreter są zaimplementowane w C++, natomiast obliczenia związane z uczeniem maszynowym realizowane są w Pythonie z wykorzystaniem gotowych bibliotek. Wyniki działania modeli, takie jak dokładność czy błąd, są prezentowane użytkownikowi bezpośrednio w konsoli.

### Rodzaj translatora:
Kompilator źródło-źródło (transpilator) o architekturze hybrydowej. Program analizuje kod napisany w języku MLScript i tłumaczy go na wykonywalny skrypt w języku Python.

### Planowany wynik działania programu:
Kompilator (transpilator) języka MLScript generuje kod wynikowy w języku Python, korzystający z gotowych bibliotek (takich jak `pandas` czy `scikit-learn`).

### Planowany język implementacji:
* **C++ (wersja 17):** implementacja logiki skanera, parsera oraz generatora kodu (transpilatora).
* **Python:** docelowy język wykonywania obliczeń oraz operacji uczenia maszynowego.
* 
### Sposób realizacji skanera/parsera:
Skaner oraz parser są generowane automatycznie z definicji gramatyki (plik `MLScript.g4`) przy użyciu generatora parserów **ANTLR4** (docelowy język: C++). Do przetwarzania wygenerowanego drzewa składniowego wykorzystany jest wzorzec wizytatora (`MLScriptBaseVisitor`).

## Wymagania
### Funkcjonalne
- Parser i interpreter komend MLScript  
- Tryb auto (automatyczny wybór modelu ML)  
- Tryb manual (ręczny wybór modelu i parametrów)  
- Generowanie raportów wyników i wizualizacji  
- Obsługa błędów składniowych  

### Niefunkcjonalne
- C++ (parser/interpreter) + Python (ML)  
- Modularna architektura  
- Czytelny DSL  
- Rozszerzalność  
- Dokumentacja i przykłady użycia  



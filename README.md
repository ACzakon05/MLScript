# MLScript

## Temat projektu
Interpreter języka dziedzinowego do automatycznego wykonywania zadań uczenia maszynowego.

## Autorzy
- Adam Czakon adamczakon@student.agh.edu.pl
- Kacper Filip kacperfilip@student.agh.edu.pl

## Opis projektu
MLScript to interpreter języka dziedzinowego zaprojektowanego do opisu i automatycznego wykonywania zadań uczenia maszynowego w prosty i czytelny sposób. Użytkownik definiuje dane oraz cel analizy za pomocą krótkich komend tekstowych, bez konieczności pisania kodu w językach ogólnego przeznaczenia. System analizuje te informacje i na ich podstawie dobiera odpowiedni model, obsługując zarówno regresję, klasyfikację, jak i klasteryzację. Projekt wspiera dwa tryby działania: automatyczny, w którym model jest wybierany samodzielnie, oraz manualny, umożliwiający użytkownikowi pełną kontrolę nad wyborem algorytmu i jego parametrów. Architektura systemu jest hybrydowa – parser i interpreter są zaimplementowane w C++, natomiast obliczenia związane z uczeniem maszynowym realizowane są w Pythonie z wykorzystaniem gotowych bibliotek. Wyniki działania modeli, takie jak dokładność czy błąd, są prezentowane użytkownikowi bezpośrednio w konsoli.

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

## Parser/Skaner
Parser oraz skaner generowane są przy użyciu narzędzia **ANTLR4**.



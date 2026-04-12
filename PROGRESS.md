# Funkcjonalności języka

## LOAD - ładowanie danych

* [x] komenda `LOAD` wspiera pliki `.csv` wraz z opcjami
* [x] komenda `LOAD` wspiera pliki `.json` wraz z opcjami
* [x] komenda `LOAD` wspiera ładowanie wybranej liczby wierszy
* [ ] obsługa błędów dla komendy `LOAD`

## SHOW - wyświetlanie danych

* [x] komenda `SHOW` wspiera wyświetlanie nazw kolumn
* [x] komenda `SHOW` wspiera wyświetlanie liczby kolumn/wierszy
* [x] komenda `SHOW` wspiera wyświetlanie wybranych kolumn
* [x] komenda `SHOW` wspiera wyświetlanie wybranych wierszy
* [x] komenda `SHOW` wspiera wyświetlanie całego datasetu
* [x] komenda `SHOW` wspiera wyświetlanie kilku kolumn datasetu
* [ ] komenda `SHOW` wspiera filtrowanie klauzulą `WHERE`
* [ ] obłsuga błędów dla komendy `SHOW`

## SET TARGET - wybór kolumny target

* [x] komenda `SET TARGET` wspiera przypisanie kolumny target
* [ ] obsługa błędów, np. nieistniejąca kolumna

## SPLIT - podział na zbiór treningowy i testowy

* [x] komenda `SPLIT` wspiera ratio, seed i shuffle
* [ ] obsługa błędów, np. błędny ratio, brak target lub datasetu

## WHERE - deklaracje warunkowe

* [x] wspiera zagnieżdżanie warunków nawiasami
* [x] wspiera negację warunków operatorem NOT
* [x] wspiera łączenie warunków operatorami logicznymi AND i OR
* [x] wspiera relacje na warunkach (`=`, `!=`, `>`, `>=`, `<`, `<=`)
* [ ] prawidłowe przechwytywanie sytuacji gdy w warunku jest nazwa kolumny a kiedy string

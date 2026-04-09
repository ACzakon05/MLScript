# Funkcjonalności języka

## LOAD - ładowanie danych

* [x] komenda `LOAD` wspiera pliki `.csv`, `.sql`, `.json`, `.pkl`, `.html`
* [x] komenda `LOAD` wspiera ładowanie wybranej liczby wierszy
* [ ] obsługa błędów dla komendy `LOAD`

## SHOW - wyświetlanie danych

* [x] komenda `SHOW` wspiera wyświetlanie nazw kolumn
* [x] komenda `SHOW` wspiera wyświetlanie liczby kolumn/wierszy
* [x] komenda `SHOW` wspiera wyświetlanie wybranych kolumn
* [x] komenda `SHOW` wspiera wyświetlanie wybranych wierszy
* [x] komenda `SHOW` wspiera wyświetlanie całego datasetu
* [x] komenda `SHOW` wspiera wyświetlanie kilku kolumn datasetu
* [ ] obłsuga błędów dla komendy `SHOW`

## SET TARGET - wybór kolumny target

* [x] komenda `SET TARGET` wspiera przypisanie kolumny target
* [ ] obsługa błędów, np. nieistniejąca kolumna

## SPLIT - podział na zbiór treningowy i testowy

* [x] komenda `SPLIT` wspiera ratio, seed i shuffle
* [ ] obsługa błędów, np. błędny ratio, brak target lub datasetu

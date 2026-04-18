
---

#  FUNKCJONALNOŚĆ JĘZYKA

---

## LOAD - ładowanie danych

* [x] komenda `LOAD` wspiera pliki `.csv` wraz z opcjami
* [x] komenda `LOAD` wspiera pliki `.json` wraz z opcjami
* [x] komenda `LOAD` wspiera ładowanie wybranej liczby wierszy
* [x] obsługa błędów dla komendy `LOAD`

---

## SHOW - wyświetlanie danych

* [x] komenda `SHOW` wspiera wyświetlanie nazw kolumn
* [x] komenda `SHOW` wspiera wyświetlanie liczby kolumn/wierszy
* [x] komenda `SHOW` wspiera wyświetlanie wybranych kolumn
* [x] komenda `SHOW` wspiera wyświetlanie wybranych wierszy
* [x] komenda `SHOW` wspiera wyświetlanie całego datasetu
* [x] komenda `SHOW` wspiera wyświetlanie kilku kolumn datasetu
* [ ] komenda `SHOW` wspiera filtrowanie klauzulą `WHERE`
* [ ] obsługa błędów dla komendy `SHOW`

---

## SET TARGET - wybór kolumny target

* [x] komenda `SET TARGET` wspiera przypisanie kolumny target
* [ ] obsługa błędów, np. nieistniejąca kolumna
* [ ] ochrona targetu przed usunięciem w `DROP COLUMN`

---

## SPLIT - podział na zbiór treningowy i testowy

* [x] komenda `SPLIT` wspiera ratio, seed i shuffle
* [x] generowanie `train/test + target split`
* [ ] obsługa błędów, np. błędny ratio, brak datasetu
* [ ] walidacja czy target istnieje przed split

---

## PREPROCESSING - czyszczenie i transformacje danych

### DROP NA

* [x] komenda `DROP NA` usuwa brakujące wartości z całego datasetu
* [x] komenda `DROP NA` wspiera operację na wybranych kolumnach (`ON COLUMNS`)
* [ ] możliwość wyboru strategii (`DROP_ROWS`, `DROP_COLUMNS`, `FILL_MEAN`, `FILL_MEDIAN`)
* [ ] obsługa błędów (brak kolumn, brak datasetu)

---

### DROP COLUMN

* [x] komenda `DROP COLUMN` usuwa pojedynczą kolumnę
* [ ] obsługa wielu kolumn w jednej komendzie
* [ ] tryb bezpieczny (`SAFE MODE`) chroniący np. target
* [ ] warunkowe usuwanie kolumn (`WHERE TYPE`, `MISSING RATE`)
* [ ] obsługa błędów (nieistniejąca kolumna)

---

### NORMALIZE

* [x] komenda `NORMALIZE` działa na całym datasetcie
* [x] komenda `NORMALIZE` działa na wybranych kolumnach
* [ ] wybór metody (`MINMAX`, `ROBUST`)
* [ ] wybór zakresu (`[0,1]`, `[-1,1]`)
* [ ] obsługa wartości NaN / INF
* [ ] obsługa błędów (kolumny nie-numeryczne)

---

### STANDARDIZE

* [x] komenda `STANDARDIZE` działa na całym datasetcie
* [x] komenda `STANDARDIZE` działa na wybranych kolumnach
* [ ] wybór wariantu (`ddof=0`, `ddof=1`)
* [ ] tryb global vs per-column
* [ ] parametr stabilizacji (`EPS`)
* [ ] obsługa błędów (kolumny nie-numeryczne)

---

## WHERE - deklaracje warunkowe

* [x] wspiera zagnieżdżanie warunków nawiasami
* [x] wspiera negację warunków operatorem NOT
* [x] wspiera łączenie warunków operatorami logicznymi AND i OR
* [x] wspiera relacje (`=`, `!=`, `>`, `>=`, `<`, `<=`)
* [ ] prawidłowe rozróżnienie: kolumna vs string literal
* [ ] optymalizacja ewaluacji warunków

---

## CREATE - tworzenie modeli
* [x] wspiera tworzenie modelu
* [x] wspiera `LinearRegression` wraz z parametrami
* [x] wspiera `SVC` wraz z parametrami
* [x] wspiera `Ridge` wraz z parametrami
* [ ] wspiera inne modele
* [x] naprawa błedu, gdzie `CREATE` wymaga `WITH` nawet bez parametrów
* [x] możliwość zamiany kolejności parametrów
* [ ] obłsuga błędów
* [ ] testy
* [x] obmyślenie krótszego i uniwersalnego sposobu na ustawianie parametrów w `visit*ParamsList` dla każdego typu modelu

---

## TRAIN - trenowanie modeli
* [x] obsługa komendy `TRAIN`
* [x] dostosowanie komendy train do wprowadzonej `CREATE`
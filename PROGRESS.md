# Progress

---

## Ładowanie i wyświetlanie danych

### LOAD

* [x] Obsługa plików `.csv` (wraz z opcjami)
* [x] Obsługa plików `.json` (wraz z opcjami)
* [x] Ładowanie wybranej liczby wierszy
* [x] Obsługa błędów

### SHOW

* [x] Wyświetlanie całego datasetu
* [x] Wyświetlanie wybranych kolumn i wierszy
* [x] Wyświetlanie nazw kolumn
* [x] Wyświetlanie liczby kolumn i wierszy
* [ ] Filtrowanie wyników klauzulą `WHERE`
* [ ] Obsługa błędów

---

## Przygotowanie danych (Data Split & Target)

### SET TARGET

* [x] Przypisanie kolumny jako target
* [x] Ochrona targetu przed usunięciem (w `DROP COLUMN`)
* [ ] Obsługa błędów (np. nieistniejąca kolumna)

### SPLIT

* [x] Podział na zbiór `train` / `test` + wydzielenie targetu
* [x] Parametry podziału: `ratio`, `seed`, `shuffle`
* [ ] Walidacja istnienia targetu przed podziałem
* [ ] Obsługa błędów (np. błędny ratio, brak datasetu)

---

## Preprocessing i transformacje

### DROP NA (Brakujące wartości)

* [x] Usuwanie z całego datasetu
* [x] Usuwanie z wybranych kolumn (`ON COLUMNS`)
* [ ] Wybór strategii (`DROP_ROWS`, `DROP_COLUMNS`, `FILL_MEAN`, `FILL_MEDIAN`)
* [ ] Obsługa błędów

### DROP COLUMN

* [x] Usuwanie pojedynczej kolumny oraz wielu kolumn jednocześnie
* [x] Tryb `SAFE MODE` (ochrona ważnych kolumn, np. targetu)
* [x] Warunkowe usuwanie (`WHERE TYPE`, `MISSING RATE`)
* [x] Walidacja datasetu i listy kolumn przed wykonaniem operacji

### NORMALIZE

* [x] Działanie na całym datasetcie oraz na wybranych kolumnach
* [ ] Wybór metody (`MINMAX`, `ROBUST`) i zakresu (`[0,1]`, `[-1,1]`)
* [ ] Obsługa wartości `NaN` / `INF`
* [ ] Obsługa błędów (np. kolumny nie-numeryczne)

### STANDARDIZE

* [x] Działanie na całym datasetcie oraz na wybranych kolumnach
* [ ] Wybór wariantu (`ddof=0`, `ddof=1`)
* [ ] Tryb globalny vs per-kolumna
* [ ] Parametr stabilizacji (`EPS`)
* [ ] Obsługa błędów

---

## Logika warunkowa (WHERE)

* [x] Relacje (`=`, `!=`, `>`, `>=`, `<`, `<=`)
* [x] Łączenie warunków operatorami `AND` i `OR`
* [x] Negacja warunków (`NOT`)
* [x] Zagnieżdżanie warunków nawiasami
* [ ] Prawidłowe rozróżnienie: kolumna vs string literal
* [ ] Optymalizacja ewaluacji warunków

---

## Modelowanie (Tworzenie i Trening)

### CREATE (Tworzenie modeli)

* [x] Uniwersalny mechanizm tworzenia modeli (w tym przypisywanie parametrów `visit*ParamsList`)
* [x] Opcjonalność słowa kluczowego `WITH` przy braku parametrów (naprawiony błąd)
* [x] Możliwość definiowania parametrów w dowolnej kolejności
* [x] Obsługiwane modele:
  * [x] `LinearRegression`
  * [x] `Ridge`
  * [x] `KNeighborsRegression`
  * [x] `SVC`
* [ ] Dodanie wsparcia dla innych modeli
* [ ] Obsługa błędów przy błędnych parametrach
* [ ] Napisanie testów jednostkowych

### TRAIN (Trenowanie)

* [x] Obsługa komendy `TRAIN`
* [x] Integracja wykonania `TRAIN` z modelami zainicjowanymi przez `CREATE`

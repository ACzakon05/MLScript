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

# Spis tokenów

### 1. Słowa kluczowe strukturalne (wspólne dla wielu poleceń)

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `AS` | Alias lub "w roli" | `(?i)AS` | `LOAD 'data.csv' AS csv ...` |
| `BY` | Przyimek "przez" | `(?i)BY` | `DELIMITED BY ','` |
| `FOR` | "dla" (używane w `SET TARGET`) | `(?i)FOR` | `SET TARGET "price" FOR housing` |
| `FROM` | Źródło danych lub operacji | `(?i)FROM` | `SHOW ROWS 1 TO 5 FROM sales` |
| `INTO` | Miejsce docelowe podziału/ładowania | `(?i)INTO` | `SPLIT data RATIO 80:20 INTO train, test` |
| `KEEP` | Zachowaj wskazane kolumny | `(?i)KEEP` | `LOAD ... KEEP "col1","col2"` |
| `LIMIT` | Maksymalna liczba wierszy | `(?i)LIMIT` | `LOAD ... LIMIT 1000` |
| `OF` | Przyimek "z" (część wyrażeń agregujących) | `(?i)OF` | `SHOW COUNT OF ROWS FROM dataset` |
| `ON` | Operuj na podzbiorze | `(?i)ON` | `DROP NA FROM data ON "col1"` |
| `TO` | Separator zakresu | `(?i)TO` | `SHOW ROWS 10 TO 20 FROM dataset` |
| `WITH` | Dodatkowe opcje/parametry | `(?i)WITH` | `CREATE MODEL linreg CHOOSE LINEAR_REGRESSION WITH FIT_INTERCEPT = TRUE` |
| `WITHOUT` | Wyklucz wskazane kolumny | `(?i)WITHOUT` | `LOAD ... WITHOUT "id","tmp"` |

---

### 2. Literały logiczne

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `TRUE` | Prawda | `(?i)TRUE` | `FIT_INTERCEPT = TRUE` |
| `FALSE` | Fałsz | `(?i)FALSE` | `SHUFFLE FALSE` |

---

### 3. Polecenie LOAD

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `LOAD` | Załaduj zbiór danych | `(?i)LOAD` | `LOAD 'data.csv' AS csv INTO df` |
| `DELIMITED` | Określenie separatora pól | `(?i)DELIMITED` | `DELIMITED BY ';'` |
| `HEADER` | Obecność nagłówka w CSV | `(?i)HEADER` | `KEEP HEADER` / `WITHOUT HEADER` |
| `ORIENT` | Orientacja JSON | `(?i)ORIENT` | `ORIENT 'records'` |
| `CSV` | Format pliku CSV | `(?i)CSV` | `LOAD ... AS csv ...` |
| `JSON` | Format pliku JSON | `(?i)JSON` | `LOAD ... AS json ...` |
| `PKL` | Format pliku Pickle | `(?i)PKL` | `LOAD ... AS pkl ...` |
| `SQL` | Źródło zapytania SQL | `(?i)SQL` | `LOAD ... AS sql ...` |

---

### 4. Polecenie SHOW i funkcje agregujące

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `SHOW` | Wyświetl dane/metadane | `(?i)SHOW` | `SHOW ROWS 1 TO 5 FROM df` |
| `COUNT` | Policz elementy | `(?i)COUNT` | `SHOW COUNT OF ROWS FROM df` |
| `FEATURE` | Pojedyncza kolumna/cecha | `(?i)FEATURE` | `SHOW FEATURE "age" FROM df` |
| `FEATURES` | Wiele cech | `(?i)FEATURES` | `SHOW FEATURES FROM df` |
| `ROW` | Pojedynczy wiersz | `(?i)ROW` | `SHOW ROW 42 FROM df` |
| `ROWS` | Zakres wierszy | `(?i)ROWS` | `SHOW ROWS 10 TO 20 FROM df` |
| `MEAN` | Średnia arytmetyczna | `(?i)MEAN` | `SHOW MEAN OF "price" FROM df` |
| `MAX` | Wartość maksymalna | `(?i)MAX` | `SHOW MAX OF "score" FROM df` |
| `MIN` | Wartość minimalna | `(?i)MIN` | `SHOW MIN OF "age" FROM df` |
| `SUM` | Suma wartości | `(?i)SUM` | `SHOW SUM OF "sales" FROM df` |
| `MEDIAN` | Mediana | `(?i)MEDIAN` | `SHOW MEDIAN OF "income" FROM df` |
| `PROD` | Iloczyn wartości | `(?i)PROD` | `SHOW PROD OF "factors" FROM df` |
| `STD` | Odchylenie standardowe | `(?i)STD` | `SHOW STD OF "temperature" FROM df` |
| `VAR` | Wariancja | `(?i)VAR` | `SHOW VAR OF "pressure" FROM df` |
| `UNIQUE_VALS` | Unikalne wartości kolumny | `(?i)UNIQUE_VALS` | `SHOW UNIQUE_VALS OF "category" FROM df` |
| `UNIQUE_COUNT` | Liczba unikalnych wartości | `(?i)UNIQUE_COUNT` | `SHOW UNIQUE_COUNT OF "city" FROM df` |

---

### 5. Klauzula WHERE i operatory logiczne

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `WHERE` | Warunek filtrowania | `(?i)WHERE` | `SHOW dataset WHERE "age" > 30` |
| `AND` | Koniunkcja logiczna | `(?i)AND` | `"age" > 30 AND "income" < 50000` |
| `OR` | Alternatywa logiczna | `(?i)OR` | `"city" = 'NYC' OR "city" = 'LA'` |
| `NOT` | Negacja logiczna | `(?i)NOT` | `NOT "active" = TRUE` |

---

### 6. Operatory porównania (tokeny niejawne)

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `=` | Równy | `=` | `"status" = 'ok'` |
| `!=` | Różny od | `!=` | `"code" != 0` |
| `>` | Większy niż | `>` | `"score" > 0.8` |
| `<` | Mniejszy niż | `<` | `"count" < 100` |
| `>=` | Większy lub równy | `>=` | `"age" >= 18` |
| `<=` | Mniejszy lub równy | `<=` | `"price" <= 99.99` |

---

### 7. Polecenie SET TARGET

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `SET` | Ustaw wartość | `(?i)SET` | `SET TARGET "label" FOR df` |
| `TARGET` | Kolumna docelowa | `(?i)TARGET` | `SET TARGET "class" FOR data` |

---

### 8. Polecenie SPLIT

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `SPLIT` | Podziel zbiór danych | `(?i)SPLIT` | `SPLIT data RATIO 70:30 INTO train, test` |
| `RATIO_KW` | Słowo kluczowe proporcji | `(?i)RATIO` | `RATIO 80:20` |
| `SEED` | Ziarno losowości | `(?i)SEED` | `WITH SEED 42` |
| `SHUFFLE` | Przetasuj przed podziałem | `(?i)SHUFFLE` | `SHUFFLE TRUE` |

---

### 9. Polecenia PREPROCESS

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `DROP` | Usuń wiersze/kolumny | `(?i)DROP` | `DROP NA FROM df` |
| `COLUMN` | Odniesienie do kolumny | `(?i)COLUMN` | `DROP COLUMN "id" FROM df` |
| `NA` | Brakujące wartości | `(?i)NA` | `DROP NA FROM df ON "age"` |
| `NORMALIZE` | Normalizuj cechy | `(?i)NORMALIZE` | `NORMALIZE df ON "num"` |
| `STANDARDIZE` | Standaryzuj cechy | `(?i)STANDARDIZE` | `STANDARDIZE df` |
| `NAME` | Nazwa kolumny (w wyrażeniach) | `(?i)NAME` | `WHERE NAME = 'income'` |
| `TYPE` | Typ danych (w wyrażeniach) | `(?i)TYPE` | `WHERE TYPE = 'numeric'` |
| `MISSING_RATE` | Odsetek brakujących (w wyrażeniach) | `(?i)MISSING_RATE` | `WHERE MISSING_RATE > 0.2` |
| `IN` | Test przynależności | `(?i)IN` | `"country" IN ("US","UK")` |
| `SAFE` | Tryb bezpieczny | `(?i)SAFE` | `DROP COLUMN ... SAFE MODE` |
| `MODE` | Słowo kluczowe trybu (część `SAFE MODE`) | `(?i)MODE` | `SAFE MODE` |
| `METHOD` | Metoda normalizacji | `(?i)METHOD` | `METHOD MINMAX` |
| `RANGE` | Zakres normalizacji | `(?i)RANGE` | `RANGE ZERO_ONE` |
| `HANDLE` | Obsługa braków danych | `(?i)HANDLE` | `HANDLE FILL_MEAN` |
| `MINMAX` | Skalowanie min-max | `(?i)MINMAX` | `METHOD MINMAX` |
| `ROBUST` | Skalowanie odporne (robust) | `(?i)ROBUST` | `METHOD ROBUST` |
| `ZERO_ONE` | Zakres [0,1] | `(?i)ZERO_ONE` | `RANGE ZERO_ONE` |
| `MINUS_ONE_ONE` | Zakres [-1,1] | `(?i)MINUS_ONE_ONE` | `RANGE MINUS_ONE_ONE` |
| `DROP_NA` | Pomiń brakujące wartości | `(?i)DROP_NA` | `HANDLE DROP_NA` |
| `FILL_MEAN` | Uzupełnij średnią | `(?i)FILL_MEAN` | `HANDLE FILL_MEAN` |
| `FILL_MEDIAN` | Uzupełnij medianą | `(?i)FILL_MEDIAN` | `HANDLE FILL_MEDIAN` |

---

### 10. Polecenia CREATE MODEL / TRAIN

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `CREATE` | Utwórz model | `(?i)CREATE` | `CREATE MODEL ...` |
| `MODEL` | Słowo kluczowe modelu | `(?i)MODEL` | `CREATE MODEL linreg ...` |
| `CHOOSE` | Wybierz typ modelu | `(?i)CHOOSE` | `CREATE MODEL svm CHOOSE SVC` |
| `TRAIN` | Trenuj model | `(?i)TRAIN` | `TRAIN linreg ON train_data` |
| `LINEAR_REGRESSION` | Regresja liniowa | `(?i)LINEAR_REGRESSION` | `CHOOSE LINEAR_REGRESSION WITH ...` |
| `RIDGE` | Regresja grzbietowa (Ridge) | `(?i)RIDGE` | `CHOOSE RIDGE WITH ALPHA = 1.0` |
| `KNN_REGRESSION` | Regresja k-najbliższych sąsiadów | `(?i)KNN_REGRESSION` | `CHOOSE KNN_REGRESSION WITH N_NEIGHBORS = 5` |
| `SVC` | Klasyfikator wektorów nośnych | `(?i)SVC` | `CHOOSE SVC WITH KERNEL = 'rbf'` |
| `FIT_INTERCEPT` | Dopasuj wyraz wolny | `(?i)FIT_INTERCEPT` | `FIT_INTERCEPT = TRUE` |
| `MAX_ITERATIONS` | Maksymalna liczba iteracji | `(?i)MAX_ITERATIONS` | `MAX_ITERATIONS = 1000` |
| `N_JOBS` | Liczba równoległych zadań | `(?i)N_JOBS` | `N_JOBS = -1` |
| `POSITIVE` | Wymuś dodatnie współczynniki | `(?i)POSITIVE` | `POSITIVE = TRUE` |
| `TOL` | Tolerancja kryterium stopu | `(?i)TOL` | `TOL = 1e-4` |
| `ALPHA` | Siła regularyzacji (Ridge) | `(?i)ALPHA` | `ALPHA = 0.5` |
| `SOLVER` | Algorytm solvera (Ridge) | `(?i)SOLVER` | `SOLVER = 'auto'` |
| `N_NEIGHBORS` | Liczba sąsiadów (KNN) | `(?i)N_NEIGHBORS` | `N_NEIGHBORS = 7` |
| `WEIGHTS` | Funkcja wag (KNN) | `(?i)WEIGHTS` | `WEIGHTS = 'distance'` |
| `ALGORITHM` | Algorytm obliczeniowy (KNN) | `(?i)ALGORITHM` | `ALGORITHM = 'ball_tree'` |
| `SVC_C` | Parametr regularyzacji C (SVC) | `(?i)C` | `C = 1.0` |
| `COEF_0` | Współczynnik jądra (SVC) | `(?i)COEF_0` | `COEF_0 = 0.0` |
| `DEGREE` | Stopień wielomianu (SVC) | `(?i)DEGREE` | `DEGREE = 3` |
| `GAMMA` | Współczynnik jądra gamma (SVC) | `(?i)GAMMA` | `GAMMA = 'scale'` |
| `KERNEL` | Typ jądra (SVC) | `(?i)KERNEL` | `KERNEL = 'linear'` |
| `PROBABILITY` | Włącz estymację prawdopodobieństw (SVC) | `(?i)PROBABILITY` | `PROBABILITY = TRUE` |

---

### 11. Znaki interpunkcyjne

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `COMMA` | Separator listy | `,` | `KEEP "a","b"` |
| `SEMICOLON` | Terminator instrukcji | `;` | `LOAD ...; SHOW ...` |
| `LPAREN` | Nawias otwierający | `(` | `WHERE ("a" > 1)` |
| `RPAREN` | Nawias zamykający | `)` | `WHERE ("a" > 1)` |

---

### 12. Literały i identyfikatory

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `STRING` | Łańcuch znaków w pojedynczym cudzysłowie | `'[^']*'` | `'data.csv'`, `'records'` |
| `INTEGER` | Liczba całkowita (opcjonalny minus) | `-?(0\|[1-9][0-9]*)` | `42`, `-3`, `0` |
| `FLOAT` | Liczba zmiennoprzecinkowa (wymagane cyfry po kropce) | `-?(0\|[1-9][0-9]*)\.[0-9]+` | `3.14`, `-0.5`, `1.0` |
| `E_FLOAT` | Liczba w notacji naukowej | `(-?(0\|[1-9][0-9]*)(\.[0-9]+)?)[eE]-?(0\|[1-9][0-9]*)` | `1e-4`, `2.5E3` |
| `RATIO` | Dwie liczby całkowite oddzielone `:` | `INTEGER : INTEGER` | `80:20`, `70:30` |
| `COL_NAME` | Nazwa kolumny w podwójnym cudzysłowie | `"[^"]*"` | `"age"`, `"first name"` |
| `IDENTIFIER` | Nazwa zdefiniowana przez użytkownika (zbiór/model) | `[a-zA-Z_][a-zA-Z_0-9]*` | `df`, `train_data`, `linreg` |

---

### 13. Tokeny pomijane (nie generują wyjścia parsera)

| Token | Opis | Wyrażenie regularne | Przykład użycia |
|-------|------|---------------------|-----------------|
| `WS` | Białe znaki (spacje, tabulacje, nowe linie) | `[ \t\r\n]+` | (ignorowane między tokenami) |
| `COMMENT` | Komentarz liniowy zaczynający się od `#` | `#.*` | `# to jest komentarz` |

# Gramatyka
```g4
grammar MLScript;

// ==================================================
// Parser Rules
// ==================================================

// --------------------------------------------------
// Entry point
// --------------------------------------------------

prog
       : statement (SEMICOLON statement)* SEMICOLON? EOF
       ;

statement
       : loadStat 
       | showStat
       | setTargetStat
       | splitStat
       | preprocessStat
       | createModelStat
       | trainStat
       ;

// --------------------------------------------------
// LOAD command
// --------------------------------------------------

loadStat
       : LOAD STRING AS fileFormatLoadOptions INTO IDENTIFIER generalLoadOptions
       ;

fileFormatLoadOptions
       : CSV csvLoadOptions    # LoadCSVFile
       | SQL                   # LoadSQLFile
       | JSON jsonLoadOptions  # LoadJSONFile 
       | PKL                   # LoadPKLFile
       ;
         
csvLoadOptions
       : (DELIMITED BY STRING)? (KEEP HEADER | WITHOUT HEADER)? 
       ;

jsonLoadOptions
       : (ORIENT STRING)?
       ;

generalLoadOptions
       : (KEEP columnList)? (WITHOUT columnList)? (LIMIT INTEGER)?
       ;

// --------------------------------------------------
// SHOW command
// --------------------------------------------------

showStat
       : SHOW showOption
       ;

showOption
       : IDENTIFIER whereClause?                                     # ShowDataset 
       | FEATURES FROM IDENTIFIER                                    # ShowFeatures
       | COUNT OF (ROWS | FEATURES) FROM IDENTIFIER                  # ShowCount
       | ROW INTEGER FROM IDENTIFIER                                 # ShowSingleRow
       | ROWS INTEGER TO INTEGER FROM IDENTIFIER whereClause?        # ShowMultipleRows
       | FEATURE (COL_NAME | INTEGER) FROM IDENTIFIER                # ShowSingleFeature
       | FEATURES (columnList | INTEGER TO INTEGER) FROM IDENTIFIER  # ShowMultipleFeatures
       | aggFunc OF columnList FROM IDENTIFIER whereClause?          # ShowAggFunc
       ;

whereClause
       : WHERE condition
       ;

condition
       : LPAREN condition RPAREN                         # NestedCondition
       | NOT condition                                   # NotCondition
       | left=condition logicalOperator right=condition  # LogicalCondition
       | expression comparisonOperator expression        # RelationalCondition
       | expression IN LPAREN (columnList| literalList) RPAREN                # InCondition
       ;

expression
       : COL_NAME  # ColumnReference
       | literal   # LiteralValue
       | NAME      # NameExpr
       | TYPE                      # TypeExpr
       | MISSING_RATE              # MissingRateExpr
       

       ;

literal
       : INTEGER
       | FLOAT
       | STRING
       | TRUE
       | FALSE
       ;
literalList
       : literal (COMMA literal)*
       ;

// --------------------------------------------------
// SET TARGET command
// --------------------------------------------------

setTargetStat
       : SET TARGET COL_NAME FOR IDENTIFIER
       ;

// --------------------------------------------------
// SPLIT command
// --------------------------------------------------

splitStat
       : SPLIT IDENTIFIER RATIO_KW RATIO INTO IDENTIFIER COMMA IDENTIFIER
         (WITH SEED INTEGER COMMA SHUFFLE (TRUE | FALSE))? 
       ;


// --------------------------------------------------
// PREPROCESS command
// --------------------------------------------------

preprocessStat
  : dropNaStat
  | dropColumnStat
  | normalizeStat
  | standardizeStat
  ;

dropNaStat
  : DROP NA FROM IDENTIFIER (ON columnList)?
  ;

dropColumnStat
  : DROP COLUMN (columnList| whereClause) FROM IDENTIFIER safeMode?
  ;

normalizeStat
  : NORMALIZE IDENTIFIER
    (ON columnList)?
    (WITH normalizeOptions)?
  ;

normalizeOptions
  : methodOption
    (COMMA rangeOption)?
    (COMMA handleOption)?
  ;

methodOption
  : METHOD methodType
  ;

methodType
  : MINMAX
  | ROBUST
  ;

rangeOption
  : RANGE rangeType
  ;

rangeType
  : ZERO_ONE
  | MINUS_ONE_ONE
  ;

handleOption
  : HANDLE handleType
  ;

handleType
  : DROP_NA
  | FILL_MEAN
  | FILL_MEDIAN
  ;


standardizeStat
  : STANDARDIZE IDENTIFIER (ON columnList)?
  ;

// --------------------------------------------------
// CREATE model command
// --------------------------------------------------

createModelStat
       : CREATE MODEL IDENTIFIER CHOOSE modelDefinition 
       ;

modelDefinition
       : LINEAR_REGRESSION (WITH linearRegressionParamsList)?  # CreateModelLinReg
       | SVC               (WITH svcParamsList)?               # CreateModelSVC
       | RIDGE             (WITH ridgeParamsList)?             # CreateModelRidge
       | KNN_REGRESSION    (WITH knnParamsList)?               # CreateModelKNNReg
       ;

// == Regressors

// LinearRegression

linearRegressionParamsList
       : linRegModelParamWithVal (COMMA linRegModelParamWithVal)*
       ;

linRegModelParamWithVal
       : FIT_INTERCEPT val=(TRUE | FALSE)?  # LinRegParamFitIntercept
       | TOL           val=E_FLOAT          # LinRegParamTol
       | N_JOBS        val=INTEGER          # LinRegParamNJobs
       | POSITIVE      val=(TRUE | FALSE)?  # LinRegParamPositive
       ;

// Ridge

ridgeParamsList
       : ridgeModelParamWithVal (COMMA ridgeModelParamWithVal)*
       ;

ridgeModelParamWithVal
       : ALPHA          val=numeric          # RidgeParamAlpha
       | FIT_INTERCEPT  val=(TRUE | FALSE)?  # RidgeParamFitIntercept
       | MAX_ITERATIONS val=INTEGER          # RidgeParamMaxIter
       | TOL            val=numeric          # RidgeParamTol
       | SOLVER         val=STRING           # RidgeParamSolver
       | POSITIVE       val=(TRUE | FALSE)?  # RidgeParamPositive
       ;

// KNN Regressor

knnParamsList
       : knnModelParamWithVal (COMMA knnModelParamWithVal)*
       ;

knnModelParamWithVal
       : N_NEIGHBORS val=INTEGER  # KNNRegParamNNeighbors
       | WEIGHTS     val=STRING   # KNNRegParamWeights
       | ALGORITHM   val=STRING   # KNNParamAlgo
       | N_JOBS      val=INTEGER  # KNNParamNJobs
       ;

// == Classifiers

// SVC

svcParamsList
       : svcParamWithVal (COMMA svcParamWithVal)*
       ;

svcParamWithVal
       : SVC_C       val=numeric          # SvcParamC
       | KERNEL      val=STRING           # SvcParamKernel
       | DEGREE      val=INTEGER          # SvcParamDegree
       | GAMMA       val=svc_gamma_val    # SvcParamGamma
       | COEF_0      val=numeric          # SvcParamCoef0
       | PROBABILITY val=(TRUE | FALSE)?  # SvcParamProbability
       ;

svc_gamma_val
       : STRING
       | numeric
       ; 

// --------------------------------------------------
// TRAIN command
// --------------------------------------------------

trainStat
       : TRAIN modelName=IDENTIFIER ON trainSet=IDENTIFIER
       ;

// --------------------------------------------------
// Shared
// --------------------------------------------------
safeMode
       : SAFE MODE
       ;

columnList
       : COL_NAME (COMMA COL_NAME)* 
       ;

aggFunc
       : MEAN | MAX | MIN | SUM | MEDIAN | PROD | STD | VAR | UNIQUE_VALS | UNIQUE_COUNT 
       ;

logicalOperator
       : AND | OR 
       ;

comparisonOperator
       : '=' | '!=' | '>' | '<' | '>=' | '<='
       ;

numeric
       : FLOAT | INTEGER | E_FLOAT 
       ;

// ==================================================
// Lexer Rules
// ==================================================

// --------------------------------------------------
// Structural keywords shared across commands
// --------------------------------------------------

AS:      A S ;
BY:      B Y ;
FOR:     F O R ;
FROM:    F R O M ;
INTO:    I N T O ;
KEEP:    K E E P ;
LIMIT:   L I M I T ;
OF:      O F ;
ON:      O N ;
TO:      T O ;
WITH:    W I T H ;
WITHOUT: W I T H O U T ;

// --------------------------------------------------
// Boolean literals
// --------------------------------------------------

TRUE:  T R U E ;
FALSE: F A L S E ;

// --------------------------------------------------
// LOAD keywords
// --------------------------------------------------

LOAD:      L O A D ;
DELIMITED: D E L I M I T E D ;
HEADER:    H E A D E R ;
ORIENT:    O R I E N T ;

// File formats

CSV:  C S V ;
JSON: J S O N ;
PKL:  P K L ;
SQL:  S Q L ;

// --------------------------------------------------
// SHOW keywords
// --------------------------------------------------

SHOW:     S H O W ;
COUNT:    C O U N T ;
FEATURE:  F E A T U R E ;
FEATURES: F E A T U R E S ;
ROW:      R O W ;
ROWS:     R O W S ;

// Aggregate functions

MEAN:         M E A N ;
MAX:          M A X ;
MIN:          M I N ;
SUM:          S U M ;
MEDIAN:       M E D I A N ;
PROD:         P R O D ;
STD:          S T D ;
VAR:          V A R ;
UNIQUE_VALS:  U N I Q U E '_' V A L S ;
UNIQUE_COUNT: U N I Q U E '_' C O U N T ;

// WHERE clause

WHERE: W H E R E ;
AND:   A N D ;
NOT:   N O T ;
OR:    O R ;

// --------------------------------------------------
// SET TARGET keywords
// --------------------------------------------------

SET:    S E T ;
TARGET: T A R G E T ;

// --------------------------------------------------
// SPLIT keywords
// --------------------------------------------------

SPLIT:    S P L I T ;
RATIO_KW: R A T I O ;
SEED:     S E E D ;
SHUFFLE:  S H U F F L E ;

// --------------------------------------------------
// PREPROCESS keywords
// --------------------------------------------------

DROP:        D R O P ;
COLUMN:      C O L U M N ;
NA:          N A ;
NORMALIZE:   N O R M A L I Z E ;
STANDARDIZE: S T A N D A R D I Z E ;
NAME:    N A M E ;
TYPE:    T Y P E ;
MISSING_RATE: M I S S I N G '_' R A T E ;
IN:      I N ;
SAFE:    S A F E ;
MODE:    M O D E ;
METHOD: M E T H O D ;
RANGE: R A N G E ;
HANDLE: H A N D L E ;
MINMAX: M I N M A X ;
ROBUST: R O B U S T ;
ZERO_ONE: Z E R O '_' O N E ;
MINUS_ONE_ONE: M I N U S '_' O N E '_' O N E ;
DROP_NA: D R O P '_' N A;
FILL_MEAN: F I L L '_' M E A N ;
FILL_MEDIAN: F I L L '_' M E D I A N ;

// --------------------------------------------------
// CREATE MODEL / TRAIN keywords
// --------------------------------------------------

CHOOSE: C H O O S E ;
CREATE: C R E A T E ;
MODEL:  M O D E L ;
TRAIN:  T R A I N ;

// Model names

LINEAR_REGRESSION: L I N E A R '_' R E G R E S S I O N ;
RIDGE:             R I D G E ;
KNN_REGRESSION:    K N N '_' R E G R E S S I O N ;
SVC:               S V C ;

// Shared model params

FIT_INTERCEPT:  F I T '_' I N T E R C E P T ;
MAX_ITERATIONS: M A X '_' I T E R A T I O N S ;
N_JOBS:         N '_' J O B S ;
POSITIVE:       P O S I T I V E ;
TOL:            T O L ;

// Ridge params

ALPHA:  A L P H A ;
SOLVER: S O L V E R ;

// KNN Regression params

N_NEIGHBORS:  N '_' N E I G H B O R S ;
WEIGHTS:      W E I G H T S ;
ALGORITHM:    A L G O R I T H M ;

// SVC params

SVC_C:       C ;
COEF_0:      C O E F '_0' ;
DEGREE:      D E G R E E ;
GAMMA:       G A M M A ;
KERNEL:      K E R N E L ;
PROBABILITY: P R O B A B I L I T Y ;

// --------------------------------------------------
// Punctuation
// --------------------------------------------------

COMMA:     ',' ;
SEMICOLON: ';' ;
LPAREN:    '(' ;
RPAREN:    ')' ;

// --------------------------------------------------
// Literals
// --------------------------------------------------

STRING:     '\'' ~'\''* '\'' ;
INTEGER:    '-'? ( '0' | [1-9] DIGIT* ) ;
FLOAT:      INTEGER '.' DIGIT+ ;
E_FLOAT:    (INTEGER | FLOAT) E INTEGER ;
RATIO:      INTEGER ':' INTEGER ;
COL_NAME:   '"' ~'"'* '"' ;
IDENTIFIER: [a-zA-Z_] [a-zA-Z_0-9]* ;

// --------------------------------------------------
// Skipped
// --------------------------------------------------

WS:      [ \t\r\n]+ -> skip ;
COMMENT: '#' ~[\r\n]* -> skip ;

// --------------------------------------------------
// Fragments
// --------------------------------------------------

fragment DIGIT: [0-9];

fragment A: [aA];
fragment B: [bB];
fragment C: [cC];
fragment D: [dD];
fragment E: [eE];
fragment F: [fF];
fragment G: [gG];
fragment H: [hH];
fragment I: [iI];
fragment J: [jJ];
fragment K: [kK];
fragment L: [lL];
fragment M: [mM];
fragment N: [nN];
fragment O: [oO];
fragment P: [pP];
fragment Q: [qQ];
fragment R: [rR];
fragment S: [sS];
fragment T: [tT];
fragment U: [uU];
fragment V: [vV];
fragment W: [wW];
fragment X: [xX];
fragment Y: [yY];
fragment Z: [zZ];
```


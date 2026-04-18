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
       ;

expression
       : COL_NAME  # ColumnReference
       | literal   # LiteralValue
       ;

literal
       : INTEGER
       | STRING
       | TRUE
       | FALSE
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
  : DROP COLUMN COL_NAME FROM IDENTIFIER
  ;

normalizeStat
  : NORMALIZE IDENTIFIER (ON columnList)?
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
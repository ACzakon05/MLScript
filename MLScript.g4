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
       : CSV csvLoadOptions     # LoadCSVFile
       | SQL                    # LoadSQLFile
       | JSON jsonLoadOptions   # LoadJSONFile 
       | PKL                    # LoadPKLFile
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
       : IDENTIFIER whereClause?                                                  # ShowDataset 
       | FEATURES FROM IDENTIFIER                                                 # ShowFeatures
       | COUNT OF (ROWS | FEATURES) FROM IDENTIFIER                               # ShowCount
       | ROW INTEGER FROM IDENTIFIER                                              # ShowSingleRow
       | ROWS INTEGER TO INTEGER FROM IDENTIFIER whereClause?                     # ShowMultipleRows
       | FEATURE (COL_NAME | INTEGER) FROM IDENTIFIER                             # ShowSingleFeature
       | FEATURES (columnList | INTEGER TO INTEGER) FROM IDENTIFIER               # ShowMultipleFeatures
       | aggFunc OF columnList FROM IDENTIFIER whereClause?                       # ShowAggFunc
       ;

whereClause
       : WHERE condition
       ;

condition
       : LPAREN condition RPAREN                                                   # NestedCondition
       | NOT condition                                                             # NotCondition
       | left=condition logicalOperator right=condition                            # LogicalCondition
       | expression comparisonOperator expression                                  # RelationalCondition
       ;

expression
       : COL_NAME                                                                 # ColumnReference
       | literal                                                                  # LiteralValue
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
// CREATE model
// --------------------------------------------------

createModelStat
       : CREATE MODEL IDENTIFIER CHOOSE modelDefinition 
       ;

modelDefinition
       : LINEAR_REGRESSION (WITH linearRegressionParamsList)?         # CreateModelLinReg
       | SVC (WITH svcParamsList)?                                    # CreateModelSVC
       | RIDGE (WITH ridgeParamsList)?                                # CreateModelRidge
       ;

// == Regressors

// LinearRegression

linearRegressionParamsList
       : linRegModelParamWithVal (COMMA linRegModelParamWithVal)*
       ;

linRegModelParamWithVal
       : FIT_INTERCEPT val=(TRUE | FALSE)?  # LinRegParamFitIntercept
       | TOL val=E_FLOAT                             # LinRegParamTol
       | N_JOBS val=INTEGER                        # LinRegParamNJobs
       | POSITIVE val=(TRUE | FALSE)?           # LinRegParamPositive
       ;

// Ridge

ridgeParamsList
       : ridgeModelParamWithVal (COMMA ridgeModelParamWithVal)*
       ;

ridgeModelParamWithVal
       : ALPHA val=numeric                     # RidgeParamAlpha
       | FIT_INTERCEPT val=(TRUE | FALSE)?     # RidgeParamFitIntercept
       | MAX_ITERATIONS val=INTEGER            # RidgeParamMaxIter
       | TOL val=numeric                       # RidgeParamTol
       | SOLVER val=STRING                     # RidgeParamSolver
       | POSITIVE val=(TRUE | FALSE)?          # RidgeParamPositive
       ;

// == Classifiers

// SVC

svcParamsList
       : svcParamWithVal (COMMA svcParamWithVal)*
       ;

svcParamWithVal
       : ('C' | 'c') val=numeric          # SvcParamC
       | KERNEL val=STRING                # SvcParamKernel
       | DEGREE val=INTEGER               # SvcParamDegree
       | GAMMA val=svc_gamma_val     # SvcParamGamma
       | COEF_0 val=numeric               # SvcParamCoef0
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

numeric: FLOAT | INTEGER | E_FLOAT ;

// ==================================================
// Lexer Rules
// ==================================================

// --------------------------------------------------
// Keywords
// --------------------------------------------------

LOAD:        L O A D ;
INTO:        I N T O ;
SHOW:        S H O W ;
FEATURE:     F E A T U R E ;
FEATURES:    F E A T U R E S ;
FROM:        F R O M ;
KEEP:        K E E P ;
WITHOUT:     W I T H O U T ;
COUNT:       C O U N T ;
OF:          O F ;
ROWS:        R O W S ;
ROW:         R O W ;
TO:          T O ;
SET:         S E T ;
TARGET:      T A R G E T ;
FOR:         F O R ;
SPLIT:       S P L I T ;
RATIO_KW:    R A T I O ;
WITH:        W I T H ;
SEED:        S E E D ;
SHUFFLE:     S H U F F L E ;
TRUE:        T R U E ;
FALSE:       F A L S E ;
MEAN:        M E A N ;
MAX:         M A X ;
MIN:         M I N ;
AS:          A S ;
DELIMITED:   D E L I M I T E D ;
LIMIT:       L I M I T ;
BY:          B Y ;
HEADER:      H E A D E R ;
ORIENT:      O R I E N T ;
WHERE:       W H E R E ;
NOT:         N O T ;
AND:         A N D ;
OR:          O R ;
SUM:         S U M ;
MEDIAN:      M E D I A N ;
PROD:        P R O D ;
STD:         S T D ;
VAR:         V A R ;
UNIQUE_VALS: U N I Q U E '_' V A L S ;
UNIQUE_COUNT: U N I Q U E '_' C O U N T ;
DROP: D R O P;
COLUMN: C O L U M N;
NA: N A;
NORMALIZE: N O R M A L I Z E ;
STANDARDIZE: S T A N D A R D I Z E ;
ON: O N;
TRAIN: T R A I N ;
CHOOSE: C H O O S E ;
CREATE: C R E A T E ;
FIT_INTERCEPT: F I T '_' I N T E R C E P T ;
TOL: T O L ;
N_JOBS: N '_' J O B S ;
POSITIVE: P O S I T I V E ;
MODEL: M O D E L ;
KERNEL: K E R N E L ;
DEGREE: D E G R E E ;
GAMMA: G A M M A ;
COEF_0: C O E F '_0' ;
PROBABILITY: P R O B A B I L I T Y ;
ALPHA: A L P H A ;
MAX_ITERATIONS: M A X '_' I T E R A T I O N S ;
SOLVER: S O L V E R ;
RIDGE: R I D G E ;

// --------------------------------------------------
// Format keywords
// --------------------------------------------------

CSV:         C S V ;
SQL:         S Q L ;
JSON:        J S O N ;
PKL:         P K L ;
HTML:        H T M L ;

// --------------------------------------------------
// Models
// --------------------------------------------------

LINEAR_REGRESSION:   L I N E A R '_' R E G R E S S I O N ;
SVC:                 S V C ;

// --------------------------------------------------
// Punctuation
// --------------------------------------------------

COMMA: ',' ;
SEMICOLON: ';' ;
LPAREN: '(' ;
RPAREN: ')' ;

// --------------------------------------------------
// Literals
// --------------------------------------------------

STRING:  '\'' ~'\''* '\'' ;
INTEGER: '-'? ( '0' | [1-9] DIGIT* ) ;
FLOAT:   INTEGER '.' DIGIT+ ; 
E_FLOAT: (INTEGER | FLOAT) E INTEGER ;
IDENTIFIER: [a-zA-Z_] [a-zA-Z_0-9]* ;
RATIO: INTEGER ':' INTEGER ;
COL_NAME: '"' ~'"'* '"' ;

// --------------------------------------------------
// Skipped
// --------------------------------------------------

WS: [ \t\r\n]+ -> skip ;
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
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
       : SHOW showOption whereClause?
       ;

showOption
       : IDENTIFIER                                                               # ShowDataset 
       | FEATURES FROM IDENTIFIER                                                 # ShowFeatures
       | COUNT OF (ROWS | FEATURES) FROM IDENTIFIER                               # ShowCount
       | ROW INTEGER FROM IDENTIFIER                                              # ShowSingleRow
       | ROWS INTEGER TO INTEGER FROM IDENTIFIER                                  # ShowMultipleRows
       | FEATURE (STRING | INTEGER) FROM IDENTIFIER                               # ShowSingleFeature
       | FEATURES (columnList | INTEGER TO INTEGER) FROM IDENTIFIER               # ShowMultipleFeatures
       | aggFunc OF columnList FROM IDENTIFIER                                    # ShowAggFunc
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
       : SET TARGET STRING FOR IDENTIFIER
       ;

// --------------------------------------------------
// SPLIT command
// --------------------------------------------------

splitStat
       : SPLIT IDENTIFIER RATIO_KW RATIO INTO IDENTIFIER COMMA IDENTIFIER
         (WITH SEED INTEGER COMMA SHUFFLE (TRUE | FALSE))? 
       ;

// --------------------------------------------------
// Shared
// --------------------------------------------------

columnList
       : COL_NAME (COMMA COL_NAME)* 
       ;

aggFunc
       : MEAN | MAX | MIN 
       ;

logicalOperator
       : AND | OR 
       ;

comparisonOperator
       : '=' | '!=' | '>' | '<' | '>=' | '<='
       ;

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

// --------------------------------------------------
// Format keywords
// --------------------------------------------------

CSV:         C S V ;
SQL:         S Q L ;
JSON:        J S O N ;
PKL:         P K L ;
HTML:        H T M L ;

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
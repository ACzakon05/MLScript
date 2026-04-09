grammar MLScript;

// ==========
// Parser
// ==========

// Root
prog: (stat SEMICOLON)* EOF ;

// Rules
stat: loadStat 
| showStat
| setTargetStat
| splitStat ;

// Command definitions
loadStat: LOAD STRING INTO IDENTIFIER (
    (KEEP columnList) |
    (WITHOUT columnList)
)? ;

showStat: SHOW showOption ;

showOption: FEATURES FROM IDENTIFIER                    # ShowFeatures
          | COUNT OF (ROWS | FEATURES) FROM IDENTIFIER  # ShowCount
          | ROW INTEGER FROM IDENTIFIER                 # ShowSingleRow
          | ROWS INTEGER TO INTEGER FROM IDENTIFIER     # ShowMultipleRows
          ;

columnList: STRING (COMMA STRING)* ;

//SET TARGET "kolumna" FOR dataset;
setTargetStat: SET TARGET STRING FOR IDENTIFIER;

// SPLIT dataset RATIO 80:20 INTO train_subset, test_subset WITH SEED 42, SHUFFLE true;
splitStat: SPLIT IDENTIFIER RATIO_KW RATIO INTO IDENTIFIER COMMA IDENTIFIER
           (WITH SEED INTEGER COMMA SHUFFLE (TRUE | FALSE))? ;


// ==========
// Lexer
// ==========

// Keywords
LOAD:     L O A D ;
INTO:     I N T O ;
SHOW:     S H O W ;
FEATURES: F E A T U R E S ;
FROM:     F R O M ;
KEEP:     K E E P ;
WITHOUT:  W I T H O U T ;
COUNT:    C O U N T ;
OF:       O F ;
ROWS:     R O W S ;
ROW:      R O W ;
TO:       T O ;
SET: S E T ;
TARGET: T A R G E T ;
FOR: F O R ;
SPLIT: S P L I T ;
RATIO_KW: R A T I O ;
WITH: W I T H ;
SEED: S E E D ;
SHUFFLE: S H U F F L E ;
TRUE: T R U E ;
FALSE: F A L S E ;

// Fragments for case-insensitivity
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

// Punctuation
COMMA: ',' ;
SEMICOLON: ';' ;

// Primitives
STRING: '"' ~'"'* '"' ;
INTEGER: '-'? [1-9][0-9]* ;

// Identifiers
IDENTIFIER: [a-zA-Z_] [a-zA-Z_0-9]* ;

// Ignore
WS: [ \t\r\n]+ -> skip ;
COMMENT: '#' ~[\r\n]* -> skip ;

//Helper Tokens
RATIO: INTEGER ':' INTEGER ;

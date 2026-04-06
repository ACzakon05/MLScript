grammar MLScript;

// ==========
// Parser
// ==========

// Root
prog: (stat SEMICOLON)* EOF ;

// Rules
stat: loadStat | showStat ;

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
INTEGER: [1-9]*[0-9]+ ;

// Identifiers
IDENTIFIER: [a-zA-Z_] [a-zA-Z_0-9]* ;

// Ignore
WS: [ \t\r\n]+ -> skip ;
COMMENT: '#' ~[\r\n]* -> skip ;
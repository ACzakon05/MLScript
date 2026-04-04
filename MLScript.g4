grammar MLScript;

// ==========
// Parser
// ==========

// Root
prog: (stat SEMICOLON)+ EOF ;

// Rules
stat: loadStat ;

// Command definitions
loadStat: LOAD STRING INTO IDENTIFIER (
    (KEEP columnList) |
    (WITHOUT columnList)
)? ;

columnList: STRING (COMMA STRING)* ;

// ==========
// Lexer
// ==========

// Keywords
LOAD: 'LOAD' ;
INTO: 'INTO' ;
KEEP: 'KEEP' ;
WITHOUT: 'WITHOUT' ;

// Punctuation
COMMA: ',' ;
SEMICOLON: ';' ;

// Primitives
STRING: '"' ~'"'* '"' ;

// Identifiers
IDENTIFIER: [a-zA-Z_] [a-zA-Z_0-9]* ;

// Ignore
WS: [ \t\r\n]+ -> skip ;
COMMENT: '#' ~[\r\n]* -> skip ;
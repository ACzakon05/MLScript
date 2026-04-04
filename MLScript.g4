grammar MLScript;

// ==========
// Parser
// ==========

// Root
prog: stat+ EOF ;

// Rules
stat: loadStat ;

// Command definitions
loadStat: LOAD STRING INTO IDENTIFIER;

// ==========
// Lexer
// ==========

// Keywords
LOAD: 'LOAD' ;
INTO: 'INTO' ;

// Primitives
STRING: '"' ~'"'* '"' ;

// Identifiers
IDENTIFIER: [a-zA-Z_] [a-zA-Z_0-9]* ;

// Ignore
WS: [ \t\r\n]+ -> skip ;
COMMENT: '#' ~[\r\n]* -> skip ;
#ifndef FRONT_H
#define FRONT_H

/* Token types */
typedef enum {
    LETTER,         DIGIT,      UNKNOWN,
    ASSIGN_OP,      ADD_OP,     KEY_IN,
    LESSER_OP,      SUB_OP,     KEY_OUT,
    GREATER_OP,     MULT_OP,    KEY_IF,
    EQUAL_OP,       DIV_OP,     KEY_ELSE,
    NEQUAL_OP,      MOD_OP,     OPEN_CURL,
    LEQUAL_OP,      BOOL_AND,   CLOSE_CURL,
    GEQUAL_OP,      BOOL_OR,    IDENT,
    OPEN_PAREN,     BOOL_NOT,   INT_LIT,
    CLOSE_PAREN,    SEMICOLON
} Token;

/* Global Variables */
extern Token nextToken;
extern char lexeme[100];

int lex();

#endif

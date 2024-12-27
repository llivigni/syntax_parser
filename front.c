/*
=============================================================================
Title       : front.c
Description : A lexical analyzer that identifies lexemes and tokens
Author      : llivigni (R#11800181)
Date        : 11/06/2024
Version     : 1.0
Usage       : Compile and run this program using a makefile against
              the grading script on the HPCC.
Notes       : This program requires front.h and parser.h.
C Version   : GNU GCC
=============================================================================
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>     /* For 'exit()' */
#include <string.h>     /* For 'strcmp()' */

#include "front.h"
#include "parser.h"

/* Global Variables */
Token nextToken;
char lexeme[100];
int currentLineNumber = 1;
int tokenLineNumber = 1;

/* Local Variables */
static int charClass;
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declaration */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main(int argc, char *argv[])
{
    printf("Cooke Parser :: R11800181\n\n");

    /* Check if argument is provided */
    if (argc != 2) {
        printf("ERROR - executable and 1 argument expected (./____ ____)");
        exit(2); /* Exit 2 if user does not provide a file as input */
    }
    /* Open the input data file and process its contents */
    if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR - cannot open %s \n", argv[1]);
        exit(3); /* Exit 3 if user provided file but file does not exist */
    } else {
        getChar();
        lex();     /* Get nextToken */
        program(); /* Call program to begin parsing */
    }

    fclose(in_fp);
    return 0;
}
/******************************************************/
/* lookup - a function to lookup parentheses/curly braces/operators/semicolon and return the token */
static int lookup(char ch) {
    switch (ch) {
    case '=': /* '=' or '==' */
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                nextToken = EQUAL_OP;
            } else {
                ungetc(nextChar, in_fp);    /* Move file pointer back if nextChar != '=' */
                nextToken = ASSIGN_OP;
            }
        break;
    case '<': /* '<' or '<=' */
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                nextToken = LEQUAL_OP;
            } else {
                ungetc(nextChar, in_fp);    /* Move file pointer back if nextChar != '=' */
                nextToken = LESSER_OP;
            }
        break;
    case '>': /* '>' or '>=' */
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                nextToken = GEQUAL_OP;
            } else {
                ungetc(nextChar, in_fp);    /* Move file pointer back if nextChar != '=' */
                nextToken = GREATER_OP;
            }
        break;
    case '(':
           addChar();
           nextToken = OPEN_PAREN;
           break;
    case ')':
            addChar();
            nextToken = CLOSE_PAREN;
            break;
    case '+':
            addChar();
            nextToken = ADD_OP;
            break;
    case '-':
            addChar();
            nextToken = SUB_OP;
            break;
    case '*':
            addChar();
            nextToken = MULT_OP;
            break;
    case '/':
            addChar();
            nextToken =  DIV_OP;
            break;
    case '%':
            addChar();
            nextToken = MOD_OP;
            break;
    case '&': /* '&&' (no address operator) */
            addChar();
            getChar();
            if (nextChar == '&') {
                addChar();
                nextToken = BOOL_AND;
            }
        break;
    case '|': /* '||' ('|' does not exist) */
            addChar();
            getChar();
            if (nextChar == '|') {
                addChar();
                nextToken = BOOL_OR;
            }
        break;
    case '!': /* '!' or '!=' */
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                nextToken = NEQUAL_OP;
            } else {
                ungetc(nextChar, in_fp);    /* Move file pointer back if nextChar != '=' */
                nextToken = BOOL_NOT;
            }
        break;
    case ';':
            addChar();
            nextToken = SEMICOLON;
            break;
    case '{':
            addChar();
            nextToken = OPEN_CURL;
            break;
    case '}':
            addChar();
            nextToken = CLOSE_CURL;
            break;
    default:
            addChar();
            nextToken = UNKNOWN;
            break;
    }
    return nextToken;
}
/******************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98){
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}
/******************************************************/
/* getChar - a function to get the next character of input and determine its character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (nextChar == '\n') {
            currentLineNumber++;       /* If a newline character is encountered, increment line number */
        }
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}
/******************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}
/******************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    tokenLineNumber = currentLineNumber;    /* Store the current line number */

    switch (charClass) {
    /* Parse identifiers */
    case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            /* Check for keywords */
            if (strcmp(lexeme, "input") == 0) {
                nextToken = KEY_IN;
            } else if (strcmp(lexeme, "output") == 0) {
                nextToken = KEY_OUT;
            } else if (strcmp(lexeme, "if") == 0) {
                nextToken = KEY_IF;
            } else if (strcmp(lexeme, "else") == 0) {
                nextToken = KEY_ELSE;
            } else {
                nextToken = IDENT;
            }
        break;

    /* Parse integer literals */
    case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
        break;

    /* Parenthesis, curly braces, operators, and semicolon */
    case UNKNOWN:
            lookup(nextChar);
            getChar();
        break;

    /* EOF */
    case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
        break;
    } /* End of switch */
    return nextToken;
} /* End of function lex */

#ifndef PARSER_H
#define PARSER_H

#include "front.h"

/* Global function declaration */
void program();
void statement();
void statement_prime();
void condition();
void condition_prime();
void expression();
void term();
void factor();

/* Global Variables */
extern Token nextToken;
extern int tokenLineNumber;
extern int currentLineNumber;

#endif

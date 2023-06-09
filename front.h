#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define INC_OP 27
#define DEC_OP 28
#define LESSER_OP 29
#define GREATER_OP 30
#define EQUAL_OP 31
#define NEQUAL_OP 41
#define LEQUAL_OP 33
#define GEQUAL_OP 34
#define SEMICOLON 35
#define KEY_READ 36
#define KEY_WRITE 37
#define KEY_WHILE 38
#define KEY_DO 39
#define KEY_OD 40
#define UNKNOWN_TOK 41


int lex();

#endif

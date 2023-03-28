/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages. 
 * Pearson, 10th edition.
 *
 */

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
static char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *in_fp;
//some variables to hold characters, and a list of potential keywords.
char charOut[1][100] = {""};
static char keyWords[5][20] = {"while","do","read","write","od"};

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main(int argc, char *argv[]) 
{

    /* Open the input data file and process its contents */
    if(fopen(argv[1], "r")  == NULL){
        printf("ERROR = Cannot find the file. \n");
       // exit(0);
    }
    else if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR - File not provided \n");
      //  exit(0);
    } else {
        getChar();
        do {
            lex();
         //   expr();
        } while (nextToken != EOF);
    }
    // if you make it here, congrats!
    // printf("Syntax Validated\n");
    //exit();
    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the 
 * token */

/* The General logic I took here was to add the char, change the nextToken to reflect the char,
    then use strcpy to add the token name to the list of my token names and output it.
    should be relatively slow, as i don't think strcpy is very good.*/
    /* The most important part of the change here is having every case call its OWN getChar() call anytime
       it reads anything, as the lookup method no longer does.
       While significantly slower, this guarantees the parser never grabs a token it can't actually parse as it only knows
       what token it is currently on.*/
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            getChar();
            nextToken = LEFT_PAREN;
            strcpy(charOut[0],"LEFT_PAREN");
            break;
        case ')':
            addChar();
            getChar();
            nextToken = RIGHT_PAREN;
            strcpy(charOut[0],"RIGHT_PAREN");
            break;
            /* The first case of where the above logic is very important
                Old program failed on cases such as <! or -! as it was reading -, calling getchar, and checking for another -
                the problem with this is, since it defaults to lookup calling getChar(), it never reads the 2nd char it was grabbing
                now it does as the character that is grabbed is just looked up immediately.*/
        case '+':
            addChar();
            getChar();
            if(nextChar == '+'){
                addChar();
                getChar();
                nextToken == INC_OP;
                strcpy(charOut[0],"INC_OP");
            }else{            
            nextToken = ADD_OP;
            strcpy(charOut[0],"ADD_OP");
            }
            break;
        case '-':
            addChar();
            getChar();
            if(nextChar == '-'){
                addChar();
                getChar();
                nextToken == DEC_OP;
                strcpy(charOut[0],"DEC_OP");
            }else{
            nextToken = SUB_OP;
            strcpy(charOut[0],"SUB_OP");
            }
            break;
        case '*':
            addChar();
            getChar();
            nextToken == MULT_OP;
            strcpy(charOut[0],"MULT_OP");
            break;
        case '/':
            addChar();
            getChar();
            nextToken = DIV_OP;
            strcpy(charOut[0],"DIV_OP");
            break;
        case '<':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                getChar();
                nextToken = LEQUAL_OP;
                strcpy(charOut[0],"LEQUAL_OP");
            } else {
            nextToken = LESSER_OP;
            strcpy(charOut[0],"LESSER_OP");
            }
            break;
        case '>':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                getChar();
                nextToken = GEQUAL_OP;
                strcpy(charOut[0],"GEQUAL_OP");
            } else {
            nextToken = GREATER_OP;
            strcpy(charOut[0],"GREATER_OP");
            }
            break;
        case ':':
            addChar();
            getChar();
            if(nextChar == '='){
                addChar();
                getChar();
                nextToken == ASSIGN_OP;
                strcpy(charOut[0],"ASSIGN_OP");
            } else {
                nextToken == UNKNOWN_TOK;
                strcpy(charOut[0],"UNKNOWN");
                break;
            }
            break;
        case '=':
            addChar();
            getChar();
            nextToken = EQUAL_OP;
            strcpy(charOut[0],"EQUAL_OP");
            break;
        case '!':
            addChar();
            getChar();
            if(nextChar == '=')
            {
                addChar();
                getChar();
                nextToken == NEQUAL_OP;
                strcpy(charOut[0],"NEQUAL_OP");
                //break;
            } else {
                nextToken == UNKNOWN_TOK;
                strcpy(charOut[0],"UNKNOWN");
                //break;
            }
            break;
        case ';':
            addChar();
            getChar();
            nextToken = SEMICOLON;
            strcpy(charOut[0],"SEMICOLON");
            break;
        default:
            addChar();
            getChar();
            nextToken = UNKNOWN_TOK;
            strcpy(charOut[0],"UNKNOWN");
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
// Needs 0 modifications for this one.
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    int keywordFlag = 0;
    char uhoh;

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            //Detects if a identifier is the first letter in a keyword, and build potential keyword.
            // This is detecting if a 5 letter lexeme matches the list of existing keywords. If it does, we raise a flag.
            // The latter portion of this code is now semi-obsolete I believe.
            for(int i = 0; i < 5; i++)
            {
                if(strcmp(lexeme,keyWords[i]) == 0)
                {
                    keywordFlag = 1;
                    uhoh = keyWords[i][0];
                }
            }
            //printf("%c \n",uhoh);
            //printf("%c\n",uhoh);
            //Switch cases to find the words.
            // The special case is w
            // Scratched that idea entirely, Since it wouldn't work at all. Now featuring if statements.
            if(keywordFlag == 1)
            {
                if(strcmp(lexeme,"while") == 0)
                {
                    nextToken = KEY_WHILE;
                    strcpy(charOut[0], "KEY_WHILE");
                    break;
                }
                if(strcmp(lexeme,"write") == 0)
                {
                    nextToken = KEY_WRITE;
                    strcpy(charOut[0], "KEY_WRITE");
                    break;
                }
                if(strcmp(lexeme,"read") == 0)
                {
                    nextToken = KEY_READ;
                    strcpy(charOut[0], "KEY_READ");
                    break;
                }
                if(strcmp(lexeme,"do") == 0)
                {
                    nextToken = KEY_DO;
                    strcpy(charOut[0], "KEY_DO");
                    break;
                }
                if(strcmp(lexeme,"od") == 0)
                {
                    nextToken = KEY_DO;
                    strcpy(charOut[0], "KEY_OD");
                    break;
                }
            } else {
            nextToken = IDENT;
            strcpy(charOut[0], "IDENT");
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
            strcpy(charOut[0], "INT_LIT");
            break;

        /* Parentheses and operators */
        // VERY IMPORTANT CHANGE:
        /* NEED to remove the getChar() call from this lookup case, otherwise the parser can't read things in
           ++ or -- or <= or >= as it gets 1 lexeme ahead of where it should be, and never READS the 2nd token in the lexeme.
        */
        case UNKNOWN:
            lookup(nextChar);
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
    if(nextToken != EOF)
       printf("%s\t%s\n", lexeme, charOut);
    //printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */


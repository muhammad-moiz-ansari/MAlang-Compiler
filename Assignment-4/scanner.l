%{
#include <iostream>
#include <string>
#include <cstring>
#include "parser.tab.h"

// Track line and column numbers for better error reporting
int line_num = 1;
int col_num = 1;

// Flex runs this macro right before executing the code for ANY matched rule.
// yyleng is the length of the text it just matched.
#define YY_USER_ACTION col_num += yyleng;

%}

%option noyywrap
%option nounput
%option noinput

%%

"{"         { return LBRACE; }
"}"         { return RBRACE; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
":"         { return COLON; }
","         { return COMMA; }

"true"      { return TRUE_VAL; }
"false"     { return FALSE_VAL; }
"null"      { return NULL_VAL; }

\"([^\"\\]|\\.)*\" { 
    std::string text = yytext;
    yylval.str_val = strdup(text.substr(1, text.length() - 2).c_str());
    return STRING; 
}

-?[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)? { 
    yylval.num_val = atof(yytext);
    return NUMBER; 
}

[ \t\r]+    { /* Ignore spaces, tabs, and carriage returns */ }

\n          { 
    /* When we hit a newline, increment the line count and reset column to 1 */
    line_num++; 
    col_num = 1; 
}

.           { 
    std::cerr << "Lexical Error at Line " << line_num << ", Col " << col_num 
              << ": unexpected character '" << yytext << "'\n"; 
    exit(1); 
}

%%
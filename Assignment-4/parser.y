%{
#include <iostream>
#include <string>
#include "ast.h"

// Declare Flex functions so Bison knows they exist
int yylex();
void yyerror(const char *s);

// A global variable to hold the root of our completely built tree
ASTNode* root = nullptr;
%}

/* The %union defines all the possible data types that tokens and grammar rules can return.
  Because standard Bison mixes C and C++, we use pointers and basic types here.
*/
%union {
    char* str_val;
    double num_val;
    class ASTNode* node_val;
    class ObjectNode* obj_val;
    class ArrayNode* arr_val;
}

/* Declare our tokens and assign them a type from the union */
%token LBRACE RBRACE LBRACKET RBRACKET COLON COMMA
%token TRUE_VAL FALSE_VAL NULL_VAL
%token <str_val> STRING
%token <num_val> NUMBER

/* Declare our non-terminals (grammar rules) and assign them a type */
%type <node_val> value
%type <obj_val> object members
%type <arr_val> array elements
%type <str_val> key

/* Tell Bison where the grammar starts */
%start json

%%

json:
    value { root = $1; } /* When the whole file is parsed, save the top node to our global root */
    ;

value:
      object        { $$ = $1; }
    | array         { $$ = $1; }
    | STRING        { $$ = new StringNode(std::string($1)); free($1); }
    | NUMBER        { $$ = new NumberNode($1); }
    | TRUE_VAL      { $$ = new BoolNode(true); }
    | FALSE_VAL     { $$ = new BoolNode(false); }
    | NULL_VAL      { $$ = new NullNode(); }
    ;

object:
      LBRACE RBRACE { $$ = new ObjectNode(); }
    | LBRACE members RBRACE { $$ = $2; }
    ;

members:
      key COLON value { 
          $$ = new ObjectNode(); 
          $$->addMember(std::string($1), $3); 
          free($1);
      }
    | members COMMA key COLON value { 
          $$ = $1; 
          $$->addMember(std::string($3), $5); 
          free($3);
      }
    ;

array:
      LBRACKET RBRACKET { $$ = new ArrayNode(); }
    | LBRACKET elements RBRACKET { $$ = $2; }
    ;

elements:
      value { 
          $$ = new ArrayNode(); 
          $$->addElement($1); 
      }
    | elements COMMA value { 
          $$ = $1; 
          $$->addElement($3); 
      }
    ;

key:
    STRING { $$ = $1; }
    ;

%%

// The error handling function Bison calls if the JSON is invalid
void yyerror(const char *s) {
    std::cerr << "Error: " << s << std::endl;
}
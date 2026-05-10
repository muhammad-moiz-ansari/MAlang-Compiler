#include "ast.h"
#include <iostream>
using namespace std;

// Telling C++ that these exist in our Bison file (parser.y)
extern int yyparse();
extern ASTNode *root;

int main() {
  // yyparse() automatically reads from standard input (cin).
  // It returns 0 if parsing was successful!
  int parseResult = yyparse();

  if (parseResult == 0 && root != nullptr) {
        
        // --- NEW: Print the AST Structure ---
        std::cout << "========== AST STRUCTURE ==========\n";
        root->printTree(0);
        std::cout << "\n========== XML OUTPUT =============\n";

        // Rule 1: Outer wrapper
        std::cout << "<root>\n";
        root->printXML("", 1); 
        std::cout << "</root>\n";
        
    } else {
    // If there was a syntax error, Bison's yyerror() already printed the error
    // message, so we just exit gracefully.
    return 1;
  }

  return 0;
}
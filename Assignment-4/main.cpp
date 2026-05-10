#include "ast.h"
#include <iostream>

// Telling C++ that these exist in our Bison file (parser.y)
extern int yyparse();
extern ASTNode *root;

int main() {
  // yyparse() automatically reads from standard input (cin).
  // It returns 0 if parsing was successful!
  int parseResult = yyparse();

  if (parseResult == 0 && root != nullptr) {
    // Rule 1: The final XML output must have exactly one outer wrapper: <root>
    // ... </root>
    std::cout << "<root>\n";

    // Telling the top node to print itself.
    // We pass an empty string "" so it doesn't print an extra object tag around
    // the root.
    root->printXML("");

    std::cout << "</root>\n";
  } else {
    // If there was a syntax error, Bison's yyerror() already printed the error
    // message, so we just exit gracefully.
    return 1;
  }

  return 0;
}
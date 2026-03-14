#include <iostream>
#include "grammar.h"
#include "left_factoring.h"
// #include "left_recursion.h"
// #include "first_follow.h"
// #include "parser.h"

using namespace std;

int main() {
    // Step 1 — load grammar
    Grammar g = loadGrammar("input/grammar2.txt");
    cout << "=== Original Grammar ===\n";
    printGrammar(g);

    // Left Factoring
    cout << "=== Left Factored Grammar ===\n";
    leftFactor(g);
    printGrammar(g);

    return 0;
}
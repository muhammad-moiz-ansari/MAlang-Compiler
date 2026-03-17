#include <iostream>
#include "grammar.h"
#include "left_factoring.h"
#include "left_recursion.h"
#include "first_follow.h"
#include "parser.h"
#include "tree.h"
#include "stack.h"
#include <algorithm>
using namespace std;

int main() {
    string filename_suffix = "5_error_handle1";

    // load grammar
    Grammar gr = loadGrammar("input/grammar" + filename_suffix + ".txt");
    cout << "=== Original Grammar ===\n";
    printGrammar(gr);

    // Left Recursion Removal
    cout << "=== Left Recursion Removed Grammar ===\n";
    Grammar g = *eliminateRecursion(gr);
    printGrammar(g);
    
    // Left Factoring
    cout << "=== Left Factored Grammar ===\n";
    leftFactor(g);
    printGrammar(g);

    cout << "=== First & Follow Sets ===\n";

    // First Sets
    map<string, set<string>> first = computeFirst(g);
    
    // Follow Sets
    map<string, set<string>> follow = computeFollow(g, first);

    printFirst(first, follow, g);
    printFollow(first, follow, g);

    // Parsing Table
    createParseTable(first, follow, g);

    cout << endl << endl;
    printParseTable(g);

    auto inputs = readInputFile("input/input" + filename_suffix + ".txt");
    for (auto& input : inputs) {
        parse(input, g);
        ParseTreeNode* root = buildParseTree(g, ll1table, input);

        if (root) {
            cout << "\n\n======= PARSE TREE (ASCII) =======\n\n";
            printTreeColored(root, g);
        }
    }
    return 0;
}
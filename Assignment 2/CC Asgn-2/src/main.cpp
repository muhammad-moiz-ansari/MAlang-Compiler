#include <iostream>
#include "grammar.h"
#include "left_factoring.h"
#include "left_recursion.h"
#include "first_follow.h"
#include "parser.h"
#include "tree.h"
//#include <stack>
#include "stack.h"
#include <algorithm>
using namespace std;

int main() {
    // load grammar
    Grammar gr = loadGrammar("input/grammar1.txt");
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
    createParseTable(first, follow, g);

    /*
    cout << endl;
    for (int i = 0; i < g.nonTerminals.size();i++) {
        cout << g.nonTerminals[i] << "     ";
        for (int j = 0; j < g.terminals.size();j++) {
            for (int k = 0; k < ll1table[g.nonTerminals[i]][g.terminals[j]].prods.size(); k++) {
                cout << g.terminals[j] << ":";
                for (int l = 0;l < ll1table[g.nonTerminals[i]][g.terminals[j]].prods[k].symbols.size();l++) {
                    cout << ll1table[g.nonTerminals[i]][g.terminals[j]].prods[k].symbols[l];
               }
                cout << "    ";
            }
        }
        cout << endl;
    }
    */
    cout << endl << endl;
    printParseTable(g);

    vector<string> input = { "id", "+", "id", "*", "id" };
    ParseTreeNode* root = buildParseTree(g, ll1table, input);

    if (root) {
        cout << "\n\n======= PARSE TREE (ASCII) =======\n\n";
        printTreeColored(root, g);
    }
    return 0;
}
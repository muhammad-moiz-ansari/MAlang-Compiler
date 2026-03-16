#include <iostream>
#include "grammar.h"
#include "left_factoring.h"
#include "left_recursion.h"
#include "first_follow.h"
#include "parser.h"

using namespace std;

int main() {
    // load grammar
    Grammar gr = loadGrammar("input/grammar1.txt");
    cout << "=== Original Grammar ===\n";
    printGrammar(gr);

    /*
    for (int i = 1;i <= 5;i++) {
        if (i == 1)
            cout << "A -> Aa | B Test" << endl << endl;
        if (i == 2)
            cout << "A -> Xa | B, X -> Ac Test" << endl << endl;
        if (i == 3)
            cout << "A -> Aa Test" << endl << endl;
        if (i == 4)
            cout << "A -> A | B Test" << endl << endl;
        if (i == 5)
            cout << "A -> A Test" << endl << endl;

        Grammar g = loadGrammar("input/left_Recursion_Test" + to_string(i) + ".txt");
        Grammar* gg = eliminateRecursion(g);
        printGrammar(*gg);
        cout << endl <<"-----------------------------" << endl;
    }
    */

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

    return 0;
}
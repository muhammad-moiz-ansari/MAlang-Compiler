#include <iostream>
#include "grammar.h"
#include "left_factoring.h"
#include "left_recursion.h"
#include "first_follow.h"
// #include "parser.h"

using namespace std;

map<string, map<string, GrammarRule>> ll1table;

void createParseTable(map<string, set<string>>& first, map<string, set<string>>& follow, Grammar& g) {
    for (int i = 0; i < g.nonTerminals.size(); i++) {
        ll1table[g.nonTerminals[i]];
        GrammarRule r;
        for (int j = 0; j < g.terminals.size(); j++) {
            ll1table[g.nonTerminals[i]][g.terminals[j]] = r;
        }
        ll1table[g.nonTerminals[i]]["$"] = r;
    }
    
    for (int i = 0; i < g.nonTerminals.size(); i++) {
        for (auto x : first[g.nonTerminals[i]])
            if (isEpsilon(x)) {
                for (auto y : follow[g.nonTerminals[i]]) {
                    Production p;
                    //p.symbols.push_back(); SYMBOLS OF PRODUCTION
                    GrammarRule r;
                    r.prods.push_back(p);
                    r.nonTerminal = g.nonTerminals[i];
                    if (ll1table[g.nonTerminals[i]][y].prods.empty())
                        ll1table[g.nonTerminals[i]][y] = r;
                    else
                        cout << "Error" << endl;
                }
            }
            else {
                Production p;
                //p.symbols.push_back(); SYMBOLS OF PRODUCTION
                GrammarRule r;
                r.prods.push_back(p);
                r.nonTerminal = g.nonTerminals[i];
                if (ll1table[g.nonTerminals[i]][x].prods.empty())
                    ll1table[g.nonTerminals[i]][x] = r;
                else
                    cout << "Error" << endl;
            }
    }
}

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
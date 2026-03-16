#include "parser.h"
#include <iostream>
#include <set>
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
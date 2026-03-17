#include "parser.h"
#include <iostream>
#include <set>
#include <iomanip>
#include <sstream>
#include "first_follow.h"
using namespace std;

///////////////////////////////////
//                               //
//         PARSING TABLE         //
//                               //
///////////////////////////////////

map<string, map<string, GrammarRule>> ll1table;

void createParseTable(map<string, set<string>>& firs, map<string, set<string>>& follow, Grammar& g) {
    for (int i = 0; i < g.nonTerminals.size(); i++) {
        ll1table[g.nonTerminals[i]];
        GrammarRule r;
        for (int j = 0; j < g.terminals.size(); j++) {
            ll1table[g.nonTerminals[i]][g.terminals[j]] = r;
        }
        ll1table[g.nonTerminals[i]]["$"] = r;
    }
    g.terminals.push_back("$");
    for (int i = 0; i < g.nonTerminals.size(); i++) {
        for (auto prod : g.rules[g.nonTerminals[i]].prods) {
            map<string, set<string>> first;
            computeFirstOfNTll1TableEdition(first, g.nonTerminals[i], g, prod);
            for (auto x : first[g.nonTerminals[i]])
                if (isEpsilon(x)) {
                    for (auto y : follow[g.nonTerminals[i]]) {
                        GrammarRule r;
                        r.prods.push_back(prod);
                        r.nonTerminal = g.nonTerminals[i];
                        if (ll1table[g.nonTerminals[i]][y].prods.empty())
                            ll1table[g.nonTerminals[i]][y] = r;
                        else
                            cout << "Error" << endl;
                    }
                }
                else {
                    GrammarRule r;
                    r.prods.push_back(prod);
                    r.nonTerminal = g.nonTerminals[i];
                    if (ll1table[g.nonTerminals[i]][x].prods.empty())
                        ll1table[g.nonTerminals[i]][x] = r;
                    else
                        cout << "Error" << endl;
                }
        }
    }
}

void printParseTable(Grammar& g) {

    const int COL_WIDTH = 15;

    cout << "\n\n========= LL(1) PARSE TABLE =========\n\n";

    // Header
    cout << setw(COL_WIDTH) << " ";
    for (const auto& t : g.terminals) {
        cout << setw(COL_WIDTH) << t;
    }
    cout << endl;

    // Line
    cout << string(COL_WIDTH * (g.terminals.size() + 1), '-') << endl;

    // Rows
    for (const auto& nt : g.nonTerminals) {
        cout << setw(COL_WIDTH) << nt;

        for (const auto& t : g.terminals) {
            stringstream cellStream;
            auto& productions = ll1table[nt][t].prods;

            if (productions.empty()) {
                cellStream << "-";
            }
            else {
                for (int i = 0; i < productions.size(); i++) {
                    cellStream << nt << "->";
                    for (const auto& sym : productions[i].symbols) {
                        cellStream << sym;
                    }
                    if (i != productions.size() - 1)
                        cellStream << " | ";
                }
            }

            string cell = cellStream.str();

            // Trim if too long
            if (cell.length() > COL_WIDTH - 2) {
                cell = cell.substr(0, COL_WIDTH - 5) + "...";
            }

            cout << setw(COL_WIDTH) << cell;
        }
        cout << endl;
    }
}

////////////////////////////////////
//                                //
//         STRING PARSING         //
//                                //
////////////////////////////////////



////////////////////////////////////
//                                //
//         ERROR HANDLING         //
//                                //
////////////////////////////////////
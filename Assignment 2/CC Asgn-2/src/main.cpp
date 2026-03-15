#include <iostream>
#include "grammar.h"
#include "left_factoring.h"
//#include "left_recursion.h"
// #include "first_follow.h"
// #include "parser.h"

using namespace std;

void separateAlphaBetaProductions(vector<Production>& alphaProds, vector<Production>& betaProds, GrammarRule r) {
    for (int j = 0; j < r.prods.size(); j++) {
        Production p = r.prods[j];
        if (!p.symbols.empty() && r.nonTerminal == p.symbols[0]) {    // if left symbol equals the NT production Symbol
            alphaProds.push_back(p);
        }
        else {
            betaProds.push_back(p);
        }
    }
}

void copyData(vector<Production>& betaProds, GrammarRule& rc) {
    for (int i = 0; i < betaProds.size(); i++) {
        rc.prods.push_back(betaProds[i]);
    }
}

void handleBetaTerms(vector<Production>& betaProds, string newNT) {
    if (betaProds.empty()) {
        Production p;
        p.symbols.push_back(" ");
        betaProds.push_back(p);
    }
    for (int i = 0; i < betaProds.size(); i++) {
        betaProds[i].symbols.push_back(newNT);
    }
}

void handleAlphaTerms(vector<Production>& alphaProds, vector<Production>& newAlpProd, string newNT) {
    for (int i = 0; i < alphaProds.size(); i++) {
        Production p;
        for (int j = 1;j < alphaProds[i].symbols.size(); j++) {
            p.symbols.push_back(alphaProds[i].symbols[j]);
        }
        if (p.symbols.empty()) {
            continue;
        }
        p.symbols.push_back(newNT);
        newAlpProd.push_back(p);
    }
    Production p;
    p.symbols.push_back("epsilon");
    newAlpProd.push_back(p);
}

bool containsEpsilon(Grammar *g) {
    bool exists = 0;
    for (int i = 0; i < g->terminals.size();i++) {
        if (g->terminals[i] == "epsilon" || g->terminals[i] == "@") {
            return 1;
        }
    }
    return 0;
}

void solveRecursion(vector<Production>& alphaProds, vector<Production>& betaProds, GrammarRule& rc, Grammar*& g) {
    if (alphaProds.empty()) {       // Means no Left Recursive production exists
        copyData(betaProds, rc);
        g->rules.insert({ rc.nonTerminal,rc });
    }
    else {                          // Means Left Recursive production exists
        string newNT = "NEW"+rc.nonTerminal;

        handleBetaTerms(betaProds, newNT);
        vector<Production> newAlpProd;
        handleAlphaTerms(alphaProds, newAlpProd, newNT);

        // Edge Case: If Grammar already contains Epsilon, do not push again
        if (!containsEpsilon(g))
            g->terminals.push_back("epsilon");

        // Pushing new Grammar rules to the Grammar
        GrammarRule r1, r2;
        r1.nonTerminal = rc.nonTerminal;
        copyData(betaProds, r1);
        g->rules.insert({ r1.nonTerminal, r1 });

        r2.nonTerminal = newNT;
        copyData(newAlpProd, r2);
        g->rules.insert({r2.nonTerminal, r2});

        g->nonTerminals.push_back(newNT);
    }
}

Grammar* eliminateRecursion(Grammar& d) {
    Grammar *g = new Grammar();         // This new object will contain removed left recursive grammar
    g->terminals = d.terminals;
    g->startSymbol = d.startSymbol;

    for (int i = 0; i < d.nonTerminals.size(); i++)
    {
        g->nonTerminals.push_back(d.nonTerminals[i]);
        GrammarRule r = d.rules[d.nonTerminals[i]];
        GrammarRule rc;
        rc.nonTerminal = d.nonTerminals[i];

        // Separates alpha and beta productions
        vector<Production> alphaProds, betaProds;
        separateAlphaBetaProductions(alphaProds, betaProds, r);

        // Solves Recursion if exists
        solveRecursion(alphaProds, betaProds, rc, g);
    }
    return g;
}

int main() {
    // load grammar
    Grammar g = loadGrammar("grammar1.txt");
    cout << "=== Original Grammar ===\n";
    printGrammar(g);

    Grammar* gg = eliminateRecursion(g);
    printGrammar(*gg);

    
    // Left Factoring
    cout << "=== Left Factored Grammar ===\n";
    leftFactor(*gg);
    printGrammar(*gg);
    return 0;
}
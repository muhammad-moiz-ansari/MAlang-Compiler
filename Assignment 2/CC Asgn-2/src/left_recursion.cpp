#include "left_recursion.h"

void separateAlphaBetaProductions(vector<Production>& alphaProds, vector<Production>& betaProds, GrammarRule r) {
    for (int j = 0; j < (int)r.prods.size(); j++) {
        Production p = r.prods[j];
        if (!p.symbols.empty() && r.nonTerminal == p.symbols[0])
            alphaProds.push_back(p);
        else
            betaProds.push_back(p);
    }
}

void copyData(vector<Production>& betaProds, GrammarRule& rc) {
    for (int i = 0; i < (int)betaProds.size(); i++)
        rc.prods.push_back(betaProds[i]);
}

void handleBetaTerms(vector<Production>& betaProds, string newNT) {
    if (betaProds.empty()) {
        Production p;
        p.symbols.push_back("epsilon");  // ✅ fixed: was " "
        betaProds.push_back(p);
    }
    for (int i = 0; i < (int)betaProds.size(); i++)
        betaProds[i].symbols.push_back(newNT);
}

void handleAlphaTerms(vector<Production>& alphaProds, vector<Production>& newAlpProd, string newNT) {
    for (int i = 0; i < (int)alphaProds.size(); i++) {
        Production p;
        for (int j = 1; j < (int)alphaProds[i].symbols.size(); j++)
            p.symbols.push_back(alphaProds[i].symbols[j]);
        if (p.symbols.empty()) continue;
        p.symbols.push_back(newNT);
        newAlpProd.push_back(p);
    }
    Production p;
    p.symbols.push_back("epsilon");
    newAlpProd.push_back(p);
}

bool containsEpsilon(Grammar* g) {
    for (int i = 0; i < (int)g->terminals.size(); i++)
        if (g->terminals[i] == "epsilon" || g->terminals[i] == "@")
            return true;
    return false;
}

void solveRecursion(vector<Production>& alphaProds, vector<Production>& betaProds, GrammarRule& rc, Grammar*& g) {
    if (alphaProds.empty()) {
        copyData(betaProds, rc);
        g->rules[rc.nonTerminal] = rc;  // ✅ fixed: was insert()
    }
    else {
        string newNT = "NEW" + rc.nonTerminal;

        handleBetaTerms(betaProds, newNT);
        vector<Production> newAlpProd;
        handleAlphaTerms(alphaProds, newAlpProd, newNT);

        if (!containsEpsilon(g))
            g->terminals.push_back("epsilon");

        GrammarRule r1, r2;
        r1.nonTerminal = rc.nonTerminal;
        copyData(betaProds, r1);
        g->rules[r1.nonTerminal] = r1;  // ✅ fixed: was insert()

        r2.nonTerminal = newNT;
        copyData(newAlpProd, r2);
        g->rules[r2.nonTerminal] = r2;  // ✅ fixed: was insert()

        g->nonTerminals.push_back(newNT);
    }
}

void substitute(GrammarRule& rc, vector<Production>& betaProds, vector<Production>& p, int i) {
    for (int k = 0; k < (int)rc.prods.size(); k++) {
        Production pp;
        for (int l = 0; l < (int)rc.prods[k].symbols.size(); l++)
            pp.symbols.push_back(rc.prods[k].symbols[l]);
        for (int m = 1; m < (int)betaProds[i].symbols.size(); m++)  // ✅ fixed: was k
            pp.symbols.push_back(betaProds[i].symbols[m]);
        p.push_back(pp);
    }
}

void removeExtraTerms(vector<Production>& betaProds, vector<int>& indices) {
    vector<Production> temp;
    for (int i = 0; i < (int)betaProds.size(); i++) {
        bool found = false;
        for (int j = 0; j < (int)indices.size(); j++) {
            if (i == indices[j]) { found = true; break; }
        }
        if (!found) temp.push_back(betaProds[i]);
    }
    betaProds = temp;  // simpler than manual pop_back loop
}

void RemoveIndirectLeftRecursion(Grammar& r, vector<Production>& betaProds, string nt) {
    if (betaProds.empty()) return;

    vector<Production> p;
    vector<int> indices;

    for (int i = 0; i < (int)betaProds.size(); i++) {
        string firstSym = betaProds[i].symbols[0];
        if (!isNonTerminal(firstSym)) continue;  // ✅ fixed: skip terminals
        GrammarRule rc = r.rules[firstSym];
        if (rc.nonTerminal.empty() || rc.nonTerminal == nt) continue;
        for (int j = 0; j < (int)rc.prods.size(); j++) {
            if (!rc.prods[j].symbols.empty() && rc.prods[j].symbols[0] == nt) {
                substitute(rc, betaProds, p, i);
                indices.push_back(i);
                break;
            }
        }
    }
    removeExtraTerms(betaProds, indices);
    for (int k = 0; k < (int)p.size(); k++)
        betaProds.push_back(p[k]);
}

Grammar* eliminateRecursion(Grammar& d) {
    Grammar* g = new Grammar();
    g->terminals = d.terminals;
    g->startSymbol = d.startSymbol;

    for (int i = 0; i < (int)d.nonTerminals.size(); i++) {
        g->nonTerminals.push_back(d.nonTerminals[i]);
        GrammarRule r = d.rules[d.nonTerminals[i]];
        GrammarRule rc;
        rc.nonTerminal = d.nonTerminals[i];

        RemoveIndirectLeftRecursion(d, r.prods, d.nonTerminals[i]);

        vector<Production> alphaProds, betaProds;
        separateAlphaBetaProductions(alphaProds, betaProds, r);

        solveRecursion(alphaProds, betaProds, rc, g);
        d.rules[d.nonTerminals[i]] = g->rules[d.nonTerminals[i]];
    }
    return g;
}
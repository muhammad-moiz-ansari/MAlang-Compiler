#pragma once
#include "grammar.h"

// ── Function Declarations ─────────────────────────────────

void separateAlphaBetaProductions(vector<Production>& alphaProds, vector<Production>& betaProds, GrammarRule r);

void copyData(vector<Production>& betaProds, GrammarRule& rc);

void handleBetaTerms(vector<Production>& betaProds, string newNT);

void handleAlphaTerms(vector<Production>& alphaProds, vector<Production>& newAlpProd, string newNT);

bool containsEpsilon(Grammar* g);

void solveRecursion(vector<Production>& alphaProds, vector<Production>& betaProds, GrammarRule& rc, Grammar*& g);

void substitute(GrammarRule& rc, vector<Production>& betaProds, vector<Production>& p, int i);

void removeExtraTerms(vector<Production>& betaProds, vector<int>& indices);

void RemoveIndirectLeftRecursion(Grammar r, vector<Production>& betaProds, string nt);

Grammar* eliminateRecursion(Grammar& d);
#pragma once

//#ifndef GRAMMAR_H
//#define GRAMMAR_H

#include <string>
#include <vector>
#include <map>
using namespace std;

// ── Data Structures ──────────────────────────────────────

struct Production {
    vector<string> symbols;  // e.g. ["Term", "ExprPrime"]
};

struct GrammarRule {
    string nonTerminal;
    vector<Production> prods;
};

struct Grammar {
    vector<string> nonTerminals;  // ordered, first = start symbol
    vector<string> terminals;
    map<string, GrammarRule> rules;
    string startSymbol;
};

// ── Function Declarations ─────────────────────────────────

Grammar loadGrammar(const string& filename);   // reads grammar.txt
void    printGrammar(const Grammar& g);        // for debugging
bool    isNonTerminal(const string& symbol);   // uppercase first char?
bool    isTerminal(const string& symbol);      // lowercase first char, digit, or punctuation (except @)
bool    isEpsilon(const string& symbol);       // "epsilon" or "@"
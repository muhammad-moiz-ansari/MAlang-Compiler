#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

// ── Data Structures ──────────────────────────────────────

struct Production {
    vector<string> symbols;  // e.g. ["Term", "ExprPrime"]
};

struct GrammarRule {
    string nonTerminal;         // L.H.S of arrow (->)
    vector<Production> prods;   // R.H.S of arrow (->)
};

struct Grammar {
    vector<string> nonTerminals;  // ordered, first = start symbol
    vector<string> terminals;
    map<string, GrammarRule> rules;
    string startSymbol;
};

// ── Function Declarations ─────────────────────────────────

bool isNonTerminal(const string& symbol);   // uppercase first char?
bool isTerminal(const string& symbol);      // lowercase first char, digit, or punctuation (except @)
bool isEpsilon(const string& symbol);       // "epsilon" or "@"
GrammarRule parseLine(const string& line);
Grammar loadGrammar(const string& filename);   // reads grammar.txt
void printGrammar(const Grammar& g);        // for debugging
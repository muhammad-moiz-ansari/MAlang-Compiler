#pragma once
#include "grammar.h"
#include <set>

// ── Function Declarations ─────────────────────────────────

//////////////////////////
//                      //
//       FIRST ()       //
//                      //
//////////////////////////

// Computes FIRST set for a single symbol (terminal or non-terminal)
set<string> computeFirstOfSymbol(const string& symbol, const Grammar& g, map<string, set<string>>& FIRST);

// Computes FIRST set for a sequence of symbols (e.g. a whole production)
set<string> computeFirstOfString(const vector<string>& symbols, const Grammar& g, map<string, set<string>>& FIRST);

// Computes FIRST set for a non-terminal
void computeFirstOfNT(map<string, set<string>>& first, string nt, const Grammar& g);

void computeFirstOfNTll1TableEdition(map<string, set<string>>& first, string nt, const Grammar& g, Production& prod);

// Computes FIRST sets for ALL non-terminals in grammar
map<string, set<string>> computeFirst(const Grammar& g);

// Prints FIRST sets in tabular format
void printFirst(const map<string, set<string>>& FIRST, const Grammar& g);

///////////////////////////
//                       //
//       FOLLOW ()       //
//                       //
///////////////////////////

bool isStart(Grammar g, string cs);

string getNextSymbol(vector<string> symbols, int& i);

set<string> getFirstOfNT(string ns, map<string, set<string>>& first);

// Computes FOLLOW sets for ALL non-terminals in grammar
map<string, set<string>> computeFollow(Grammar& g, map<string, set<string>>& FIRST);

// Prints FOLLOW sets in tabular format
void printFollow(const map<string, set<string>>& FIRST, const map<string, set<string>>& FOLLOW, const Grammar& g);



// ----- Save first() and follow() -----
void saveFirstFollow(const map<string, set<string>>& FIRST, const map<string, set<string>>& FOLLOW, const Grammar& g, const string& filename = "output/first_follow_sets.txt");
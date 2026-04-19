#include "grammar_augment.h"
#include <iostream>
using namespace std;

Grammar augmentGrammar(const Grammar& g) {
	Grammar augmented = g;
	string newStartSymbol = g.nonTerminals[0] + "'";
	
	// Create new start rule
	GrammarRule newStartRule;
	newStartRule.nonTerminal = newStartSymbol;
	Production prod;
	prod.symbols.push_back(g.nonTerminals[0]);
	newStartRule.prods.push_back(prod);
	
	// Add the new start rule to the grammar
	augmented.rules[newStartSymbol] = newStartRule;
	augmented.nonTerminals.insert(augmented.nonTerminals.begin(), newStartSymbol);
	augmented.startSymbol = newStartSymbol;
	
	return augmented;
}
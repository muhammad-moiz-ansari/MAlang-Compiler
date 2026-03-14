#include "grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

bool isNonTerminal(const string& symbol) {
	return !symbol.empty() && isupper(symbol[0]);
}

bool isTerminal(const string& symbol) {
	return !symbol.empty() && !isNonTerminal(symbol) && !isEpsilon(symbol);
}

bool isEpsilon(const string& symbol) {
	return (symbol == "@" || symbol == "epsilon");
}

GrammarRule parseLine(const string& line) {
	GrammarRule rule;
	stringstream ss(line);
	string token;
	
	ss >> rule.nonTerminal;		// Non-terminal (LHS)
	ss >> token;				// The "->" symbol

	// Productions (RHS)
	Production prod;
	while (ss >> token) {
		if (token == "|") {
			rule.prods.push_back(prod);
			prod = Production();  // start fresh
		}
		else
			prod.symbols.push_back(token);
	}
	if (!prod.symbols.empty())
		rule.prods.push_back(prod);  // push last production

	return rule;
}

Grammar loadGrammar(const string& filename) {
	Grammar g;
	string line;
	bool isStartSymbol = true;
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Error: could not open file: " << filename << endl;
		exit(1);
	}

	while (getline(file, line)) {
		if (line.empty())
			continue;

		GrammarRule rule = parseLine(line);
		g.rules[rule.nonTerminal] = rule;
		g.nonTerminals.push_back(rule.nonTerminal);

		if (isStartSymbol) {
			g.startSymbol = rule.nonTerminal;
			isStartSymbol = false;
		}
	}

	// Collecting all non-terminals
	for (const auto& nt : g.nonTerminals) {
		auto& rule = g.rules.at(nt);
		for (int i = 0; i < rule.prods.size(); ++i) {
			for (int j = 0; j < rule.prods[i].symbols.size(); ++j) {
				auto symbol = rule.prods[i].symbols[j];
				// Add the ternimal to list if doesnt already exists
				if (isTerminal(symbol)) {
					if (find(g.terminals.begin(), g.terminals.end(), symbol) == g.terminals.end())
						g.terminals.push_back(symbol);
				}
			}
		}
	}
	return g;
}

void printGrammar(const Grammar& g) {
	// For each non-terminal
	for (const auto& nonT : g.nonTerminals) {
		const auto& rule = g.rules.at(nonT);
		cout << nonT << " -> ";

		// For each production of that non-terminal
		for (int i = 0; i < rule.prods.size(); ++i) {
			const auto& prod = rule.prods[i];

			// For each symbol of that production
			for (const auto& symbol : prod.symbols) {
				cout << symbol << " ";
			}
			if (i != rule.prods.size() - 1)
				cout << "| ";
		}
		cout << endl;
	}
	cout << endl;
}
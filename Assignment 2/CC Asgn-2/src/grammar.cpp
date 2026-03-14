#include "grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

bool isNonTerminal(const string& symbol) {
	if(!symbol.empty() && isupper(symbol[0]))
		return true;
}

bool isTerminal(const string& symbol) {
	if(!symbol.empty() && (islower(symbol[0]) || isdigit(symbol[0]) || ispunct(symbol[0])) && (symbol != "@" && symbol != "epsilon"))
		return true;
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
	while (ss >> token) {
		Production prod;
		prod.symbols.push_back(token);

		// Read until "|" or end of line comes
		while (ss.peek() != '|' && ss.peek() != EOF) {
			ss >> token;
			prod.symbols.push_back(token);
		}
		rule.prods.push_back(prod);
		// Skip the separator "|"
		if (ss.peek() == '|') {
			ss.get(); // Consume
		}
	}
	return rule;
}

Grammar loadGrammar(const string& filename) {
	Grammar g;
	ifstream file(filename);
	string line;
	bool isStartSymbol = true;

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
	for (auto& pair : g.rules) {
		auto rule = pair.second;
		for (int i = 0; i < rule.prods.size(); ++i) {
			for (int j = 0; j < rule.prods[i].symbols.size(); ++j) {
				auto symbol = rule.prods[i].symbols[j];
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
}
#include "first_follow.h"
#include <iostream>
using namespace std;

set<string> computeFirstOfSymbol(const string& symbol, const Grammar& g, map<string, set<string>>& first)
{
	if (isEpsilon(symbol) || isTerminal(symbol))
		return { symbol };
	else	// if non-terminal
	{
		auto it = first.find(symbol);
		if (it == first.end() || (it != first.end() && !first[symbol].empty())) // First set doesnt exist for nt
			computeFirstOfNT(first, symbol, g);
		return first.at(symbol);
	}
}

set<string> computeFirstOfString(const vector<string>& symbols, const Grammar& g, map<string, set<string>>& first)
{
	set<string> retSet;
	bool prevHasEp = false;

	// If prod empty or only epsilon exists
	if (symbols.empty() || (symbols.size() == 1 && isEpsilon(symbols[0]))) {
		retSet.insert("epsilon");
		return retSet;
	}

	for (int i = 0; i < symbols.size(); ++i) {
		if (i != 0 && !prevHasEp)
			break;
		set<string> tempSet = computeFirstOfSymbol(symbols[i], g, first);

		prevHasEp = false;
		// Add everything except epsilon
		for (const auto& s : tempSet) {
			if (s != "epsilon")
				retSet.insert(s);
			else
				prevHasEp = true;
		}

		// If last symbol can derive epsilon
		if (i == symbols.size() - 1 && prevHasEp)
			retSet.insert("epsilon");
	}
	return retSet;
}

void computeFirstOfNT(map<string, set<string>>& first, string nt, const Grammar& g) {
	const auto& rule = g.rules.at(nt);
	for (const auto& prod : rule.prods) {
		set<string> tempSet = computeFirstOfString(prod.symbols, g, first);
		first[nt].insert(tempSet.begin(), tempSet.end());
	}
}

map<string, set<string>> computeFirst(const Grammar& g)
{
	map<string, set<string>> first;
	for (const auto& nt : g.nonTerminals) {
		auto it = first.find(nt);
		if (it == first.end() || (it != first.end() && first[nt].empty())) // First set doesnt exist for nt
		{
			computeFirstOfNT(first, nt, g);
		}
	}
	return first;
}

void printFirst(const map<string, set<string>>& FIRST, const map<string, set<string>>& FOLLOW, const Grammar& g)
{
	cout << "--- First() Sets ---\n";
	for (const auto& nt : g.nonTerminals) {
		cout << nt << ":\t{";
		for (const auto& s : FIRST.at(nt)) {
			cout << " " << s << ",";
		}
		cout << "\b }\n";
	}
	cout << endl;
}


//-------------------LET THE PARTY STARTED--------------------

bool isStart(Grammar g, string cs) {
	GrammarRule r = g.rules[cs];
	if (r.nonTerminal == g.startSymbol)
		return 1;
	return 0;
}

string getNextSymbol(vector<string> symbols, int& i) {
	if (i + 1 < symbols.size()) {
		string retval = symbols[i + 1];
		i++;
		return retval;
	}
	return "-1";
}

set<string> getFirstOfNT(string ns, map<string, set<string>>& first) {
	return first[ns];
}

map<string, set<string>> computeFollow(Grammar& g, map<string, set<string>>& FIRST)
{
	map<string, set<string>> follow;

	for (int i = 0; i < g.nonTerminals.size(); i++) {
		follow[g.nonTerminals[i]];
	}
	bool c = true;
	while(c) {
		c = false;
		for (int i = 0; i < g.nonTerminals.size(); i++) {
			string currentSymbol = g.nonTerminals[i];
			if (isStart(g, currentSymbol))
				follow[g.nonTerminals[i]].insert("$");

			for (int l = 0; l < g.nonTerminals.size(); l++) {
				GrammarRule r = g.rules[g.nonTerminals[l]];
				set<string> prev;
				prev = follow[g.nonTerminals[i]];
				for (int j = 0; j < r.prods.size();j++) {
					for (int k = 0; k < r.prods[j].symbols.size(); k++) {
						if (r.prods[j].symbols[k] == currentSymbol) {
							int m = k;
							while (1) {
								string ns = getNextSymbol(r.prods[j].symbols, m);
								if (ns == "-1") {
									// Do not stuck in loop if S -> ASB situation arises for S-> follow(S)
									if (r.nonTerminal == currentSymbol)
										break;
									// Appending follow(parent NT) to current follow
									for (auto x : follow[r.nonTerminal])
										follow[g.nonTerminals[i]].insert(x);
									break;
								}
								else if (isTerminal(ns)) {
									follow[g.nonTerminals[i]].insert(ns);
									break;
								}
								else if (isNonTerminal(ns)) {
									set<string> f = getFirstOfNT(ns, FIRST);
									bool e = 0;
									for (auto x : f) {
										if (isEpsilon(x)) { e = 1; continue; }
										follow[g.nonTerminals[i]].insert(x);
									}
									if (e == 0)
										break;
								}
							}
							
						}
					}
					if (follow[g.nonTerminals[i]] == prev)
						c = c;
					else
						c = true;
				}
			}
		}
	}

	return follow;
}

void printFollow(const map<string, set<string>>& FIRST, const map<string, set<string>>& FOLLOW, const Grammar& g)
{
	cout << "--- Follow() Sets ---\n";
	for (const auto& nt : g.nonTerminals) {
		cout << nt << ":\t{";
		for (const auto& s : FOLLOW.at(nt)) {
			cout << " " << s << ",";
		}
		cout << "\b}\n";
	}
	cout << endl;
}
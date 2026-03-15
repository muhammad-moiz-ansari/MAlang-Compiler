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

map<string, set<string>> computeFollow(const Grammar& g, map<string, set<string>>& FIRST)
{
	map<string, set<string>> follow;



	return follow;
}

void printFirstFollow(const map<string, set<string>>& FIRST, const map<string, set<string>>& FOLLOW, const Grammar& g)
{
	cout << "--- Fisrt() Sets ---\n";
	for (const auto& nt : g.nonTerminals) {
		cout << nt << ":\t{";
		for (const auto& s : FIRST.at(nt)) {
			cout << " " << s << ",";
		}
		cout << "\b }\n";
	}
	cout << endl;
}
#include "left_factoring.h"
#include <algorithm>

void leftFactor(Grammar& g) {
	bool hasChanged = true;

	while(hasChanged) {
		hasChanged = false;
		vector<string> currentNTs = g.nonTerminals;		// Copying non-terminals so that iterator doesnt crash when this vector is updated

		// For each non-terminal
		for (const auto& nt : currentNTs) {
			auto& rule = g.rules.at(nt);
			sort(rule.prods.begin(), rule.prods.end(), [](const Production& a, const Production& b) {
				if (a.symbols.empty()) return true;
				if (b.symbols.empty()) return false;
				return a.symbols[0] < b.symbols[0];
			});

			// For each production of that non-terminal
			for (int i = 0; i < (int)rule.prods.size() - 1 ; ++i) {
				if (rule.prods[i].symbols[0] == rule.prods[i + 1].symbols[0]) {
					hasChanged = true;
					string newNT = nt + "'";
					while (g.rules.count(newNT)) newNT += "'";		//
					string matchingSym = rule.prods[i].symbols[0];
					g.nonTerminals.push_back(newNT);
					// New Rule
					GrammarRule newRule;
					newRule.nonTerminal = newNT;
					for (int j = 0; j < 2; ++j) {
						Production p = rule.prods[i];
						p.symbols.erase(p.symbols.begin() + 0);
						if (p.symbols.empty())
							p.symbols.push_back("epsilon");
						newRule.prods.push_back(p);
						rule.prods.erase(rule.prods.begin() + i);
					}
					// Checking if any other matching prefix
					for (; i < (int)rule.prods.size() - 1; ++i) {
						if (rule.prods[i].symbols[0] == matchingSym) {
							Production p = rule.prods[i];
							p.symbols.erase(p.symbols.begin() + 0);
							newRule.prods.push_back(p);
							rule.prods.erase(rule.prods.begin() + i);
							--i;
						}
					}
					Production newProd;
					newProd.symbols.push_back(matchingSym);
					newProd.symbols.push_back(newNT);
					rule.prods.push_back(newProd);
					g.rules[newRule.nonTerminal] = newRule;
				}
			}
		}
		//printGrammar(g);
	}
}
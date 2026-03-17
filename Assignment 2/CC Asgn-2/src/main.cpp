#include <iostream>
#include "grammar.h"
#include "left_factoring.h"
#include "left_recursion.h"
#include "first_follow.h"
#include "parser.h"
#include <iomanip>
#include <sstream>
#include <stack>
using namespace std;
#include <algorithm>

// ANSI color codes (work in most terminals)
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"

bool isTerminal(string sym, Grammar& g) {
    return isTerminal(sym);
}
struct ParseTreeNode {
    string symbol;
    vector<ParseTreeNode*> children;

    ParseTreeNode(string s) : symbol(s) {}
};
ParseTreeNode* buildParseTree(
    Grammar& g,
    map<string, map<string, GrammarRule>>& table,
    vector<string> input
) {
    stack<pair<string, ParseTreeNode*>> st;

    // Root
    ParseTreeNode* root = new ParseTreeNode(g.startSymbol);

    // Push $ and start symbol
    st.push({ "$", nullptr });
    st.push({ g.startSymbol, root });

    input.push_back("$");
    int ip = 0;

    while (!st.empty()) {
        auto top = st.top();
        st.pop();

        string X = top.first;
        ParseTreeNode* node = top.second;
        string a = input[ip];

        // Terminal or $
        if (X == a) {
            ip++; // match
        }
        else if (find(g.terminals.begin(), g.terminals.end(), X) != g.terminals.end() || X == "$") {
            cout << "ERROR: Unexpected token " << a << endl;
            return nullptr;
        }
        else {
            // Non-terminal
            auto& rule = table[X][a];

            if (rule.prods.empty()) {
                cout << "ERROR: No rule for (" << X << ", " << a << ")\n";
                return nullptr;
            }

            // Take first production (LL(1))
            auto& prod = rule.prods[0];

            vector<ParseTreeNode*> children;

            // Create children nodes
            for (auto& sym : prod.symbols) {
                ParseTreeNode* child = new ParseTreeNode(sym);
                children.push_back(child);
            }

            // Attach to tree
            node->children = children;

            // Push in reverse order
            for (int i = children.size() - 1; i >= 0; i--) {
                if (children[i]->symbol != "epsilon") {
                    st.push({ children[i]->symbol, children[i] });
                }
            }
        }
    }

    return root;
}

void printParseTree(ParseTreeNode* node, int depth = 0) {
    if (!node) return;

    for (int i = 0; i < depth; i++)
        cout << "  ";

    cout << node->symbol << endl;

    for (auto child : node->children) {
        printParseTree(child, depth + 1);
    }
}
void printTreeASCII(ParseTreeNode* node, string prefix = "", bool isLast = true) {
    if (!node) return;

    cout << prefix;

    if (isLast) {
        cout << "\\-- ";
        prefix += "    ";
    }
    else {
        cout << "|-- ";
        prefix += "|   ";
    }

    cout << node->symbol << endl;

    for (int i = 0; i < node->children.size(); i++) {
        printTreeASCII(node->children[i], prefix, i == node->children.size() - 1);
    }
}
void printTreeColored(ParseTreeNode* node, Grammar& g, string prefix = "", bool isLast = true) {
    if (!node) return;

    cout << prefix;

    if (isLast) {
        cout << "\\-- ";
        prefix += "    ";
    }
    else {
        cout << "|-- ";
        prefix += "|   ";
    }

    // Color logic
    if (node->symbol == "epsilon") {
        cout << YELLOW << node->symbol << RESET << endl;
    }
    else if (isTerminal(node->symbol, g)) {
        cout << BLUE << node->symbol << RESET << endl;
    }
    else {
        cout << GREEN << node->symbol << RESET << endl;
    }

    for (int i = 0; i < node->children.size(); i++) {
        printTreeColored(node->children[i], g, prefix, i == node->children.size() - 1);
    }
}

map<string, map<string, GrammarRule>> ll1table;

void createParseTable(map<string, set<string>>& firs, map<string, set<string>>& follow, Grammar& g) {
    for (int i = 0; i < g.nonTerminals.size(); i++) {
        ll1table[g.nonTerminals[i]];
        GrammarRule r;
        for (int j = 0; j < g.terminals.size(); j++) {
            ll1table[g.nonTerminals[i]][g.terminals[j]] = r;
        }
        ll1table[g.nonTerminals[i]]["$"] = r;
    }
    g.terminals.push_back("$");
    for (int i = 0; i < g.nonTerminals.size(); i++) {
        for (auto prod : g.rules[g.nonTerminals[i]].prods) {
            map<string, set<string>> first;
            computeFirstOfNTll1TableEdition(first, g.nonTerminals[i], g, prod);
            for (auto x : first[g.nonTerminals[i]])
                if (isEpsilon(x)) {
                    for (auto y : follow[g.nonTerminals[i]]) {
                        GrammarRule r;
                        r.prods.push_back(prod);
                        r.nonTerminal = g.nonTerminals[i];
                        if (ll1table[g.nonTerminals[i]][y].prods.empty())
                            ll1table[g.nonTerminals[i]][y] = r;
                        else
                            cout << "Error" << endl;
                    }
                }
                else {
                    GrammarRule r;
                    r.prods.push_back(prod);
                    r.nonTerminal = g.nonTerminals[i];
                    if (ll1table[g.nonTerminals[i]][x].prods.empty())
                        ll1table[g.nonTerminals[i]][x] = r;
                    else
                        cout << "Error" << endl;
                }
        }
    }
}

void print(Grammar& g) {

    const int COL_WIDTH = 15;

    cout << "\n\n========= LL(1) PARSE TABLE =========\n\n";

    // Header
    cout << setw(COL_WIDTH) << " ";
    for (const auto& t : g.terminals) {
        cout << setw(COL_WIDTH) << t;
    }
    cout << endl;

    // Line
    cout << string(COL_WIDTH * (g.terminals.size() + 1), '-') << endl;

    // Rows
    for (const auto& nt : g.nonTerminals) {
        cout << setw(COL_WIDTH) << nt;

        for (const auto& t : g.terminals) {
            stringstream cellStream;
            auto& productions = ll1table[nt][t].prods;

            if (productions.empty()) {
                cellStream << "-";
            }
            else {
                for (int i = 0; i < productions.size(); i++) {
                    cellStream << nt << "->";
                    for (const auto& sym : productions[i].symbols) {
                        cellStream << sym;
                    }
                    if (i != productions.size() - 1)
                        cellStream << " | ";
                }
            }

            string cell = cellStream.str();

            // Trim if too long
            if (cell.length() > COL_WIDTH - 2) {
                cell = cell.substr(0, COL_WIDTH - 5) + "...";
            }

            cout << setw(COL_WIDTH) << cell;
        }
        cout << endl;
    }
}

int main() {
    // load grammar
    Grammar gr = loadGrammar("input/grammar1.txt");
    cout << "=== Original Grammar ===\n";
    printGrammar(gr);

    /*
    for (int i = 1;i <= 5;i++) {
        if (i == 1)
            cout << "A -> Aa | B Test" << endl << endl;
        if (i == 2)
            cout << "A -> Xa | B, X -> Ac Test" << endl << endl;
        if (i == 3)
            cout << "A -> Aa Test" << endl << endl;
        if (i == 4)
            cout << "A -> A | B Test" << endl << endl;
        if (i == 5)
            cout << "A -> A Test" << endl << endl;

        Grammar g = loadGrammar("input/left_Recursion_Test" + to_string(i) + ".txt");
        Grammar* gg = eliminateRecursion(g);
        printGrammar(*gg);
        cout << endl <<"-----------------------------" << endl;
    }
    */

    // Left Recursion Removal
    cout << "=== Left Recursion Removed Grammar ===\n";
    Grammar g = *eliminateRecursion(gr);
    printGrammar(g);
    
    // Left Factoring
    cout << "=== Left Factored Grammar ===\n";
    leftFactor(g);
    printGrammar(g);

    cout << "=== First & Follow Sets ===\n";

    // First Sets
    map<string, set<string>> first = computeFirst(g);
    
    // Follow Sets
    map<string, set<string>> follow = computeFollow(g, first);

    printFirst(first, follow, g);
    printFollow(first, follow, g);
    createParseTable(first, follow, g);

    /*
    cout << endl;
    for (int i = 0; i < g.nonTerminals.size();i++) {
        cout << g.nonTerminals[i] << "     ";
        for (int j = 0; j < g.terminals.size();j++) {
            for (int k = 0; k < ll1table[g.nonTerminals[i]][g.terminals[j]].prods.size(); k++) {
                cout << g.terminals[j] << ":";
                for (int l = 0;l < ll1table[g.nonTerminals[i]][g.terminals[j]].prods[k].symbols.size();l++) {
                    cout << ll1table[g.nonTerminals[i]][g.terminals[j]].prods[k].symbols[l];
               }
                cout << "    ";
            }
        }
        cout << endl;
    }
    */
    cout << endl << endl;
    print(g);

    vector<string> input = { "id", "+", "id", "*", "id" };
    ParseTreeNode* root = buildParseTree(g, ll1table, input);

    if (root) {
        cout << "\n\n======= PARSE TREE (ASCII) =======\n\n";
        printTreeColored(root, g);
    }
    return 0;
}
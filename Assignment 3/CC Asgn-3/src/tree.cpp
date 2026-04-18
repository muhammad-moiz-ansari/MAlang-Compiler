#include "grammar.h"
#include "stack.h"
#include <iostream>
#include <fstream> // Included for ofstream


// ANSI color codes (work in most terminals)
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"

struct ParseTreeNode {
    string symbol;
    vector<ParseTreeNode*> children;

    ParseTreeNode(string s) : symbol(s) { }
};

bool isTerminal(string sym, Grammar& g) {
    return isTerminal(sym);
}


void deleteTree(ParseTreeNode* node) {
    if (!node) return;
    for (auto child : node->children) {
        deleteTree(child);
    }
    delete node;
}

ParseTreeNode* buildParseTree(
    Grammar& g,
    map<string, map<string, GrammarRule>>& table,
    vector<string> input
) {
    Stack<pair<string, ParseTreeNode*>> st;

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
            cout << "TREE ERROR: Unexpected token " << a << endl;
            deleteTree(root);
            return nullptr;
        }
        else {
            // Non-terminal
            auto& rule = table[X][a];

            if (rule.prods.empty()) {
                cout << "TREE ERROR: No rule for (" << X << ", " << a << ")\n";
                deleteTree(root);
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

void printParseTree(ParseTreeNode* node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++)
        cout << "  ";

    cout << node->symbol << endl;

    for (auto child : node->children) {
        printParseTree(child, depth + 1);
    }
}
void printTreeASCII(ParseTreeNode* node, string prefix, bool isLast = true) {
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

void printTreeColored(ParseTreeNode* node, Grammar& g, int trace_no, string prefix, bool isLast = true) {
    if (!node) return;

    // Use append mode to stream the parsed tree continuously across invocations
    ofstream outFile("output/parse_trees.txt", ios::app);
    if (!outFile.is_open()) {
        cout << "TREE ERROR: Unable to successfully open the parse_trees.txt file" << endl;
        return;
    }

    if (prefix.empty()) {
        outFile << "\n==============================\n";
        outFile << "Tree Number: " << trace_no << "\n";
        outFile << "==============================\n";
    }

    // Write structure block to terminal
    cout << prefix;
    
    // Write structure block to file
    outFile << prefix;

    if (isLast) {
        cout << "\\-- ";
        outFile << "\\-- ";
        prefix += "    ";
    }
    else {
        cout << "|-- ";
        outFile << "|-- ";
        prefix += "|   ";
    }

    // Color logic (Only printed to console, otherwise adds ANSI colors to file directly)
    if (node->symbol == "epsilon") {
        cout << YELLOW << node->symbol << RESET << endl;
    }
    else if (isTerminal(node->symbol, g)) {
        cout << BLUE << node->symbol << RESET << endl;
    }
    else {
        cout << GREEN << node->symbol << RESET << endl;
    }

    outFile << node->symbol << "\n";
    outFile.close();

    for (int i = 0; i < node->children.size(); i++) {
        printTreeColored(node->children[i], g, trace_no, prefix, i == node->children.size() - 1);
    }
}
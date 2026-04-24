#include "tree.h"
#include "grammar.h"
#include <iostream>
#include <fstream> // Included for ofstream

// ANSI color codes (work in most terminals)
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"


bool isTerminalForTree(const string& sym, const Grammar& g) {
    return isTerminal(sym);
}

void deleteTree(ParseTreeNode* node) {
    if (!node) return;
    for (auto child : node->children) {
        deleteTree(child);
    }
    delete node;
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

void printTreeASCII(ParseTreeNode* node, string prefix, bool isLast) {
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

void printTreeColored(ParseTreeNode* node, Grammar& g, int trace_no, string prefix, bool isLast) {
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
    else if (isTerminalForTree(node->symbol, g)) {
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
#pragma once
#include "grammar.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

// Node structure for the Parse Tree
struct ParseTreeNode {
    string symbol;
    vector<ParseTreeNode*> children;

    ParseTreeNode(string s) : symbol(s) {}
};

// Function prototypes
bool isTerminal(string sym, Grammar& g);

ParseTreeNode* buildParseTree(
    Grammar& g,
    map<string, map<string, GrammarRule>>& table,
    vector<string> input
);

void printParseTree(ParseTreeNode* node, int depth = 0);
void printTreeASCII(ParseTreeNode* node, string prefix = "", bool isLast = true);
void printTreeColored(ParseTreeNode* node, Grammar& g, int trace_no, string prefix = "", bool isLast = true);

// Memory cleanup 
void deleteTree(ParseTreeNode* node);
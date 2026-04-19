#include <iostream>
#include "grammar.h"
#include "left_factoring.h"
#include "left_recursion.h"
#include "first_follow.h"
#include "parser.h"
#include "parsing_table.h"
#include "tree.h"
#include "stack.h"
#include "items.h"
#include <algorithm>
using namespace std;

int main() {
    // ====================================================
    // ====================================================

    Grammar gr = loadGrammar("input/grammar5_error_handle1.txt");
    vector<ItemState> v;
    map<string, set<string>> fist = computeFirst(gr);
    map<string, set<string>> follo = computeFollow(gr, fist);
    v = dfa_generate(gr, fist);
    printItemSets(v);
    buildParsingTable(v, gr, follo, 1, fist);
    printLRParseTable();

    // ====================================================
    // ====================================================
    return 0;

    string filename_suffix = "5_all_accept1";
    filename_suffix = "5_error_handle1";

    // load grammar
    //Grammar gr = loadGrammar("input/grammar" + filename_suffix + ".txt");
    cout << "=== Original Grammar ===\n";
    printGrammar(gr);
    
    
    

    // Left Recursion Removal
    cout << "=== Left Recursion Removed Grammar ===\n";
    Grammar* temp = eliminateRecursion(gr);
    Grammar g = *temp;
    delete temp;
    printGrammar(g);
    
    // Left Factoring
    cout << "=== Left Factored Grammar ===\n";
    leftFactor(g);
    printGrammar(g);
    saveGrammar(g);

    cout << "=== First & Follow Sets ===\n";

    // First Sets
    map<string, set<string>> first = computeFirst(g);
    
    // Follow Sets
    map<string, set<string>> follow = computeFollow(g, first);

    printFirst(first, g);
    printFollow(first, follow, g);

    // Parsing Table
    createParseTable(first, follow, g);

    cout << endl << endl;
    printNsaveParseTable(g);

    auto inputs = readInputFile("input/input" + filename_suffix + ".txt");
    int trace_no = 1;
    for (auto& input : inputs) {
        parse(input, g, trace_no);
        ParseTreeNode* root = buildParseTree(g, ll1table, input);

        if (root) {
            cout << "\n\n======= PARSE TREE (ASCII) =======\n\n";
            printTreeColored(root, g, trace_no);
            deleteTree(root);
            root = nullptr;
        }
		++trace_no;
    }
    return 0;
}
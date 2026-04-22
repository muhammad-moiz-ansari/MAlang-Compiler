#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "grammar.h"
#include "grammar_augment.h"
#include "first_follow.h"
#include "slr_parser.h"
#include "parser.h"
#include "tree.h"
using namespace std;

void saveComparison(int slrSize, int lr1Size, const string& filename);
void parse_input_file(const Grammar& g, const string& inputFileType, const string& displayTitle, const string& outFileName, bool isFirstFile);

int main() {
    // ====================== 1. Load grammar ======================
    Grammar g;
    g = loadGrammar("input/grammar1.txt");
    cout << "=== Original Grammar ===\n";
    printGrammar(g);

    // ====================== 2. Augment grammar ======================
    augmentGrammar(g);
    cout << "\n=== Augmented Grammar ===\n";
    printGrammar(g);
    saveGrammar(g, "output/augmented_grammar.txt");

    // ====================== 3. Compute FIRST and FOLLOW ======================
    map<string, set<string>> FIRST, FOLLOW;
    FIRST = computeFirst(g);
    FOLLOW = computeFollow(g, FIRST);
    cout << "\n=== First & Follow Sets ===\n";
    printFirst(FIRST, g);
    printFollow(FIRST, FOLLOW, g);
    saveFirstFollow(FIRST, FOLLOW, g, "output/first_follow.txt");

    // ====================== 4. SLR(1) Item Sets + Table ======================
    cout << "\n========== SLR(1) ==========\n";
    vector<ItemState> slrStates = buildSLR(g, FOLLOW, FIRST);

    cout << "\n--- LR(0) Item Sets ---\n";
    printNsaveItemSets(slrStates, "output/slr_items.txt");

    cout << "\n--- SLR(1) Parsing Table ---\n";
    printNsaveLRParseTable("output/slr_parsing_table.txt");

    // ====================== 5. SLR(1) Parse input strings ======================
    cout << "\n--- SLR(1) Parsing Traces ---\n";
    // true = overwrite the file
    parse_input_file(g, "valid", "SLR(1) - Valid", "output/slr_trace.txt", true);
    // false = append to the file
    parse_input_file(g, "invalid", "SLR(1) - Invalid", "output/slr_trace.txt", false);

    // ====================== 6. LR(1) Item Sets + Table ======================
    cout << "\n========== LR(1) ==========\n";
    vector<ItemState> lr1States = buildLR1(g, FOLLOW, FIRST);

    cout << "\n--- LR(1) Item Sets ---\n";
    printNsaveItemSets(lr1States, "output/lr1_items.txt");

    cout << "\n--- LR(1) Parsing Table ---\n";
    printNsaveLRParseTable("output/lr1_parsing_table.txt");

    // ====================== 7. LR(1) Parse input strings ======================
    cout << "\n--- LR(1) Parsing Traces ---\n";
    // true = overwrite the file
    parse_input_file(g, "valid", "LR(1) - Valid", "output/lr1_trace.txt", true);
    // false = append to the file
    parse_input_file(g, "invalid", "LR(1) - Invalid", "output/lr1_trace.txt", false);

    // ====================== 8. Comparison ======================
    cout << "\n========== Comparison ==========\n";
    cout << "SLR(1) states : " << slrStates.size() << "\n";
    cout << "LR(1)  states : " << lr1States.size() << "\n";
    cout << "Extra states in LR(1): " << (lr1States.size() - slrStates.size()) << "\n";
    saveComparison(slrStates.size(), lr1States.size(), "output/comparison.txt");

    return 0;
}


/////////////////////// FUNCTION DEFINITIONS ///////////////////////

void parse_input_file(const Grammar& g, const string& inputFileType, const string& displayTitle, const string& outFileName, bool isFirstFile)
{
    ifstream fin("input/input_" + inputFileType + ".txt");
    string line;

    // True for first parse of this type of parser
    bool firstLine = true;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        vector<string> tokens = tokenizeInput(line);

        parse(tokens, g, displayTitle, isFirstFile, firstLine, line, outFileName);

		isFirstFile = false;  // After the first line, we want to append to the file instead of overwriting
        firstLine = false;
    }
}

void saveComparison(int slrSize, int lr1Size, const string& filename) {
    // Open the file for writing
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not open " << filename << " for writing.\n";
    }

    outFile << "\n========== Comparison ==========\n";
    outFile << "SLR(1) states : " << slrSize << "\n";
    outFile << "LR(1)  states : " << lr1Size << "\n";
    outFile << "Extra states in LR(1): " << (lr1Size - slrSize) << "\n";
    
    if (outFile.is_open()) {
        outFile << endl;
        outFile.close();
        cout << "\n[Success] Comparison safely saved to " << filename << "\n\n";
    }
}
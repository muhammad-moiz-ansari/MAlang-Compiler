#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <chrono>
#include "grammar.h"
#include "grammar_augment.h"
#include "first_follow.h"
#include "slr_parser.h"
#include "parser.h"
#include "tree.h"
using namespace std;

void saveComparison(int slrSize, int lr1Size, long long slrTime, long long lr1Time, long long slrMemory, long long lr1Memory, const string& filename);
void parse_input_file(const Grammar& g, const string& inputFileType, const int& langNum, const string& displayTitle, const string& outFileName, bool isFirstFile);

int main() {
	int langNum;  // Change this to switch between grammar1, grammar2, etc and input_valid1, input_valid2, etc
	string grammarFileName;

	// ====================== 0. Select input files ======================
	cout << "Choose one of the following grammars (Corresponding input files will be selected automatically):\n";
	cout << "1. grammar1.txt\n2. grammar2.txt\n3. grammar3.txt\n4. grammar_with_conflict.txt\n";
    while (true)
    {
        cout << "Enter choice (1-4): ";
		cin >> langNum;
        if (langNum >= 1 && langNum <= 4) {
            break;
        }
        else {
            cout << "Invalid choice! Please enter a number between 1 and 4.\n";
		}
    }
    if (langNum == 4)
        grammarFileName = "input/grammar_with_conflict.txt";
    else
		grammarFileName = "input/grammar" + to_string(langNum) + ".txt";


    // ====================== 1. Load grammar ======================
    Grammar g;
    g = loadGrammar(grammarFileName);
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

    // Start clock
    auto slr_start = chrono::high_resolution_clock::now();

    vector<ItemState> slrStates = buildSLR(g, FOLLOW, FIRST);

    // Stop clock
    auto slr_end = chrono::high_resolution_clock::now();

    // Calculate duration in microseconds
    auto slr_time = chrono::duration_cast<chrono::microseconds>(slr_end - slr_start).count() / 1000;
    cout << "Time to construct SLR(1) table: " << slr_time << " milliseconds\n";

    // Memory calculation
    int slrActionCount = 0, slrGotoCount = 0;
    for (auto& r : ACTION) slrActionCount += r.second.size();
    for (auto& r : GOTO)   slrGotoCount += r.second.size();
    long long slrMemory = slrActionCount * 14LL + slrGotoCount * 8LL;
    cout << "SLR(1) approx. memory: " << slrMemory << " bytes\n";

    cout << "\n--- LR(0) Item Sets ---\n";
    printNsaveItemSets(slrStates, "output/slr_items.txt");

    cout << "\n--- SLR(1) Parsing Table ---\n";
    printNsaveLRParseTable("output/slr_parsing_table.txt");

    // ====================== 5. SLR(1) Parse input strings ======================
    cout << "\n--- SLR(1) Parsing Traces ---\n";
    // true = overwrite the file
    parse_input_file(g, "valid", langNum, "SLR(1) - Valid", "output/slr_trace.txt", true);
    // false = append to the file
    parse_input_file(g, "invalid", langNum, "SLR(1) - Invalid", "output/slr_trace.txt", false);

    // ====================== 6. LR(1) Item Sets + Table ======================
    cout << "\n========== LR(1) ==========\n";

    // Start clock
    auto lr1_start = chrono::high_resolution_clock::now();

    vector<ItemState> lr1States = buildLR1(g, FOLLOW, FIRST);

    // Stop clock
    auto lr1_end = chrono::high_resolution_clock::now();

    // Calculate duration in microseconds
    auto lr1_time = chrono::duration_cast<chrono::microseconds>(lr1_end - lr1_start).count() / 1000;
    cout << "Time to construct LR(1) table: " << lr1_time << " milliseconds\n";

    // Memory calculation
    int lr1ActionCount = 0, lr1GotoCount = 0;
    for (auto& r : ACTION) lr1ActionCount += r.second.size();
    for (auto& r : GOTO)   lr1GotoCount += r.second.size();
    long long lr1Memory = lr1ActionCount * 14LL + lr1GotoCount * 8LL;
    cout << "LR(1) approx. memory: " << lr1Memory << " bytes\n";

    cout << "\n--- LR(1) Item Sets ---\n";
    printNsaveItemSets(lr1States, "output/lr1_items.txt");

    cout << "\n--- LR(1) Parsing Table ---\n";
    printNsaveLRParseTable("output/lr1_parsing_table.txt");

    // ====================== 7. LR(1) Parse input strings ======================
    cout << "\n--- LR(1) Parsing Traces ---\n";
    // true = overwrite the file
    parse_input_file(g, "valid", langNum, "LR(1) - Valid", "output/lr1_trace.txt", true);
    // false = append to the file
    parse_input_file(g, "invalid", langNum, "LR(1) - Invalid", "output/lr1_trace.txt", false);

    // ====================== 8. Comparison ======================
    cout << "\n========== Comparison ==========\n";
	int slrSize = slrStates.size(), lr1Size = lr1States.size();

    // States
    cout << "--- States ---\n";
    cout << "SLR(1) states        : " << slrSize << "\n";
    cout << "LR(1)  states        : " << lr1Size << "\n";
    cout << "Extra states in LR(1): " << (lr1Size - slrSize) << "\n\n";

    // Time
    cout << "--- Table Construction Time ---\n";
    cout << "SLR(1) time : " << slr_time << " ms\n";
    cout << "LR(1)  time : " << lr1_time << " ms\n";
    cout << "LR(1) is    : " << (slr_time > 0 ? to_string(lr1_time / max(slr_time, 1LL)) : "N/A") << "x slower\n\n";

    // Memory
    cout << "--- Approximate Memory Usage (table entries only) ---\n";
    cout << "SLR(1) memory : " << slrMemory << " bytes (" << slrMemory / 1024 << " KB)\n";
    cout << "LR(1)  memory : " << lr1Memory << " bytes (" << lr1Memory / 1024 << " KB)\n";
    cout << "LR(1) uses    : " << (lr1Memory - slrMemory) << " bytes more than SLR(1)\n";
    cout << "(Note: actual memory is higher due to std::map node overhead ~40 bytes/entry)\n\n";

    saveComparison(slrSize, lr1Size, slr_time, lr1_time, slrMemory, lr1Memory, "output/comparison.txt");

    return 0;
}


/////////////////////// FUNCTION DEFINITIONS ///////////////////////

void parse_input_file(const Grammar& g, const string& inputFileType, const int& langNum, const string& displayTitle, const string& outFileName, bool isFirstFile)
{
    ifstream fin("input/input_" + inputFileType + to_string(langNum) + ".txt");
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

void saveComparison(int slrSize, int lr1Size, long long slrTime, long long lr1Time, long long slrMemory, long long lr1Memory, const string& filename) {
    // Open the file for writing
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not open " << filename << " for writing.\n";
    }

    outFile << "\n========== Comparison ==========\n";
    // States
    outFile << "--- States ---\n";
    outFile << "SLR(1) states        : " << slrSize << "\n";
    outFile << "LR(1)  states        : " << lr1Size << "\n";
    outFile << "Extra states in LR(1): " << (lr1Size - slrSize) << "\n\n";

    // Time
    outFile << "--- Table Construction Time ---\n";
    outFile << "SLR(1) time : " << slrTime << " ms\n";
    outFile << "LR(1)  time : " << lr1Time << " ms\n";
    outFile << "LR(1) is    : " << (slrTime > 0 ? to_string(lr1Time / max(slrTime, 1LL)) : "N/A") << "x slower\n\n";

    // Memory
    outFile << "--- Approximate Memory Usage (table entries only) ---\n";
    outFile << "SLR(1) memory : " << slrMemory << " bytes (" << slrMemory / 1024 << " KB)\n";
    outFile << "LR(1)  memory : " << lr1Memory << " bytes (" << lr1Memory / 1024 << " KB)\n";
    outFile << "LR(1) uses    : " << (lr1Memory - slrMemory) << " bytes more than SLR(1)\n";
    outFile << "(Note: actual memory is higher due to std::map node overhead ~40 bytes/entry)\n\n";
    
    if (outFile.is_open()) {
        outFile << endl;
        outFile.close();
        cout << "\n[Success] Comparison safely saved to " << filename << "\n\n";
    }
}
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "parser.h"
#include "stack.h"
using namespace std;

///////////////////////////////////
//                               //
//         PARSING TABLE         //
//                               //
///////////////////////////////////

// Tokenize Input Line (e.g. "id + id * id") into vector of symbols, appends "$"
vector<string> tokenizeInput(const string& line) {
    vector<string> tokens;

    string token;
    stringstream ss(line);

    while (ss >> token)
        tokens.push_back(token);
    tokens.push_back("$");  // Append end marker
    return tokens;
}

// HELPER: Print one step of the trace
// stepNum | stack (symbols) | remaining input | action taken
void printStep(int stepNum, const vector<pair<string, int>>& stk,  // (symbol, state) pairs
    const vector<string>& tokens, int inputPos, const string& action, ofstream& outFile)
{
    // Step number
    cout << left << setw(6) << stepNum;
    if (outFile.is_open()) outFile << left << setw(6) << stepNum;

    // Stack
    cout << "| ";
    if (outFile.is_open()) outFile << "| ";
    for (auto& element : stk) {
        cout << element.first << " ";
        if (outFile.is_open()) outFile << element.first << " ";
    }
    cout << setw(30 - (int)stk.size() * 2) << " ";  // padding
    if (outFile.is_open()) outFile << setw(30 - (int)stk.size() * 2) << " ";

    // Remaining input
    cout << "| ";
	if (outFile.is_open()) outFile << "| ";
    for (int i = inputPos; i < tokens.size(); i++) {
        cout << tokens[i] << " ";
        if (outFile.is_open()) outFile << tokens[i] << " ";
    }
    cout << setw(20) << " ";
	if (outFile.is_open()) outFile << setw(20) << " ";

    // Action
    cout << "| " << action << endl;
	if (outFile.is_open()) outFile << "| " << action << endl;
}



bool parse(const vector<string>& tokens, const Grammar& g, const string& parserName, bool isFirstFile, bool firstLine, const string& inputStr, const string& filename)
{
    // Open the file for writing (overwrite if first line, append otherwise)
    ofstream outFile;
    if (isFirstFile) {
        outFile.open(filename);
    }
    else {
        outFile.open(filename, ios::app);
    }

    if (!outFile.is_open()) {
        cout << "Error: Could not open " << filename << " for writing.\n";
    }

    if (firstLine) {
        cout << "\n========== " << parserName << " Parsing ==========\n";
        if (outFile.is_open()) outFile << "\n========== " << parserName << " Parsing ==========\n";
    }

    cout << "\n\nInput: " << inputStr << "\n\n";
    if (outFile.is_open()) outFile << "\n\nInput: " << inputStr << "\n\n";

    cout << left << setw(6) << "Step"
        << "| Stack                          "
        << "| Input               "
        << "| Action\n";
    cout << string(80, '-') << endl;

    if (outFile.is_open()) {
        outFile << left << setw(6) << "Step"
            << "| Stack                          "
            << "| Input               "
            << "| Action\n";
        outFile << string(80, '-') << endl;
    }

    // Stack: (symbol, state) pairs
    // Bottom of stack: ("$", 0)
    vector<pair<string, int>> stk;
    stk.push_back({ "$", 0 });

    int inputPos = 0;
    int step = 1;

    // For parse tree
    // Each stack entry also needs a tree node ... we'll use a parallel node stack
    // (integrate with tree.cpp here)
    // TODO

    while (true) {
        int  s = stk.back().second;          // top of stack
        string a = tokens[inputPos];         // current input symbol

        // Look up ACTION[s][a]
        if (ACTION.find(s) == ACTION.end() || ACTION[s].find(a) == ACTION[s].end()) {
            printStep(step, stk, tokens, inputPos, "ERROR: no action for [" + to_string(s) + ", " + a + "]", outFile);

            cout << "Result: REJECTED\n";
            if (outFile.is_open()) outFile << "Result: REJECTED\n";

            return false;
        }

        string action = ACTION[s][a];

        // ?????????????????????? SHIFT ??????????????????????
        if (action[0] == 's') {
            int t = stoi(action.substr(1));   // target state
            printStep(step++, stk, tokens, inputPos, "Shift " + to_string(t), outFile);
            stk.push_back({ a, t });
            inputPos++;
        }

        // ?????????????????????? REDUCE ??????????????????????
        else if (action[0] == 'r') {
            // Action format: "r(A->XYZ)"
            string inner = action.substr(2, action.size() - 3); // "A->XYZ"
            int arrow = inner.find("->");
            string lhs = inner.substr(0, arrow);
            string rhsStr = inner.substr(arrow + 2);

            // We need to get symbol count to pop 2x times the count
            // So we check in grammar
            int rhsLen = 0;
            vector<string> rhsSymbols;

            // Finding matching production in grammar
            if (g.rules.count(lhs)) {
                for (auto& prod : g.rules.at(lhs).prods) {
                    // Reconstructing it as a string to compare with rhsStr
                    string concat = "";
                    for (auto& sym : prod.symbols)
                        concat += sym;
                    if (concat == rhsStr) {
                        rhsLen = prod.symbols.size();
                        rhsSymbols = prod.symbols;
                        break;
                    }
                }
            }

            // Handle epsilon — pop 0
            if (rhsStr == "epsilon") rhsLen = 0;

            printStep(step++, stk, tokens, inputPos, "Reduce " + lhs + " -> " + rhsStr, outFile);

            // Pop (2 * rhsLen) entries (symbol + state = 1 pair/entry)
            for (int i = 0; i < rhsLen; i++)
                stk.pop_back();

            // After popping, top state tells us where to go
            int topState = stk.back().second;

            if (GOTO.find(topState) == GOTO.end() || GOTO[topState].find(lhs) == GOTO[topState].end()) {
                cout << "ERROR: no GOTO for [" << topState << ", " << lhs << "]\n";
                if (outFile.is_open()) outFile << "ERROR: no GOTO for [" << topState << ", " << lhs << "]\n";

                cout << "Result: REJECTED\n";
                if (outFile.is_open()) outFile << "Result: REJECTED\n";
                return false;
            }

            int gotoSt = GOTO[topState][lhs];
            stk.push_back({ lhs, gotoSt });
        }

        // ?????????????????????? ACCEPT ??????????????????????
        else if (action == "accept") {
            printStep(step++, stk, tokens, inputPos, "ACCEPT", outFile);

            cout << "Result: ACCEPTED\n";
            if (outFile.is_open()) outFile << "Result: ACCEPTED\n";
            return true;
        }

        // ?????????????????????? ERROR ??????????????????????
        else {
            printStep(step, stk, tokens, inputPos, "ERROR: unknown action " + action, outFile);

            cout << "Result: REJECTED\n";
            if (outFile.is_open()) outFile << "Result: REJECTED\n";
            return false;
        }
    }
}
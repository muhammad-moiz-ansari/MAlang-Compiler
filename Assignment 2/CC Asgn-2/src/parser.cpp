#include <iostream>
#include <set>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "parser.h"
#include "first_follow.h"
#include "grammar.h"
using namespace std;

///////////////////////////////////
//                               //
//         PARSING TABLE         //
//                               //
///////////////////////////////////

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
    if (find(g.terminals.begin(), g.terminals.end(), "$") == g.terminals.end())
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

void printNsaveParseTable(Grammar& g, const string& filename) {
    const int COL_WIDTH = 15;
    // Open the file for writing
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not open " << filename << " for writing.\n";
    }

    cout << "\n\n========= LL(1) PARSE TABLE =========\n\n";

    // Header
    cout << setw(COL_WIDTH) << " ";
    if (outFile.is_open()) outFile << setw(COL_WIDTH) << " ";
    for (const auto& t : g.terminals) {
        cout << setw(COL_WIDTH) << t;
        if (outFile.is_open()) outFile << setw(COL_WIDTH) << t;
    }
    cout << endl;
    if (outFile.is_open()) outFile << endl;

    // Line
    cout << string(COL_WIDTH * (g.terminals.size() + 1), '-') << endl;
    if (outFile.is_open()) outFile << string(COL_WIDTH * (g.terminals.size() + 1), '-') << endl;

    // Rows
    for (const auto& nt : g.nonTerminals) {
        cout << setw(COL_WIDTH) << nt;
        if (outFile.is_open()) outFile << setw(COL_WIDTH) << nt;

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
            if (outFile.is_open()) outFile << setw(COL_WIDTH) << cell;
        }
        cout << endl;
        if (outFile.is_open()) outFile << endl;
    }

    // Close the file and confirm
    if (outFile.is_open()) {
        outFile << endl;
        outFile.close();
        cout << "\n[Success] Parsing table safely saved to " << filename << "\n\n";
    }
}

////////////////////////////////////
//                                //
//         STRING PARSING         //
//                                //
////////////////////////////////////

// Reading Input File
vector<string> tokenizeLine(const string& line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (ss >> token)
        tokens.push_back(token);
    tokens.push_back("$");  // Append end marker
    return tokens;
}

vector<vector<string>> readInputFile(const string& filename) {
    vector<vector<string>> tokensList;
    string line;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open file: " << filename << endl;
        exit(1);
    }

    while (getline(file, line)) {
        if (line.empty())
            continue;

        tokensList.push_back(tokenizeLine(line));
    }
    return tokensList;
}

int stackStr_gap = 65,
    inputStr_gap = 20;

void printStep(int step, Stack<string> stk, const vector<string>& tokens, int pos, const string& action, ofstream& outFile) {

    // ----- Convert stack to string (bottom to top) -----
    Stack<string> st1 = stk;
    Stack<string> st2;
    string stackStr = "";
    // Reversing stack
    while (!st1.empty()) {
        st2.push(st1.top());
        st1.pop();
    }
    // Appending in string
    while (!st2.empty()) {
        stackStr.append(st2.top());
        st2.pop();
        if (!st2.empty())
            stackStr.append(" ");
    }

    // ----- Convert remaining tokens to string -----
    string inputStr = "";
    for (int i = pos; i < tokens.size(); ++i) {
        inputStr.append(tokens[i]);
        if (i < tokens.size() - 1)
            inputStr.append(" ");
    }

    // ----- Print formatted row -----
    cout << left
        << setw(5) << step
        << "| " << setw(stackStr_gap) << stackStr
        << "| " << setw(inputStr_gap) << inputStr
        << "| " << action << "\n";
    if (outFile.is_open()) outFile << left
        << setw(5) << step
        << "| " << setw(stackStr_gap) << stackStr
        << "| " << setw(inputStr_gap) << inputStr
		<< "| " << action << "\n";
}

void parse(vector<string> input, const Grammar& g, int trace_no) {
	string filename = "output/parsing_trace" + to_string(trace_no) + ".txt";

    // Open the file for writing
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not open " << filename << " for writing.\n";
    }

    cout << "\n\n======= PARSING TRACE " << trace_no << " =======\n\n";
    cout << left << "Step | " << setw(stackStr_gap) << "Stack" << "| " << setw(inputStr_gap) << "Input" << "| Action\n";
	if (outFile.is_open()) outFile << left << "Step | " << setw(stackStr_gap) << "Stack" << "| " << setw(inputStr_gap) << "Input" << "| Action\n";
    cout << "-----|-" << string(stackStr_gap, '-') << "|-" << string(inputStr_gap, '-') << "|" << string(30, '-') << endl;
	if (outFile.is_open()) outFile << "-----|-" << string(stackStr_gap, '-') << "|-" << string(inputStr_gap, '-') << "|" << string(30, '-') << endl;

    Stack<string> st;
    int ind = 0;
    string action;
    int step = 1;
    int errorCount = 0;
    bool isAbort = false;

    st.push("$");
    st.push(g.startSymbol);

    while (!st.empty()) {
        string tos = st.top();  // Top of Stack
        string lookahead = input[ind];
        Stack<string> tempSt = st;

        // Case 1: Both are $ (Accept)
        if (tos == "$" && lookahead == "$") {
            action = "Accept";
            printStep(step, st, input, ind, action, outFile);
            cout << "\nResult: String accepted!\n";
            break;
        }

        // Stack is empty, but input still has tokens (Extra garbage at the end)
        if (tos == "$" && lookahead != "$") {
            errorCount++;
            action = "ERROR: Extra input remaining. Unexpected trailing '" + lookahead + "'";
            printStep(step, st, input, ind, action, outFile);
            isAbort = true;
            break;
        }

        // Case 2: tos is a Terminal or $
        if (isTerminal(tos) || tos == "$") {
            if (tos == lookahead) {
                action = "Match " + lookahead;
                st.pop();
                ind++;
            }
            else {
                errorCount++;
                action = "ERROR: Unexpected \'" + lookahead + "\'\t";
                action.append("Expected: " + tos + "\t");
                action.append("Skipping \'" + lookahead + "\'");
                ind++; // skip input
            }
        }
        // Case 3: tos is a Non-Terminal
        else {
            GrammarRule rule = ll1table[tos][lookahead];

            if (rule.prods.empty()) {
                errorCount++;
                action = "ERROR: No production for M[" + tos + ", " + lookahead + "]\t";
                // Pop the stack for error recovery
                st.pop();
            }
            else {
                // Expand the production
                action = "Gen " + rule.nonTerminal + " ->";
                int size = rule.prods[0].symbols.size();
                st.pop();

                // Push the right side in reverse order
                for (int i = size - 1; i >= 0; --i) {
                    string sym = rule.prods[0].symbols[i];
                    if (!isEpsilon(sym)) {
                        st.push(sym);
                    }
                    action.append(" " + rule.prods[0].symbols[size - 1 - i]);
                }
            }
        }

        printStep(step, tempSt, input, ind, action, outFile);
        ++step;

        if (ind >= input.size()) {
            isAbort = true;
            break;
        }
    }
    if (errorCount > 0) {
        if (!isAbort)
            cout << "\nResult: Parsing completed with " << errorCount << " error.\n";
        else
            cout << "\nResult: Parsing aborted with " << errorCount << " error.\n";
    }

    // Close the file and confirm
    if (outFile.is_open()) {
        outFile << endl;
        outFile.close();
        cout << "\n[Success] Parsing table safely saved to " << filename << "\n\n";
    }
}
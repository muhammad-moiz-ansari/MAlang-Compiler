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

void printParseTable(Grammar& g) {

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

void printStep(int step, Stack<string> stk, const vector<string>& tokens, int pos, const string& action) {

    // ----- Convert stack to string (bottom to top) -----
    Stack<string> st1 = stk;
    Stack<string> st2;
    string stackStr;
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
    string inputStr;
    for(int i=pos; i<tokens.size(); ++i) {
        inputStr.append(tokens[i]);
        if (i < tokens.size() - 1)
            inputStr.append(" ");
    }

    // ----- Print formatted row -----
    cout << left
        << setw(5) << step
        << "| " << setw(30) << stackStr
        << "| " << setw(20) << inputStr
        << "| " << action << "\n";
}

void parse(vector<string> input, const Grammar& g)
{
    cout << "\n\n======= PARSING TRACE =======\n\n";
    cout << left << "Step | " << setw(30) << "Stack" << "| " << setw(20) << "Input" << "| Action\n";
    //cout << "Step | Stack" << setw(30) << "| Input" << setw(20) << "| Action\n";
    cout << "-----|-" << string(30, '-') << "|-" << string(20, '-') << "|" << string(60, '-') << endl;

    Stack<string> st;
    int ind = 0;
    string action;
    int step = 1;

    st.push("$");
    st.push(g.startSymbol);

    while (!st.empty()) {
        string tos = st.top();  // Top of Stack
        string lookahead = input[ind];
        Stack<string> tempSt = st;

        // Case 1: Both are $ (Accept)
        if (tos == "$" && lookahead == "$") {
            action = "Accept";
            printStep(step, st, input, ind, action);
            cout << "\nResult: String accepted!\n";
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
                action = "ERROR: Unexpected \'" + lookahead + "\'\n";
                action.append("Expected: " + tos + "\n");
                action.append("Skipping \'" + lookahead + "\'");
                ind++; // skip input
            }
        }
        // Case 3: tos is a Non-Terminal
        else {
            GrammarRule rule = ll1table[tos][lookahead];

            if (rule.prods.empty()) {
                action = "ERROR: No production for M[" + tos + ", " + lookahead + "]\n";
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

        printStep(step, st, input, ind, action);
        ++step;
    }
}
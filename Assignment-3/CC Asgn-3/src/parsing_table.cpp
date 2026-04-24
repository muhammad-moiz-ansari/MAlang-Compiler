#include"parsing_table.h"
#include<iostream>
#include <fstream>
using namespace std;

map<int, map<string, string>> ACTION;
map<int, map<string, int>> GOTO;

void buildParsingTable(vector<ItemState>& C, const Grammar& g, map<string, set<string>>& FOLLOW, int type, map<string, set<string>>& first)
{
    for (int i = 0; i < C.size(); i++) {
        ItemState I = C[i];

        for (int j = 0; j < I.items.size(); j++) {

            bool isEpsilonRule = (I.items[j].rhs.size() == 1 && I.items[j].rhs[0] == "epsilon");

            // ---------------------- REDUCE CASE ----------------------
            // If the dot is at the end, OR it's an epsilon rule, it's a reduction.
            if (I.items[j].dotPos == I.items[j].rhs.size() || isEpsilonRule) {

                string A = I.items[j].lhs;
                string production = A + "->";

                for (auto& s : I.items[j].rhs)
                    production += s;

                // SLR(1) reduce
                if (type == 0) {
                    if (FOLLOW.count(A)) {
                        for (auto& t : FOLLOW.at(A)) {
                            // Do not put S' -> S in the reduce table. That's for ACCEPT only.
                            if (A == "S'") continue;

                            string newAction = "r(" + production + ")";
                            // --- CONFLICT CHECK ---
                            if (ACTION[i].count(t) && ACTION[i][t] != newAction) {
                                cout << "CONFLICT DETECTED in SLR(1) State " << i << " on symbol '" << t << "': "
                                    << ACTION[i][t] << " vs " << newAction << endl;
                            }
                            ACTION[i][t] = newAction;
                        }
                    }
                }

                // LR(1) reduce
                else if (type == 1) {
                    for (auto& it : I.items) {
                        if (it.lhs == A && it.rhs == I.items[j].rhs &&
                            (it.dotPos == I.items[j].dotPos || isEpsilonRule)) {

                            if (A == "S'") continue;

                            string newAction = "r(" + production + ")";
                            // --- CONFLICT CHECK ---
                            if (ACTION[i].count(it.look) && ACTION[i][it.look] != newAction) {
                                cout << "CONFLICT DETECTED in LR(1) State " << i << " on symbol '" << it.look << "': "
                                    << ACTION[i][it.look] << " vs " << newAction << endl;
                            }
                            ACTION[i][it.look] = newAction;
                        }
                    }
                }
            }

            // ---------------------- SHIFT CASE ----------------------
            // It can ONLY be a shift if it is NOT an epsilon rule, and the dot is not at the end.
            else if (!isEpsilonRule && I.items[j].dotPos < I.items[j].rhs.size()) {

                string sym = I.items[j].rhs[I.items[j].dotPos];

                if (isTerminal(sym)) {
                    int targetState = getGotoState(I, sym, C, type, g, first);

                    if (targetState != -1) {
                        string newAction = "s" + to_string(targetState);
                        // --- CONFLICT CHECK ---
                        if (ACTION[i].count(sym) && ACTION[i][sym] != newAction) {
                            cout << "CONFLICT DETECTED in State " << i << " on symbol '" << sym << "': "
                                << ACTION[i][sym] << " vs " << newAction << endl;
                        }
                        ACTION[i][sym] = newAction;
                    }
                }
            }

            // ---------------------- ACCEPT CASE ----------------------
            if (I.items[j].lhs == "S'" &&
                I.items[j].dotPos == I.items[j].rhs.size() &&
                (I.items[j].look == "$" || I.items[j].look == "x")) {

                ACTION[i]["$"] = "accept";
            }
        }

        // GOTO Table Construction
        for (auto& nt : g.nonTerminals) {
            int j = getGotoState(I, nt, C, type, g, first);
            if (j != -1)
                GOTO[i][nt] = j;
        }
    }
}

void printNsaveLRParseTable(const string& filename) {
    // Open the file for writing
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not open " << filename << " for writing.\n";
    }
    cout << "\nACTION TABLE\n";
    outFile << "\nACTION TABLE\n";
    for (auto& row : ACTION) {
        cout << "State " << row.first << ":\n";
        outFile << "State " << row.first << ":\n";
        for (auto& col : row.second) {
            cout << "  " << col.first << " -> " << col.second << endl;
            outFile << "  " << col.first << " -> " << col.second << endl;
        }
    }

    cout << "\nGOTO TABLE\n";
    outFile << "\nGOTO TABLE\n";
    for (auto& row : GOTO) {
        cout << "State " << row.first << ":\n";
        outFile << "State " << row.first << ":\n";
        for (auto& col : row.second) {
            cout << "  " << col.first << " -> " << col.second << endl;
            outFile << "  " << col.first << " -> " << col.second << endl;
        }
    }

    if (outFile.is_open()) {
        outFile << endl;
        outFile.close();
        cout << "\n[Success] LR Parsing Table safely saved to " << filename << "\n\n";
    }
}
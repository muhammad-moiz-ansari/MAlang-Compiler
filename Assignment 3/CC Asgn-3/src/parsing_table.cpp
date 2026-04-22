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
            // Shift Case
            if (I.items[j].dotPos < I.items[j].rhs.size()) {
                string sym = I.items[j].rhs[I.items[j].dotPos];

                if (isTerminal(sym)) {
                    int j = getGotoState(I, sym, C, type, g, first);

                    if (j != -1)
                        ACTION[i][sym] = "s" + to_string(j);
                }
            }
            // Reduce Case
            else {
                string A = I.items[j].lhs;
                string production = A + "->";

                for (auto& s : I.items[j].rhs)
                    production += s;

                // SLR(1) reduce
                if (type == 0) {
                    // S' is the augmented start, skiping it here, accept case handles it
                    if (FOLLOW.count(A))
                        for (auto& t : FOLLOW.at(A))
                            ACTION[i][t] = "r(" + production + ")";
                }

                // LR(1) reduce
                else if (type == 1) {

                    for (auto& it : I.items) {
                        if (it.lhs == A &&
                            it.rhs == I.items[j].rhs &&
                            it.dotPos == I.items[j].dotPos) {

                            ACTION[i][it.look] = "r(" + production + ")";
                        }
                    }
                }
            }

            // Accept Case
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
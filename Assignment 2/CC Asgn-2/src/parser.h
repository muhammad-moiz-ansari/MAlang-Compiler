#pragma once
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iomanip>
#include "first_follow.h"

using namespace std;

// ---------------------------------------------------------
// Global Parsing Table Declaration
// ---------------------------------------------------------
extern map<string, map<string, GrammarRule>> ll1table;

// ---------------------------------------------------------
// Function Prototypes
// ---------------------------------------------------------

///////////////////////////////////
//                               //
//         PARSING TABLE         //
//                               //
///////////////////////////////////
void createParseTable(map<string, set<string>>& firs, map<string, set<string>>& follow, Grammar& g);

void printParseTable(Grammar& g);

////////////////////////////////////
//                                //
//         STRING PARSING         //
//                                //
////////////////////////////////////

// reads one line like "id + id * id"
// returns ["id", "+", "id", "*", "id", "$"]
vector<string> tokenizeLine(const string& line);

// reads the whole input.txt
// returns a list of token lists, one per line
vector<vector<string>> readInputFile(const string& filename);
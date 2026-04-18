#pragma once
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iomanip>
#include "first_follow.h"
#include "stack.h"

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

void printNsaveParseTable(Grammar& g, const string& filename = "output/parsing_table.txt");

////////////////////////////////////
//                                //
//         STRING PARSING         //
//                                //
////////////////////////////////////

// Reads one line like "id + id * id"
vector<string> tokenizeLine(const string& line);

// Returns a list of token lists, one per line
vector<vector<string>> readInputFile(const string& filename);


// Prints a single step of Parsing Trace
void printStep(int step, Stack<string> stk, const vector<string>& tokens, int pos, const string& action, ofstream& outFile);

void parse(vector<string> input, const Grammar& g, int trace_no);
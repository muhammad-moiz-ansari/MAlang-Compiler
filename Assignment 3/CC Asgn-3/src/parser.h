#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "parsing_table.h"
#include "tree.h"
#include "stack.h"
using namespace std;

// ---------------------------------------------------------
// Function Prototypes
// ---------------------------------------------------------

///////////////////////////////////
//                               //
//         PARSING TABLE         //
//                               //
///////////////////////////////////

// Tokenizes a single input line into symbols, appends "$"
vector<string> tokenizeInput(const string& line);

// Runs the shift-reduce parsing algorithm on the given token stream
// parserName is just for display ("SLR(1)" or "LR(1)")
// Returns true if accepted, false if error
bool parse(const vector<string>& tokens, const Grammar& g, const string& parserName, bool isFirstFile, bool firstLine, const string& inputStr, const string& filename);

//void printNsaveParseTable(Grammar& g, const string& filename = "output/parsing_table.txt");

////////////////////////////////////
//                                //
//         STRING PARSING         //
//                                //
////////////////////////////////////

//// Reads one line like "id + id * id"
//vector<string> tokenizeLine(const string& line);
//
//// Returns a list of token lists, one per line
//vector<vector<string>> readInputFile(const string& filename);
//
//
//// Prints a single step of Parsing Trace
//void printStep(int step, Stack<string> stk, const vector<string>& tokens, int pos, const string& action, ofstream& outFile);
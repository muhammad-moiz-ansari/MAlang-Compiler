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
void createParseTable(map<string, set<string>>& firs, map<string, set<string>>& follow, Grammar& g);

void printParseTable(Grammar& g);
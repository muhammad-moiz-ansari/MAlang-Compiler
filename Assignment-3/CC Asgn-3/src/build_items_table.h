#pragma once
#include "parsing_table.h"

// Builds SLR(1) canonical collection and fills ACTION/GOTO
// Returns the item sets for printing/reporting
vector<ItemState> buildSLR(
    Grammar& g,
    map<string, set<string>>& FOLLOW,
    map<string, set<string>>& FIRST
);

// Builds LR(1) canonical collection and fills ACTION/GOTO
vector<ItemState> buildLR1(
    Grammar& g,
    map<string, set<string>>& FOLLOW,
    map<string, set<string>>& FIRST
);
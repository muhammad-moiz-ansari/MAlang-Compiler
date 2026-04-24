#include "build_items_table.h"

vector<ItemState> buildSLR(
    Grammar& g,
    map<string, set<string>>& FOLLOW,
    map<string, set<string>>& FIRST)
{
    // Clear previous table entries (important if called after LR1 or vice versa)
    ACTION.clear();
    GOTO.clear();

    // Build LR(0) canonical collection (no FIRST needed)
    vector<ItemState> C = dfa_generate(g);

    // Fill ACTION and GOTO using FOLLOW sets (type=0 means SLR)
    buildParsingTable(C, g, FOLLOW, 0, FIRST);

    return C;
}

vector<ItemState> buildLR1(
    Grammar& g,
    map<string, set<string>>& FOLLOW,
    map<string, set<string>>& FIRST)
{
    ACTION.clear();
    GOTO.clear();

    // Build LR(1) canonical collection (needs FIRST for lookahead computation)
    vector<ItemState> C = dfa_generate(g, FIRST);

    // Fill ACTION and GOTO using per-item lookaheads (type=1 means LR1)
    buildParsingTable(C, g, FOLLOW, 1, FIRST);

    return C;
}
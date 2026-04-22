#pragma once
#include "grammar.h"
#include "first_follow.h"
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct Item {
    string lhs;                 // A
    vector<string> rhs;         // α β
    int dotPos;                 // position of .
    string look;
};

struct ItemState {
    vector<Item> items;
};

bool operator==(const Item& a, const Item& b);

bool containsItem(const vector<Item>& items, const Item& it);

bool equalItemSet(const ItemState& A, const ItemState& B);

ItemState closure(ItemState is, const Grammar& g);

ItemState gotoState(ItemState is, const string& sym, const Grammar& g);

vector<ItemState> dfa_generate(const Grammar& g);

void printItemSets(const vector<ItemState>& C);

// =====================================================================
// =====================================================================

ItemState closure(ItemState is, const Grammar& g, map<string, set<string>>& FIRST);

ItemState gotoState(ItemState is, const string& sym, const Grammar& g, map<string, set<string>>& FIRST);

vector<ItemState> dfa_generate(const Grammar& g, map<string, set<string>>& FIRST);

int getGotoState(ItemState I, string X, vector<ItemState>& C, int type, Grammar g, map<string, set<string>>& first);
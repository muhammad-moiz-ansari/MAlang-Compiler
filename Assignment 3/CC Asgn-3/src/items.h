#pragma once
#include "grammar.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Item {
    string lhs;                 // A
    vector<string> rhs;         // α β
    int dotPos;                 // position of .
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
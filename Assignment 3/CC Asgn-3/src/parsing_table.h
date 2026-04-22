#pragma once
#include"items.h"
#include"first_follow.h"

extern map<int, map<string, string>> ACTION;
extern map<int, map<string, int>> GOTO;

void buildParsingTable(vector<ItemState>& C, const Grammar& g, map<string, set<string>>& FOLLOW, int type, map<string, set<string>>& first);

void printNsaveLRParseTable(const string& filename);
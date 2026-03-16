#pragma once
#include "grammar.h"
#include <set>

void createParseTable(map<string, set<string>>& first, map<string, set<string>>& follow, Grammar& g);
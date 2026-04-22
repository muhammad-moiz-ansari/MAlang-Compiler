#include "items.h"
#include "first_follow.h"
#include <iostream>
#include <set>
using namespace std;

bool operator==(const Item& a, const Item& b) {
    return (a.lhs == b.lhs &&
        a.rhs == b.rhs &&
        a.dotPos == b.dotPos && a.look == b.look);
}

bool containsItem(const vector<Item>& items, const Item& it) {
    for (const auto& i : items)
        if (i == it) return true;
    return false;
}

bool equalItemSet(const ItemState& A, const ItemState& B) {
    if (A.items.size() != B.items.size()) return false;

    for (const auto& item : A.items) {
        if (!containsItem(B.items, item))
            return false;
    }
    return true;
}

ItemState closure(ItemState is, const Grammar& g) {
    ItemState cl = is;
    bool changed = false;

    do {
        changed = false;
        for (int i = 0; i < cl.items.size(); i++) {
            if (cl.items[i].dotPos <cl.items[i].rhs.size() && isNonTerminal(cl.items[i].rhs[cl.items[i].dotPos])) {
                GrammarRule r = g.rules.at(cl.items[i].rhs[cl.items[i].dotPos]);
                for (int j = 0; j < r.prods.size(); j++) {
                    Item nitem;
                    nitem.lhs = cl.items[i].rhs[cl.items[i].dotPos];
                    nitem.dotPos = 0;
                    nitem.rhs = r.prods[j].symbols;
                    nitem.look = "x";

                    if (!containsItem(cl.items, nitem)) {
                        cl.items.push_back(nitem);
                        changed = true;
                    }
                }
            }
        }
    } while (changed);

    return cl;
}

ItemState gotoState(ItemState is, const string& sym, const Grammar& g) {
    ItemState gstate;
    for (int i = 0; i < is.items.size(); i++) {
        if (is.items[i].dotPos < is.items[i].rhs.size() && is.items[i].rhs[is.items[i].dotPos] == sym) {
            Item nitem;
            nitem.dotPos = is.items[i].dotPos + 1;
            nitem.lhs = is.items[i].lhs;
            nitem.rhs = is.items[i].rhs;
            nitem.look = "x";

            gstate.items.push_back(nitem);
        }
    }
    return closure(gstate, g);
}

vector<ItemState> dfa_generate(const Grammar& g) {
    vector<ItemState> c;

    ItemState is;
    Item i;
    i.lhs = "S'";
    i.rhs = { g.startSymbol };
    i.dotPos = 0;
    i.look = "x";
    is.items.push_back(i);

    is = closure(is, g);
    c.push_back(is);
    bool changed = false;

    vector<string> sym;
    for (int j = 0; j < g.terminals.size(); j++)
        sym.push_back(g.terminals[j]);
    for (int j = 0; j < g.nonTerminals.size();j++)
        sym.push_back(g.nonTerminals[j]);

    do {
        changed = false;
        for (int k = 0; k < c.size(); k++) {
            ItemState x = c[k];
            for (int j = 0; j < sym.size(); j++) {
                ItemState s = gotoState(x, sym[j], g);
                if (s.items.empty())
                    continue;
                bool exist = 0;

                for (int l = 0; l < c.size(); l++) {
                    if (equalItemSet(c[l], s)) {
                        exist = 1;
                        break;
                    }
                }

                if (!exist) {
                    c.push_back(s);
                    changed = true;
                }
            }
        }

    } while (changed);

    return c;
}

void printItemSets(const vector<ItemState>& C) {
    for (int i = 0; i < C.size(); i++) {
        cout << "I" << i << ":\n";

        for (const auto& item : C[i].items) {
            cout << item.lhs << " -> ";

            for (int j = 0; j < item.rhs.size(); j++) {
                if (j == item.dotPos) cout << ".";
                cout << item.rhs[j] << " ";
            }

            if (item.dotPos == item.rhs.size())
                cout << ".";
            cout << " , " << item.look;
            cout << endl;
        }
        cout << endl;
    }
}

// =================================================================================
// =================================================================================

ItemState closure(ItemState is, const Grammar& g, map<string, set<string>>& FIRST) {

    ItemState cl = is;
    bool changed;

    do {
        changed = false;

        for (int i = 0; i < cl.items.size(); i++) {
            Item item = cl.items[i];
            if (item.dotPos < item.rhs.size()) {
                string B = item.rhs[item.dotPos];
                if (isNonTerminal(B)) {
                    vector<string> beta;
                    for (int k = item.dotPos + 1; k < item.rhs.size(); k++)
                        beta.push_back(item.rhs[k]);
                    beta.push_back(item.look);
                    set<string> laSet = computeFirstOfString(beta, g, FIRST);
                    GrammarRule r = g.rules.at(B);

                    for (int j = 0; j < r.prods.size(); j++) {
                        for (auto& la : laSet) {
                            Item nitem;
                            nitem.lhs = B;
                            nitem.rhs = r.prods[j].symbols;
                            nitem.dotPos = 0;
                            nitem.look = la;

                            if (!containsItem(cl.items, nitem)) {
                                cl.items.push_back(nitem);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }

    } while (changed);

    return cl;
}

ItemState gotoState(ItemState is, const string& sym, const Grammar& g, map<string, set<string>>& FIRST) {
    ItemState gstate;
    for (int i = 0; i < is.items.size(); i++) {
        if (is.items[i].dotPos < is.items[i].rhs.size() && is.items[i].rhs[is.items[i].dotPos] == sym) {
            Item nitem;
            nitem.dotPos = is.items[i].dotPos + 1;
            nitem.lhs = is.items[i].lhs;
            nitem.rhs = is.items[i].rhs;
            nitem.look = is.items[i].look;

            gstate.items.push_back(nitem);
        }
    }
    return closure(gstate, g, FIRST);
}

vector<ItemState> dfa_generate(const Grammar& g, map<string, set<string>>& FIRST) {
    vector<ItemState> c;

    ItemState is;
    Item i;
    i.lhs = "S'";
    i.rhs = { g.startSymbol };
    i.dotPos = 0;
    i.look = "$";
    is.items.push_back(i);

    is = closure(is, g, FIRST);
    c.push_back(is);
    bool changed = false;

    vector<string> sym;
    for (int j = 0; j < g.terminals.size(); j++)
        sym.push_back(g.terminals[j]);
    for (int j = 0; j < g.nonTerminals.size();j++)
        sym.push_back(g.nonTerminals[j]);

    do {
        changed = false;
        for (int k = 0; k < c.size(); k++) {
            ItemState x = c[k];
            for (int j = 0; j < sym.size(); j++) {
                ItemState s = gotoState(x, sym[j], g, FIRST);
                if (s.items.empty())
                    continue;
                bool exist = 0;

                for (int l = 0; l < c.size(); l++) {
                    if (equalItemSet(c[l], s)) {
                        exist = 1;
                        break;
                    }
                }

                if (!exist) {
                    c.push_back(s);
                    changed = true;
                }
            }
        }

    } while (changed);

    return c;
}

int getGotoState(ItemState I, string X, vector<ItemState>& C, int type, Grammar g, map<string, set<string>>& first) {

    ItemState moved;

    for (auto& item : I.items) {
        if (item.dotPos < item.rhs.size() &&
            item.rhs[item.dotPos] == X) {

            Item newItem = item;
            newItem.dotPos++;
            moved.items.push_back(newItem);
        }
    }

    if (moved.items.empty())
        return -1;

    if (type == 0)
        moved = closure(moved, g);
    else if(type == 1)
        moved = closure(moved, g, first);

    for (int i = 0; i < C.size(); i++) {
        if (equalItemSet(C[i], moved))
            return i;
    }

    return -1;
}
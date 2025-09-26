#ifndef ITEM_H
#define ITEM_H

#include <string>

struct Item {
    std::string name;
    std::string type;
    int value;
    std::string description;
    mutable int durability;
    mutable bool isBroken;

    Item(const std::string& n, const std::string& t, int val, const std::string& desc = "", int dur = 0, bool isBroken = false)
        : name(n), type(t), value(val), description(desc), durability(dur), isBroken(isBroken) {}
};

#endif // ITEM_H
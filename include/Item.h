#ifndef ITEM_H
#define ITEM_H

#include <string>

struct Item {
    std::string name;
    std::string type;
    int value;

    Item(const std::string& n, const std::string& t, int v) : name(n), type(t), value(v) {}
};

#endif // ITEM_H
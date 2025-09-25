#ifndef ITEM_H
#define ITEM_H

#include <string>

struct Item {
    std::string name;
    std::string type;
    int value;
    std::string description;

    Item(const std::string& n, const std::string& t, int v, const std::string& d = "")
        : name(n), type(t), value(v), description(d) {}
};

#endif // ITEM_H
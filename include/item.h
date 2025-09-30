#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <utility>

struct item {
    std::string mName;
    std::string mType;
    int mValue;
    std::string mDescription;
    mutable int mDurability;
    mutable bool mIsBroken;

    item(std::string pName, std::string  pType, const int pValue, std::string  pDescription = "", const int pDurability = 0, const bool pIsBroken = false)
        : mName(std::move(pName)), mType(std::move(pType)), mValue(pValue), mDescription(std::move(pDescription)), mDurability(pDurability), mIsBroken(pIsBroken) {}
};

#endif // ITEM_H
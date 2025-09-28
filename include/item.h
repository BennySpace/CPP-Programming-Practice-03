#ifndef ITEM_H
#define ITEM_H

#include <string>

struct item {
    std::string mName;
    std::string mType;
    int mValue;
    std::string mDescription;
    mutable int mDurability;
    mutable bool mIsBroken;

    item(const std::string& pName, const std::string& pType, int pValue, const std::string& pDescription = "", int pDurability = 0, bool pIsBroken = false)
        : mName(pName), mType(pType), mValue(pValue), mDescription(pDescription), mDurability(pDurability), mIsBroken(pIsBroken) {}
};

#endif // ITEM_H
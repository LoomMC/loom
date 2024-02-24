#pragma once

#include "../../utils/mem.h"
#include "Item.h"

class ItemStack {
public:
    //Item* getItem(void);
    BUILD_ACCESS(this, char, ItemStackCount, 0x22);
};
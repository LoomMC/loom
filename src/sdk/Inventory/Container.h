#pragma once

#include <cstdint>
#include "../Item/ItemStack.h"

class Container {
public:

    uintptr_t** vtable;

    const ItemStack* getItem(int slot) {
        using fn = const ItemStack*(__thiscall*)(Container*, int);
        return reinterpret_cast<fn>(this->vtable[5])(this, slot);
    }
};
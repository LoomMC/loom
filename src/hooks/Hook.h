#pragma once
#include "../utils/mem.h"
#include "../include/minhook/MinHook.h"

class Hook {
public:
    virtual void dispatch() {};
};
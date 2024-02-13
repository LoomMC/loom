#pragma once
#include <vector>

#include "Hook.h"
#include "../utils/mem.h"
#include "../include/minhook/MinHook.h"

// list the things here

class HookManager {
public:
    static inline std::vector<std::unique_ptr<Hook>> hooks;

    static void reg(Hook* h)
    {
        hooks.push_back(std::unique_ptr<Hook>(h));
    };

    static void run(Hook* h = nullptr)
    {
        if(h != nullptr)
        {
            h->dispatch();
        }
        else
        {
            for (auto& hooki : hooks)
            {
                hooki->dispatch();
            }
        }
    };

    static void init()
    {

        MH_Initialize();

        // register things
        // reg(new hook())

        run();
    };

    static void clear()
    {
        hooks.clear();
        
        MH_RemoveHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    };
};
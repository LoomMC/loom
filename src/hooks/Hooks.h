// this is kinda just like a test

#include "api/weave.h"

loom::weave::WeaveFuncPtr* o_setupandrender;

void SetupAndRenderDetour(void* a1, void* a2)
{
    loom::weave::CallFunc<void*, void*, void*>(
        o_setupandrender,
        a1,
        a2
    );
}

loom::weave::WeaveHookStatus SetupAndRenderHook()
{

    loom::weave::WeaveFuncPtr hookAddr = loom::weave::FindSignature("48 8B C4 48 89 58 18 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? ? ? 70 B8 ? ? 78 A8 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA");
    
    return loom::weave::CreateHook(hookAddr, (loom::weave::WeaveFuncPtr)&SetupAndRenderDetour, (loom::weave::WeaveFuncPtr*)&o_setupandrender);
}
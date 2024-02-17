#pragma once

#include "ScreenContext.h"
#include "../Core/ClientInstance.h"
#include "../../utils/mem.h"
#include "ItemRenderer.h"
#include "../Core/MinecraftGame.h"

class BaseActorRenderContext {
private:
    std::byte padding[4096];
public:
    BaseActorRenderContext(ScreenContext* screenCtx, ClientInstance* clientInstance, MinecraftGame* game)
    {
        using fn = void(__thiscall*)(BaseActorRenderContext*, ScreenContext*, ClientInstance*, MinecraftGame*);
        static auto func = reinterpret_cast<fn>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 4C 24 ? 57 48 83 EC ? 49 8B F8 48 8B DA 48 8B F1 48 8D 05 ? ? ? ? 48 89 01 33 ED"));
        func(this, screenCtx, clientInstance, game);
    };
    BUILD_ACCESS(this, ItemRenderer*, itemRenderer, 0x58);
};
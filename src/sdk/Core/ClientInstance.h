#pragma once

#include <stdint.h>
#include "../../utils/mem.h"
#include "../Entity/LocalPlayer.h"
#include "../Block/BlockSource.h"
#include "Minecraft.h"
#include "MinecraftGame.h"

class ClientInstance {
public:

    Minecraft* getMinecraft()
    {
        return *reinterpret_cast<Minecraft**>((uintptr_t)this + 0xD0);
    }

    BUILD_ACCESS(this, MinecraftGame*, minecraftGame, 0x0C8);

    LocalPlayer* getLocalPlayer()
    {
        return CallVFunc<28, LocalPlayer*>(this);
    }

    BlockSource* getRegion()
    {
        return CallVFunc<27, BlockSource*>(this);
    }


};
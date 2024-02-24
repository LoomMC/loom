#pragma once

// Minecraft SDK
#include "../../sdk/Block/Block.h"
#include "../../sdk/Block/BlockSource.h"
#include "../../sdk/Core/ClientInstance.h"
#include "../../sdk/Core/Minecraft.h"
#include "../../sdk/Core/MinecraftGame.h"
#include "../../sdk/Entity/Actor.h"
#include "../../sdk/Entity/LocalPlayer.h"
#include "../../sdk/Entity/Mob.h"
#include "../../sdk/Entity/Player.h"
#include "../../sdk/Item/Item.h"
#include "../../sdk/Item/ItemStack.h"
#include "../../sdk/Level/Biome.h"
#include "../../sdk/Render/BaseActorRenderContext.h"
#include "../../sdk/Render/ItemRenderer.h"
#include "../../sdk/Render/MinecraftUIRenderContext.h"
#include "../../sdk/Render/ScreenContext.h"

// Loom SDK
#include "gfx/device.h"

namespace loom {
    namespace events {};
    namespace gfx {
        namespace device {};
        namespace muirc {};
        namespace tess {};
    };
};
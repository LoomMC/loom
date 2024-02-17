#pragma once

#include "../../utils/mem.h"
#include "ScreenContext.h"
#include "ClientInstance.h"

class MinecraftUIRenderContext {
    BUILD_ACCESS(this, ClientInstance*, clientInstance, 0x8);
    BUILD_ACCESS(this, ScreenContext*, screenContext, 0x10);
};
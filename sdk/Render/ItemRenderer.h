#pragma once

#include "BaseActorRenderContext.h"
#include "../Item/ItemStack.h"

class ItemRenderer {
public:
	void renderGuiItemNew(BaseActorRenderContext* BaseActorRenderCtx, ItemStack* item, int mode, float x, float y, float opacity, float scale, bool isEnchanted);
};
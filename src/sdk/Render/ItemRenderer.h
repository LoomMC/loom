#pragma once

#include "BaseActorRenderContext.h"
#include "../Item/ItemStack.h"

class ItemRenderer {
public:
	void renderGuiItemNew(BaseActorRenderContext* BaseActorRenderCtx, ItemStack* item, uint32_t frame, float x, float y, bool forceEnchantmentFoil, float transparency, float lightMultiplyer, float scale)
	{
		using fn = void(__fastcall*)(ItemRenderer*, BaseActorRenderContext*, ItemStack*, uint32_t, float, float, bool, float, float, float);
		static auto func = reinterpret_cast<fn>(FindSignature("48 8B ? 53 55 56 57 41 ? 41 ? 41 ? 41 ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? ? 45 8B"));
		func(this, BaseActorRenderCtx, item, frame, x, y, forceEnchantmentFoil, transparency, lightMultiplyer, scale);
	}
};
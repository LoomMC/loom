#pragma once

#include "../../utils/mem.h"
#include "../../utils/math.h"
#include "Block.h"
#include "../Level/Biome.h"

class BlockSource {
public:

    BUILD_ACCESS(this, Biome*, biome, 0x30);

    Block* getBlock(const Vec3<int>& bp) {
        return CallVFunc<2, Block*>(this, bp);
    }

    Biome* getBiome(const Vec3<int>& bp) {

        static uintptr_t sig = FindSignature("40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0 75 5C");

        using fn = Biome*(__cdecl*)(BlockSource*, const Vec3<int>&);
        return reinterpret_cast<fn>(sig)(this, bp);
    }
};
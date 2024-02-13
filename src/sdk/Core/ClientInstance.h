#pragma once

#include <stdint.h>
#include "../../utils/mem.h"
#include "../Entity/LocalPlayer.h"

class ClientInstance {
public:
    LocalPlayer* getLocalPlayer()
    {
        return CallVFunc<28, LocalPlayer*>(this);
    }
};
#pragma once

#include "../Event.h"

class TickEvent : public loom::events::Event {
public:
    loom::events::EventType GetType() { return loom::events::EventType::Tick; }
};
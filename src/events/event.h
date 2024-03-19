#pragma once
#include "../common.h"

enum EventTypes
{
    OnFrame
};

class Event {
public:
    bool* cancelled;
};

namespace loom
{
    namespace events
    {

        class listener {
        public:
            virtual void run(Event* event) {};
        };

        std::vector<loom::events::listener*> listeners;
        
        template <typename A>
        void dispatch(A* event)
        {
            for (auto& listener : listeners)
            {
                listener->run(event);
            }
        }

        void add(loom::events::listener* listener)
        {
            listeners.push_back(listener);
        }
    };
};
#pragma once

#include "types/Tick.h"

#include <vector>
#include <memory>
#include <functional>

using EventCallback = std::function<void(const loom::events::Event *)>;

namespace loom
{
    namespace events
    {
        enum class EventType
        {
            Base,
            Tick
        };

        class Event
        {
        public:
            virtual ~Event() = default; // Virtual destructor (if needed)
            bool *cancelled;
            loom::events::EventType getType() { return loom::events::EventType::Base; };
        };

        class EventManager
        {
        public:
            std::vector<std::vector<std::unique_ptr<EventListener<loom::events::EventType>>>> listeners_;

            template <typename ET>
            std::unique_ptr<EventListener<ET>> on(EventCallback callback)
            {
                auto listener = std::make_unique<EventListener<ET>>(callback, shared_from_this());
                listeners_[ET].push_back(std::move(listener));
                return listener;
            }

            void dispatch(loom::events::Event *event)
            {
                auto eventType = event->getType();

                for (auto &listener : listeners_[static_cast<std::size_t>(eventType)])
                {
                    if (!event->cancelled)
                    {
                        listener->callback_(event); // Now passing the event object
                    }
                }
            }
        };

        template <typename UnderlyingType = std::underlying_type<EventType>::type>
        class EventListener
        {
        public:
            EventCallback callback_;
            std::shared_ptr<EventManager> manager_;

            EventListener(EventCallback callback, const std::shared_ptr<EventManager> &manager)
                : callback_(callback), manager_(manager) {}

            void stop()
            {
                manager_->listeners_[static_cast<std::size_t>(ET)].erase(
                    std::remove_if(manager_->listeners_[static_cast<std::size_t>(ET)].begin(),
                                   manager_->listeners_[static_cast<std::size_t>(ET)].end(),
                                   [this](const std::unique_ptr<EventListener<ET>> &listener)
                                   {
                                       return listener.get() == this;
                                   }),
                    manager_->listeners_[static_cast<std::size_t>(ET)].end());
            }
        };
    }
}

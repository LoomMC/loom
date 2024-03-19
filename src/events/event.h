#pragma once

enum EventTypes
{
    OnFrame
};

class Event {
public:
    bool* cancelled;
};
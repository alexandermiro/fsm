#ifndef EVENT_USER_ACTION_H
#define EVENT_USER_ACTION_H

#include <cstdint>

struct EventUserAction
{
    enum class Type : uint16_t 
    {
          None = 0
        , StartCharge
        , Pause
        , Cancel
        , ChargerInfo
    };

    Type type{Type::None};
};

#endif  // EVENT_USER_ACTION_H
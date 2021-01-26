#ifndef CORE_ROOM_H
#define CORE_ROOM_H

#include <condition_variable>

namespace Core {

class Room
{
    std::condition_variable con_var;
public:
    Room();
};

} // namespace Core

#endif // CORE_ROOM_H

#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include <iostream>
#include <string>
#include <thread>
#include <queue>
#include "trinputstream.h"
#include "troutputstream.h"
#include "ifinputstream.h"
#include "ifoutputstream.h"
#include "pkinputstream.h"
#include "pkoutputstream.h"
#include "enums.h"
#include "ifsystem.h"
#include "pool.h"
#include "room.h"
#include "module.h"
#include "datapack.h"

namespace Core {

class System
{
    id_type m_system = -1;
    id_type m_input = -1;
    id_type m_output = -1;
    id_type r_main_room = -1;
    Pool<Room> rooms;
    Pool<Module> modules;
    Pool<DataPack> data;
public:
    System();
    ~System();
    int exec();
};

} // namespace Core

#endif // CORE_SYSTEM_H

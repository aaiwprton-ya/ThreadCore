#ifndef CORE_DATAPACK_H
#define CORE_DATAPACK_H

#include "package.h"

namespace Core {

class DataPack
{
public:
    Package const *pack;
public:
    DataPack(Package *pack);
};

} // namespace Core

#endif // CORE_DATAPACK_H

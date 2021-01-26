#ifndef CORE_IFSYSTEM_H
#define CORE_IFSYSTEM_H

#include "interface.h"
#include "defines.h"

namespace Core {

class IFSystem : public Core::Interface
{
public:
    IFSystem(System *system, id_type room, State state = INIT);
public:
    bool execute(id_type module, std::string cmd) override;
};

} // namespace Core

#endif // CORE_IFSYSTEM_H

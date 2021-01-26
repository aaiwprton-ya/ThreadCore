#include "ifsystem.h"

namespace Core {

IFSystem::IFSystem(System *system, id_type room, State state)
    :
      Core::Interface(CORE_IFSYSTEM, system, room, state)
{}

bool IFSystem::execute(id_type module, std::string cmd)
{
    // TODO: ecexute command
    return false;
}

} // namespace Core

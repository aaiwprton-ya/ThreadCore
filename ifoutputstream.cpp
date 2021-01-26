#include "ifoutputstream.h"

namespace Core {

IFOutputStream::IFOutputStream(System *system, id_type room, State state)
    :
      Core::Interface(CORE_IFOUTPUTSTREAM, system, room, state)
{}

bool IFOutputStream::execute(id_type module, std::string cmd)
{
    // TODO: execute command
    return false;
}

} // namespace Core

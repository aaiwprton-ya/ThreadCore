#include "ifinputstream.h"

namespace Core {

IFInputStream::IFInputStream(System *system, id_type room, State state)
    :
      Core::Interface(CORE_IFINPUTSTREAM, system, room, state)
{}

bool IFInputStream::execute(id_type module, std::string cmd)
{
    bool permision = false;
    // slow find
    for (auto c: connects)
    {
        if (module == c)
        {
            permision = true;
            break;
        }
    }
    if (permision)
    {
        // TODO: execute command
        return true;
    }
    return false;
}

} // namespace Core

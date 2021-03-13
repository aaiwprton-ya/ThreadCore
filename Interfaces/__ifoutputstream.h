#ifndef CORE_IFOUTPUTSTREAM_H
#define CORE_IFOUTPUTSTREAM_H

#include <string>
#include <queue>
#include "interface.h"
#include "defines.h"

namespace Core {

class IFOutputStream : public Core::Interface
{
public:
    IFOutputStream(System *system, id_type room, State state = INIT);
public:
    bool execute(id_type module, std::string cmd) override;
};

} // namespace Core

#endif // CORE_IFOUTPUTSTREAM_H

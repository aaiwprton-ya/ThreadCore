#ifndef CORE_IFINPUTSTREAM_H
#define CORE_IFINPUTSTREAM_H

#include <string>
#include <queue>
#include "interface.h"
#include "defines.h"

namespace Core {

class IFInputStream : public Core::Interface
{
public:
    IFInputStream(System *system, id_type room,  State state = INIT);
public:
    bool execute(id_type module, std::string cmd) override;
};

} // namespace Core

#endif // CORE_IFINPUTSTREAM_H

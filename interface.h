#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H

#include <string>
#include <vector>
#include "id.h"
#include "enums.h"

namespace Core {

class System;

class Interface
{
public:
    const std::string name = "unnamed";
protected:
    System *system = nullptr;
    const id_type room = -1;
public:
    std::vector<id_type> connects;
    std::vector<id_type> packages;
    State state = INIT;
public:
    Interface(std::string name, System *system, id_type room,  State state = INIT);
    virtual ~Interface();
public:
    virtual bool execute(id_type module, std::string cmd) = 0;
};

} // namespace Core

#endif // CORE_INTERFACE_H

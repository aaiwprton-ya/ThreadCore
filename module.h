#ifndef CORE_MODULE_H
#define CORE_MODULE_H

#include "interface.h"

namespace Core {

class Module
{
public:
    Interface const *iface;
public:
    Module(Interface *iface);
};

} // namespace Core

#endif // CORE_MODULE_H

#ifndef CORE_SYSTEM_H
#define CORE_SYSTEM_H

#include "std_afx.h"
#include "Utils/pool.h"
#include "Core/package.h"
#include "Core/interface.h"
#include "Core/module.h"

namespace Core {

using namespace Utils;

class Package;
class Interface;
class Module;

class System
{
    Pool<Package> packs;
    Pool<Interface> ifaces;
    Pool<std::thread> threads;
    std::map<std::string, Module*> modules;
public:
    System();
    ~System();
public:
    int exec();
    void include_module(Module *module);
private:
    void init_modules();
};

} // namespace Core

#endif // CORE_SYSTEM_H

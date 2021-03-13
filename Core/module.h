#ifndef CORE_MODULE_H
#define CORE_MODULE_H

#include "std_afx.h"
#include "Utils/pool.h"

namespace Core {

//using namespace Utils;

class Module
{
/*
 * Package:
 *  - name
 *  - key
 *  - type
 *
 * Interface:
 *  - name
 *  - key
 *
 * Thread object:
 *  - temp point to the interface
 *  - type
 *  - key for thread
 *
 * Thread:
 *  - key from the thread object
 *  - temp point to the thread object
 *
 * 1) names the other moduls for the interfase access
 * 2) pairs of names the other objects and them package names for the packages access
 * 3) lists of module names for all of them packages
*/
protected:
    std::vector<std::string> iface_links;
    std::map<std::string, std::string> pack_links;
    std::map<std::string, std::vector<std::pair<bool, std::string>>> thread_links;
public:
    const std::string name;
public:
    Module(std::string name);
    virtual ~Module();
    virtual void generate_packages(Pool<Package> *packs) const noexcept = 0;
    virtual void generate_interface(Pool<Interface> *ifaces) const noexcept = 0;
    virtual void link_interfaces(Pool<Interface> *ifaces, std::map<std::string, Module*> *modules) const noexcept = 0;
    virtual void generate_thread(Pool<std::thread> *threads, Pool<Interface> *ifaces, Pool<Package> *packs, std::map<std::string, Module*> *modules) const noexcept = 0;
    virtual void link_threads(Pool<std::thread> *threads, Pool<Package> *packs, std::map<std::string, Module*> *modules) const noexcept = 0;
    virtual void disconnect(Pool<std::thread> *threads, Pool<Package> *packs, std::map<std::string, Module*> *modules) const noexcept = 0;
public:
    virtual Interface::__BaseIfaceKey__ get_if_key() const noexcept = 0;
    virtual void get_pack_key(const Package::__BasePackKey__ *&p_key, std::string pack_name) const noexcept = 0;
    virtual void get_thread_key(const Thread::__BaseThreadKey__ *&t_key) const noexcept = 0;
};

} // namespace Core

#endif // CORE_MODULE_H

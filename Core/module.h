#ifndef CORE_MODULE_H
#define CORE_MODULE_H

#include "std_afx.h"
#include "Utils/pool.h"

namespace Core {

typedef std::vector<std::string> IFACE_LINKS;
typedef std::string IFACE_LINK;
typedef std::map<std::string, std::string> PACK_LINKS;
typedef std::pair<std::string, std::string> PACK_LINK;
typedef std::pair<bool, std::string> TH_LINK;
typedef std::vector<TH_LINK> TH_LINKS;
typedef std::map<std::string, TH_LINKS> THREAD_LINKS;
typedef std::pair<std::string, TH_LINKS> THREAD_LINK;

typedef Package::__BasePackKey__ BASE_PKEY;
typedef std::map<std::string, BASE_PKEY*> PK_KEYS;
typedef std::pair<std::string, BASE_PKEY*> PK_KEY;
typedef Interface::__BaseIfaceKey__ BASE_IFKEY;

class Module
{
protected:
    IFACE_LINKS iface_links;
    PACK_LINKS pack_links;
    THREAD_LINKS thread_links;
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

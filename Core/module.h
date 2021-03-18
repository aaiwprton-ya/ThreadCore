#ifndef CORE_MODULE_H
#define CORE_MODULE_H

#include "std_afx.h"
#include "Utils/pool.h"

namespace Core {

typedef std::vector<std::string> IFACE_LINKS;
typedef std::string IFACE_LINK;
typedef std::vector<std::pair<std::string, std::string>> PACK_LINKS;
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
    PACK_LINKS pack_links;          // this is link of this thread to the module-package pair (name module, name package)
    THREAD_LINKS thread_links;      // this is link for all of those packages to the type-module pair (name package , list of pairs (bool, name module))
public:
    const std::string name;
public:
    Module(std::string name);
    virtual ~Module();
    virtual void generate_packages(Pool<Package> *packs) const noexcept = 0;
    virtual void generate_interface(Pool<Interface> *ifaces) const noexcept = 0;
    virtual void link_interfaces(Pool<Interface> *ifaces,
                                 std::map<std::string,
                                 Module*> *modules) const noexcept = 0;
    virtual void generate_thread(Pool<std::thread> *threads,
                                 Pool<Interface> *ifaces,
                                 Pool<Package> *packs,
                                 std::map<std::string,
                                 Module*> *modules) const noexcept = 0;
    virtual void link_threads(Pool<std::thread> *threads,
                              Pool<Package> *packs,
                              std::map<std::string,
                              Module*> *modules) const noexcept = 0;
    virtual void disconnect(Pool<std::thread> *threads,
                            Pool<Package> *packs,
                            std::map<std::string,
                            Module*> *modules) const noexcept = 0;
public:
    virtual Interface::__BaseIfaceKey__ get_if_key() const noexcept = 0;
    virtual void get_pack_key(const BASE_PKEY *&p_key,
                              std::string pack_name) const noexcept = 0;
    virtual void get_thread_key(const BASE_THKEY *&t_key) const noexcept = 0;
protected:
    BASE_PKEY *_get_pack_key_(const std::string &pack_name,
                              const PACK_LINKS &this_packs,
                              const PACK_LINKS &link_packs,
                              const PK_KEYS &pk_keys) const noexcept;
};

} // namespace Core

#endif // CORE_MODULE_H

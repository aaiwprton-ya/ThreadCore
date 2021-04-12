#ifndef CORE_MODULE_H
#define CORE_MODULE_H

#include "std_afx.h"
#include "Utils/pool.h"

namespace Core {

typedef Pool<std::thread> THRAEDS;
typedef Pool<Interface> INTERFACES;
typedef Pool<Package> PACKAGES;

enum AccessType { AT_UNIQUE, AT_SHARED };

typedef std::pair<AccessType, MODULE_NAME> ACCESS_VALUE;
typedef std::vector<ACCESS_VALUE> ACCESS_TO_PACK;
typedef std::map<PACKAGE_NAME, ACCESS_TO_PACK> ACCESS_SETTINGS;
typedef std::pair<PACKAGE_NAME, ACCESS_TO_PACK> ACCESS_ITEM;

typedef std::vector<MODULE_NAME> IFACE_LINKS;
typedef std::string IFACE_LINK;
typedef std::vector<std::pair<MODULE_NAME, PACKAGE_NAME>> PACK_LINKS;
typedef std::pair<MODULE_NAME, PACKAGE_NAME> PACK_LINK;

typedef Package::__BasePackKey__ BASE_PKEY;
typedef std::map<PACKAGE_NAME, BASE_PKEY*> PK_KEYS;
typedef std::pair<PACKAGE_NAME, BASE_PKEY*> PK_KEY;
typedef Interface::__BaseIfaceKey__ BASE_IFKEY;

struct ModuleSettings
{
    MODULE_NAME self_name;
    PACK_LINKS own_packs;                           // this is link of this thread to the module-package pair (name module, name package)
    PACK_LINKS linked_packs;                        // this is link of this thread to the module-package pair (name module, name package)
    IFACE_LINKS subordinate_iface_links;
    IFACE_LINKS controlled_iface_links;
    IFACE_LINKS concurrent_iface_links;
    ACCESS_SETTINGS access_settings;                // this is link for all of those packages to the type-module pair (name package , list of pairs (bool, name module))
    ModuleSettings(std::string self_name,
                   PACK_LINKS own_packs,
                   PACK_LINKS linked_packs,
                   IFACE_LINKS subordinate_iface_links,
                   IFACE_LINKS controlled_iface_links,
                   IFACE_LINKS concurrent_iface_links,
                   ACCESS_SETTINGS access_settings)
        : self_name(self_name),
          own_packs(own_packs),
          linked_packs(linked_packs),
          subordinate_iface_links(subordinate_iface_links),
          controlled_iface_links(controlled_iface_links),
          concurrent_iface_links(concurrent_iface_links),
          access_settings(access_settings) {}
};

class Module
{
protected:
    const ModuleSettings m_set;
    PK_KEYS pk_keys;
    BASE_IFKEY if_key;
public:
    const MODULE_NAME name;
    BASE_THKEY *th_key;
public:
    Module(const ModuleSettings &m_set, BASE_THKEY *th_key);
    virtual ~Module();
    virtual void generate_packages(Pool<Package> *packs) const noexcept = 0;
    virtual void generate_interface(Pool<Interface> *ifaces) const noexcept;
    virtual void link_interfaces(Pool<Interface> *ifaces,
                                 std::map<std::string,
                                 Module*> *modules) const noexcept;
    virtual void generate_thread(Pool<std::thread> *threads,
                                 Pool<Interface> *ifaces,
                                 Pool<Package> *packs,
                                 std::map<std::string,
                                 Module*> *modules) const noexcept = 0;
    virtual void link_threads(Pool<std::thread> *threads,
                              Pool<Package> *packs,
                              std::map<std::string,
                              Module*> *modules) const noexcept;
    virtual void disconnect(Pool<std::thread> *threads,
                            Pool<Package> *packs,
                            std::map<std::string,
                            Module*> *modules) const noexcept;
protected:
    template<class ThreadType, class KeyType>
    void append_packs(ThreadType &&thread,
                      Pool<std::thread> *threads,
                      Pool<Package> *packs,
                      std::map<std::string,
                      Module*> *modules,
                      KeyType th_key) const noexcept
    {
        {
            const Package::__BasePackKey__ *p_key = nullptr;
            for (auto &item: m_set.own_packs)
            {
                modules->at(item.first)->get_pack_key(p_key, item.second);
                thread.append_pack(&(*packs)[*p_key]);
                p_key = nullptr;
            }
            for (auto &item: m_set.linked_packs)
            {
                modules->at(item.first)->get_pack_key(p_key, item.second);
                thread.append_pack(&(*packs)[*p_key]);
                p_key = nullptr;
            }
            thread._init_pack_pointers_();
            (*threads)[th_key] = std::thread(std::move(thread));
        }
    }
public:
    BASE_IFKEY get_if_key() const noexcept;
    void get_pack_key(const BASE_PKEY *&p_key, PACKAGE_NAME pack_name) const noexcept;
    void get_thread_key(const BASE_THKEY *&t_key) const noexcept;
protected:
    BASE_PKEY *_get_pack_key_(const std::string &pack_name,
                              const PACK_LINKS &this_packs,
                              const PACK_LINKS &link_packs,
                              const PK_KEYS &pk_keys) const noexcept;
};

typedef std::map<std::string, Module*> MODULES;

} // namespace Core

#endif // CORE_MODULE_H

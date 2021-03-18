#ifndef MD_IOSYSTEM_H
#define MD_IOSYSTEM_H

#include "std_afx.h"
#include "Core/module.h"
#include "Core/thread.h"
#include "Modules/pk_iosystem.h"

namespace md_iosystem
{

const std::string M_NAME = "IOSystem";
const std::vector<std::string> P_NAME = { "OutputQueue" };

using namespace Core;

typedef class TH_IOSystem : public Thread
{
public:
    TH_IOSystem(Interface *iface);
public:
    bool thread() override;
public:
    class Key : public __BaseThreadKey__ {
    public: typedef std::thread type; };
} ThreadType;

typedef class MD_IOSystem : public Module
{
    PK_KEYS pk_keys;
    BASE_IFKEY if_key;
    ThreadType::Key th_key;
public:
    MD_IOSystem();
    ~MD_IOSystem();
public:
    void generate_packages(Pool<Package> *packs) const noexcept override;
    void generate_interface(Pool<Interface> *ifaces) const noexcept override;
    void link_interfaces(Pool<Interface> *ifaces,
                         std::map<std::string,
                         Module*> *modules) const noexcept override;
    void generate_thread(Pool<std::thread> *threads,
                         Pool<Interface> *ifaces,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override;
    void link_threads(Pool<std::thread> *threads,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override;
    void disconnect(Pool<std::thread> *threads,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override;
public:
    BASE_IFKEY get_if_key() const noexcept override;
    void get_pack_key(const BASE_PKEY *&p_key, std::string pack_name) const noexcept override;
    void get_thread_key(const BASE_THKEY *&t_key) const noexcept override;
} ModuleType;

} // md_iosystem

#endif // MD_IOSYSTEM_H

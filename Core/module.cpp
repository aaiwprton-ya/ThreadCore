#include "module.h"

namespace Core {

Module::Module(const ModuleSettings &m_set, BASE_THKEY *th_key)
    : m_set(m_set),
      name(m_set.self_name),
      th_key(th_key) {}

Module::~Module() {}

void Module::generate_interface(Pool<Interface> *ifaces) const noexcept {
    (*ifaces)[if_key] = Interface(m_set.self_name); }

void Module::link_interfaces(Pool<Interface> *ifaces, std::map<std::string, Module*> *modules) const noexcept
{
    for (auto &m_name: m_set.subordinate_iface_links) {
        (*ifaces)[if_key].append_subordinate_iface(&(*ifaces)[modules->at(m_name)->get_if_key()]); }
    for (auto &m_name: m_set.controlled_iface_links) {
        (*ifaces)[if_key].append_controlled_iface(&(*ifaces)[modules->at(m_name)->get_if_key()]); }
    for (auto &m_name: m_set.concurrent_iface_links) {
        (*ifaces)[if_key].append_concurrent_iface(&(*ifaces)[modules->at(m_name)->get_if_key()]); }
}

void Module::link_threads(Pool<std::thread> *threads, Pool<Package> *packs, std::map<std::string, Module *> *modules) const noexcept
{
    const Package::__BasePackKey__ *p_key = nullptr;
    for (auto &item: m_set.access_settings)
    {
        p_key = _get_pack_key_(item.first, m_set.own_packs, m_set.linked_packs, pk_keys);
        if (p_key == nullptr) {
            continue; }
        const Thread::__BaseThreadKey__ *t_key = nullptr;
        for (auto &i_second: item.second)
        {
            modules->at(i_second.second)->get_thread_key(t_key);
            if (i_second.first == AT_SHARED) {
                (*packs)[*p_key].connect_sh_lock((*threads)[*t_key].get_id()); }
            else {
                (*packs)[*p_key].connect_un_lock((*threads)[*t_key].get_id()); }
            t_key = nullptr;
        }
        p_key = nullptr;
    }
}

void Module::disconnect(Pool<std::thread> *threads, Pool<Package> *packs, std::map<std::string, Module *> *modules) const noexcept
{
    const Package::__BasePackKey__ *p_key = nullptr;
    for (auto &item: m_set.access_settings)
    {
        p_key = _get_pack_key_(item.first, m_set.own_packs, m_set.linked_packs, pk_keys);
        if (p_key == nullptr) {
            continue; }
        const Thread::__BaseThreadKey__ *t_key = nullptr;
        for (auto &i_second: item.second)
        {
            modules->at(i_second.second)->get_thread_key(t_key);
            if (i_second.first == AT_SHARED) {
                (*packs)[*p_key].disconnect_sh_lock((*threads)[*t_key].get_id()); }
            else {
                (*packs)[*p_key].disconnect_un_lock((*threads)[*t_key].get_id()); }
            t_key = nullptr;
        }
        p_key = nullptr;
    }
}

BASE_IFKEY Module::get_if_key() const noexcept {
    return if_key; }

void Module::get_pack_key(const BASE_PKEY *&p_key, PACKAGE_NAME pack_name) const noexcept
{
    p_key = nullptr;
    for (auto &item: m_set.own_packs)
    {
        if (pack_name == item.second)
        {
            p_key = pk_keys.at(item.second);
            break;
        }
    }
}

void Module::get_thread_key(const BASE_THKEY *&t_key) const noexcept {
    t_key = th_key; }

BASE_PKEY *Module::_get_pack_key_(const std::string &pack_name,
                                  const PACK_LINKS &this_packs,
                                  const PACK_LINKS &link_packs,
                                  const PK_KEYS &pk_keys) const noexcept
{
    Package::__BasePackKey__ *p_key = nullptr;
    for (auto &_item: this_packs)
    {
        if (pack_name == _item.second) {
            p_key = pk_keys.at(_item.second); }
        if (p_key != nullptr) {
            break; }
    }
    for (auto &_item: link_packs)
    {
        if (pack_name == _item.second) {
            p_key = pk_keys.at(_item.second); }
        if (p_key != nullptr) {
            break; }
    }
    return p_key;
}

} // namespace Core

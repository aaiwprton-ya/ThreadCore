#ifndef MD_IOSYSTEM_H
#define MD_IOSYSTEM_H

#include "std_afx.h"
#include <iostream>
#include <list>
#include "Core/module.h"
#include "Core/thread.h"
#include "Core/package.h"

namespace md_iosystem
{

const std::string M_NAME = "IOSystem";
const std::vector<std::string> P_NAME = {"OutputQueue"};

using namespace Core;

class PK_OutputQueue : public Package
{
    std::list<std::string> _queue;
public:
    Access::To<std::list<std::string>> queue;
public:
    PK_OutputQueue() : queue(&_queue, sizeof(_queue), getP_base()) {}
    PK_OutputQueue(std::string name, std::string modul_name, std::thread::id system)
        : Package(name, modul_name, system),
          queue(&_queue, sizeof(_queue), getP_base()) {}
public:
    PK_OutputQueue &operator=(PK_OutputQueue &&pack) {
        static_cast<Package&>(*this) = std::move(pack);
        return *this; }
public:
    class Key : public __BasePackKey__ {
    public: typedef PK_OutputQueue type; };
};

class TH_IOSystem : public Thread
{
public:
    TH_IOSystem(Interface *iface)
        : Thread(iface) {
        _add_pack_link_(0, std::string(P_NAME[0]), std::string(M_NAME)); }                                                  // name
public:
    bool thread() override
    {
        std::string cmd;
        while (true)
        {
            packs[0]->unique_lock();
            for (std::string str: static_cast<PK_OutputQueue*>(packs[0])->queue.get()) {       // pack type
                std::cout << str << std::endl; }
            packs[0]->unlock();
            std::cin >> cmd;
            if (cmd == "exit") {
                break; }
            packs[0]->unique_lock();
            static_cast<PK_OutputQueue*>(packs[0])->queue.get().push_back(cmd);
            packs[0]->unlock();
        }
        return false;
    }
public:
    class Key : public __BaseThreadKey__ {
    public: typedef std::thread type; };
};

class MD_IOSystem : public Module
{
public:
    const std::string name = M_NAME;
    std::map<std::string, Package::__BasePackKey__*> pk_keys;
    Interface::__BaseIfaceKey__ if_key;
    TH_IOSystem::Key th_key;
public:
    MD_IOSystem();
    ~MD_IOSystem();
public:
    void generate_packages(Pool<Package> *packs) const noexcept override {
        (*packs)[*static_cast<PK_OutputQueue::Key*>(pk_keys.at(P_NAME[0]))] = PK_OutputQueue(P_NAME[0], M_NAME, std::this_thread::get_id()); }
    void generate_interface(Pool<Interface> *ifaces) const noexcept override {
        (*ifaces)[if_key] = Interface(); }
    void link_interfaces(Pool<Interface> *ifaces, std::map<std::string, Module*> *modules) const noexcept override
    {
        for (auto m_name: iface_links) {
            (*ifaces)[if_key].append_iface(&(*ifaces)[modules->at(m_name)->get_if_key()]); }
    }
    void generate_thread(Pool<std::thread> *threads,
                         Pool<Interface> *ifaces,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override
    {
        TH_IOSystem thread(&(*ifaces)[if_key]);
        const Package::__BasePackKey__ *p_key = nullptr;
        for (auto item: pack_links)
        {
            modules->at(item.first)->get_pack_key(p_key, item.second);
            thread.append_pack(&(*packs)[*p_key]);                                          // ?????????????????????????????????? base type key pointer
            p_key = nullptr;
        }
        (*threads)[th_key] = std::thread(std::move(thread));
    }
    void link_threads(Pool<std::thread> *threads,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override
    {
        const Package::__BasePackKey__ *p_key = nullptr;
        for (auto item: thread_links)
        {
            if (item.first == P_NAME[0]) {
                p_key = pk_keys.at(P_NAME[0]); }
            else {
                continue; }
            const Thread::__BaseThreadKey__ *t_key = nullptr;
            for (auto i_second: item.second)
            {
                modules->at(i_second.second)->get_thread_key(t_key);
                if (i_second.first) {
                    (*packs)[*pk_keys.at(P_NAME[0])].connect_sh_lock((*threads)[*t_key].get_id()); }
                else {
                    (*packs)[*pk_keys.at(P_NAME[0])].connect_un_lock((*threads)[*t_key].get_id()); }
                t_key = nullptr;
            }
        }
    }
    void disconnect(Pool<std::thread> *threads,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override
    {
        const Package::__BasePackKey__ *p_key = nullptr;
        for (auto item: thread_links)
        {
            if (item.first == P_NAME[0]) {
                p_key = pk_keys.at(P_NAME[0]); }
            else {
                continue; }
            const Thread::__BaseThreadKey__ *t_key = nullptr;
            for (auto i_second: item.second)
            {
                modules->at(i_second.second)->get_thread_key(t_key);
                if (i_second.first) {
                    (*packs)[*pk_keys.at(P_NAME[0])].disconnect_sh_lock((*threads)[*t_key].get_id()); }
                else {
                    (*packs)[*pk_keys.at(P_NAME[0])].disconnect_un_lock((*threads)[*t_key].get_id()); }
                t_key = nullptr;
            }
        }
    }
public:
    Interface::__BaseIfaceKey__ get_if_key() const noexcept override {
        return if_key; }
    void get_pack_key(const Package::__BasePackKey__ *&p_key, std::string pack_name) const noexcept override
    {
        if (pack_name == P_NAME[0]) {
            p_key = pk_keys.at(P_NAME[0]); }
    }
    void get_thread_key(const Thread::__BaseThreadKey__ *&t_key) const noexcept override {
        t_key = &th_key; }
};

} // md_iosystem


#endif // MD_IOSYSTEM_H

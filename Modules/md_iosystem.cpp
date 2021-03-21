#include "md_iosystem.h"

namespace md_iosystem
{

ThreadType::TH_IOSystem(Interface *iface)
    : Thread(iface)
{
    int i = 0;
    for (auto &item: THIS_PACKS) {
        _add_pack_link_(i++, std::string(item.first), std::string(item.second)); }
    for (auto &item: LINK_PACKS) {
        _add_pack_link_(i++, std::string(item.first), std::string(item.second)); }
}

bool ThreadType::thread()
{
    std::string cmd;
    PK_OutputQueue *p_output_queue = static_cast<PK_OutputQueue*>(packs[OUTPUT_QUEUE]);
    while (true)
    {
        p_output_queue->unique_lock();
        for (std::string &str: p_output_queue->queue.get()) {
            std::cout << str << std::endl; }
        p_output_queue->unlock();
        std::cin >> cmd;
        if (cmd == "stop")
        {
            if (iface->get_iface(0)->get_state() == TS_RUN) {
            iface->get_iface(0)->send_command(CV_STOP); }
        }
        if (cmd == "start")
        {
            if (iface->get_iface(0)->get_state() == TS_STOP) {
            iface->get_iface(0)->activate(); }
        }
        if (cmd == "exit")
        {
            iface->send_command_all(CV_TERMINATE);
            iface->try_activate_all();
            break;
        }
        p_output_queue->unique_lock();
        p_output_queue->queue.get().push_back(cmd);
        p_output_queue->unlock();
    }
    return false;
}

ModuleType::MD_IOSystem()
    : Module(THIS_NAME)
{
    pk_keys.insert(PK_KEY(THIS_PACKS[OUTPUT_QUEUE].second, new PK_OutputQueue::Key()));
    for (auto &item: THIS_PACKS)
    {
        pack_links.push_back(PACK_LINK(item.first, item.second));
        thread_links.insert(THREAD_LINK(item.second, TH_LINKS({ TH_LINK(false, item.first) })));
    }
    for (auto &item: LINK_PACKS) {
        pack_links.push_back(PACK_LINK(item.first, item.second)); }
    for (auto &item: LINK_IFACES) {
        iface_links.push_back(item); }
}

ModuleType::~MD_IOSystem()
{
    for (auto &p_key: pk_keys) {
        delete p_key.second; }
}

void ModuleType::generate_packages(Pool<Package> *packs) const noexcept
{
    (*packs)[*static_cast<PK_OutputQueue::Key*>(pk_keys.at(THIS_PACKS[OUTPUT_QUEUE].second))]
            = PK_OutputQueue(THIS_PACKS[OUTPUT_QUEUE].first,
                             THIS_PACKS[OUTPUT_QUEUE].second,
                             std::this_thread::get_id());
}

void ModuleType::generate_interface(Pool<Interface> *ifaces) const noexcept {
    (*ifaces)[if_key] = Interface(); }

void ModuleType::link_interfaces(Pool<Interface> *ifaces,
                                  std::map<std::string,
                                  Module *> *modules) const noexcept
{
    for (auto &m_name: iface_links) {
        (*ifaces)[if_key].append_iface(&(*ifaces)[modules->at(m_name)->get_if_key()]); }
}

void ModuleType::generate_thread(Pool<std::thread> *threads,
                                  Pool<Interface> *ifaces,
                                  Pool<Package> *packs,
                                  std::map<std::string,
                                  Module*> *modules) const noexcept
{
    ThreadType thread(&(*ifaces)[if_key]);
    const Package::__BasePackKey__ *p_key = nullptr;
    for (auto &item: pack_links)
    {
        modules->at(item.first)->get_pack_key(p_key, item.second);
        thread.append_pack(&(*packs)[*p_key]);
        p_key = nullptr;
    }
    (*threads)[th_key] = std::thread(std::move(thread));
}

void ModuleType::link_threads(Pool<std::thread> *threads,
                               Pool<Package> *packs,
                               std::map<std::string,
                               Module *> *modules) const noexcept
{
    const Package::__BasePackKey__ *p_key = nullptr;
    for (auto &item: thread_links)
    {
        p_key = _get_pack_key_(item.first, THIS_PACKS, LINK_PACKS, pk_keys);
        if (p_key == nullptr) {
            continue; }
        const Thread::__BaseThreadKey__ *t_key = nullptr;
        for (auto &i_second: item.second)
        {
            modules->at(i_second.second)->get_thread_key(t_key);
            if (i_second.first) {
                (*packs)[*p_key].connect_sh_lock((*threads)[*t_key].get_id()); }
            else {
                (*packs)[*p_key].connect_un_lock((*threads)[*t_key].get_id()); }
            t_key = nullptr;
        }
        p_key = nullptr;
    }
}

void ModuleType::disconnect(Pool<std::thread> *threads,
                             Pool<Package> *packs,
                             std::map<std::string,
                             Module *> *modules) const noexcept
{
    const Package::__BasePackKey__ *p_key = nullptr;
    for (auto &item: thread_links)
    {
        p_key = _get_pack_key_(item.first, THIS_PACKS, LINK_PACKS, pk_keys);
        if (p_key == nullptr) {
            continue; }
        const Thread::__BaseThreadKey__ *t_key = nullptr;
        for (auto &i_second: item.second)
        {
            modules->at(i_second.second)->get_thread_key(t_key);
            if (i_second.first) {
                (*packs)[*p_key].disconnect_sh_lock((*threads)[*t_key].get_id()); }
            else {
                (*packs)[*p_key].disconnect_un_lock((*threads)[*t_key].get_id()); }
            t_key = nullptr;
        }
        p_key = nullptr;
    }
}

BASE_IFKEY ModuleType::get_if_key() const noexcept {
    return if_key; }

void ModuleType::get_pack_key(const BASE_PKEY *&p_key, std::string pack_name) const noexcept
{
    for (auto &item: THIS_PACKS)
    {
        if (pack_name == item.second) {
            p_key = pk_keys.at(item.second); }
    }
}

void ModuleType::get_thread_key(const BASE_THKEY *&t_key) const noexcept {
    t_key = &th_key; }

} // md_iosystem

#include "md_iosystem.h"

namespace md_iosystem
{

ThreadType::TH_IOSystem(Interface *iface)
    : Thread(iface)
{
    int i = 0;
    for (auto &item: m_settings.own_packs) {
        _add_pack_link_(i++, MODULE_NAME(item.first), PACKAGE_NAME(item.second)); }
    for (auto &item: m_settings.linked_packs) {
        _add_pack_link_(i++, MODULE_NAME(item.first), PACKAGE_NAME(item.second)); }
}

bool ThreadType::thread()
{
    while (true)
    {
        p_output_queue->unique_lock();
        for (std::string &str: p_output_queue->queue.get()) {
            std::cout << str << std::endl; }
        p_output_queue->queue.get().clear();
        p_output_queue->unlock();
        std::cout << ">>> ";
        std::cin >> cmd;
        if (cmd == "stop")
        {
            iface->get_controlled_iface(CTRI_LFReader)->send_command(CV_STOP);
        }
        if (cmd == "start")
        {
            iface->get_controlled_iface(CTRI_LFReader)->send_command(CV_START);
        }
        if (cmd == "disable")
        {
            iface->get_controlled_iface(CTRI_LFReader)->send_command(CV_DISABLE);
        }
        if (cmd == "enable")
        {
            iface->get_controlled_iface(CTRI_LFReader)->send_command(CV_ENABLE);
        }
        if (cmd == "state")
        {
            for (auto &item: iface->get_controlled_ifaces()) {
                std::cout << item->get_name() << ": " << ts_to_str(item->get_state()) << std::endl; }
        }
        if (cmd == "exit")
        {
            iface->send_command_all(CV_TERMINATE);
            iface->send_command(CV_TERMINATE);
            break;
        }
    }
    return true;
}

void TH_IOSystem::_init_pack_pointers_() noexcept {
    p_output_queue = static_cast<PK_OutputQueue*>(packs[OUTPUT_QUEUE]); }

ModuleType::MD_IOSystem()
    : Module(m_settings, &th_key) {
    pk_keys.insert(PK_KEY(m_set.own_packs[OUTPUT_QUEUE].second, new PK_OutputQueue::Key())); }

ModuleType::~MD_IOSystem()
{
    for (auto &p_key: pk_keys) {
        delete p_key.second; }
}

void ModuleType::generate_packages(PACKAGES *packs) const noexcept
{
    (*packs)[*static_cast<PK_OutputQueue::Key*>(pk_keys.at(m_set.own_packs[OUTPUT_QUEUE].second))]
            = PK_OutputQueue(m_set.own_packs[OUTPUT_QUEUE].first,
                             m_set.own_packs[OUTPUT_QUEUE].second,
                             std::this_thread::get_id());
}

void ModuleType::generate_thread(THRAEDS *threads, INTERFACES *ifaces,
                                 PACKAGES *packs, MODULES *modules) const noexcept
{
    ThreadType thread(&(*ifaces)[if_key]);
    append_packs(std::move(thread), threads, packs, modules, th_key);
}

} // md_iosystem

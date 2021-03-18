#include "md_iosystem.h"

namespace md_iosystem
{

ThreadType::TH_IOSystem(Interface *iface)
    : Thread(iface) {
    _add_pack_link_(0, std::string(P_NAME[0]), std::string(M_NAME)); }

bool ThreadType::thread()
{
    //        cv::Mat mat = cv::imread("test_image.png");
    //        std::string title = "Test OpenCV";
    //        cv::imshow(title, mat);
    //        int key = cv::waitKey(0);
    std::string cmd;
    PK_OutputQueue *p_output_queue = static_cast<PK_OutputQueue*>(packs[0]);
    while (true)
    {
        p_output_queue->unique_lock();
        for (std::string &str: p_output_queue->queue.get()) {
            std::cout << str << std::endl; }
        p_output_queue->unlock();
        std::cin >> cmd;
        if (cmd == "exit") {
            break; }
        p_output_queue->unique_lock();
        p_output_queue->queue.get().push_back(cmd);
        p_output_queue->unlock();
    }
    return false;
}

ModuleType::MD_IOSystem()
    : Module(M_NAME)
{
    for (size_t i = 0; i < P_NAME.size(); ++i)
    {
        pk_keys.insert(PK_KEY(P_NAME[0], new PK_OutputQueue::Key()));
        pack_links.insert(PACK_LINK(M_NAME, P_NAME[0]));
        thread_links.insert(THREAD_LINK(P_NAME[0], TH_LINKS({ TH_LINK(false, M_NAME) })));
//        thread_links.insert(THREAD_LINK(P_NAME[0], TH_LINKS({ TH_LINK(false, "some_module_name") })));
    }
//    iface_links.push_back("some_module_name");
//    pack_links.insert(PACK_LINK("some_module_name", "some_package_name"));
}

ModuleType::~MD_IOSystem()
{
    for (auto &p_key: pk_keys) {
        delete p_key.second; }
}

void ModuleType::generate_packages(Pool<Package> *packs) const noexcept
{
    for (size_t i = 0; i < P_NAME.size(); ++i)
    {
        (*packs)[*static_cast<PK_OutputQueue::Key*>(pk_keys.at(P_NAME[i]))]
                = PK_OutputQueue(P_NAME[i], M_NAME, std::this_thread::get_id());
    }
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
                                  Module *> *modules) const noexcept
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
        if (item.first == P_NAME[0]) {
            p_key = pk_keys.at(P_NAME[0]); }
        else {
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
        if (item.first == P_NAME[0]) {
            p_key = pk_keys.at(P_NAME[0]); }
        else {
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
    }
}

BASE_IFKEY ModuleType::get_if_key() const noexcept {
    return if_key; }

void ModuleType::get_pack_key(const BASE_PKEY *&p_key, std::string pack_name) const noexcept
{
    for (size_t i = 0; i < P_NAME.size(); ++i)
    {
        if (pack_name == P_NAME[i]) {
            p_key = pk_keys.at(P_NAME[i]); }
    }
}

void ModuleType::get_thread_key(const BASE_THKEY *&t_key) const noexcept {
    t_key = &th_key; }

} // md_iosystem

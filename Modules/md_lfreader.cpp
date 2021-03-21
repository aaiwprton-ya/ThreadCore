#include "md_lfreader.h"

namespace md_lfreader
{

ThreadType::TH_LFReader(Interface *iface)
    : Thread(iface)
{
    int i = 0;
    for (auto &item: THIS_PACKS) {
        _add_pack_link_(i++, std::string(item.first), std::string(item.second)); }
    for (auto &item: LINK_PACKS) {
        _add_pack_link_(i++, std::string(item.first), std::string(item.second)); }
//    cap.open("test_video.mp4");
    cap.open(0);
    if (!cap.isOpened()) {
        throw std::runtime_error("error: VideoCapture wasn't open"); }
}

bool ThreadType::thread()
{
    PK_FrameBuffer *frame_buffer = static_cast<PK_FrameBuffer*>(packs[FRAME_BUFFER]);
    md_iosystem::PK_OutputQueue *output_queue = static_cast<md_iosystem::PK_OutputQueue*>(packs[OUTPUT_QUEUE + COUNT_PACKS]);
    frame_buffer->unique_lock();
    cap.read(frame_buffer->frame.get());
    if(frame_buffer->frame.get().empty()) {
        return false; }
    frame_buffer->unlock();
    frame_buffer->unique_lock();
    cv::imshow("title", frame_buffer->frame.get());
    frame_buffer->unlock();
    if (cv::waitKey(1) == 27) {
        return false; }
    return true;
}

ModuleType::MD_LFReader()
    : Module(THIS_NAME)
{
    pk_keys.insert(PK_KEY(THIS_PACKS[FRAME_BUFFER].second, new PK_FrameBuffer::Key()));
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

ModuleType::~MD_LFReader()
{
    for (auto &p_key: pk_keys) {
        delete p_key.second; }
}

void ModuleType::generate_packages(Pool<Package> *packs) const noexcept
{
    (*packs)[*static_cast<PK_FrameBuffer::Key*>(pk_keys.at(THIS_PACKS[OUTPUT_QUEUE].second))]
            = PK_FrameBuffer(THIS_PACKS[FRAME_BUFFER].first,
                             THIS_PACKS[FRAME_BUFFER].second,
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

} // md_lfreader

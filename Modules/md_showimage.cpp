#include "md_showimage.h"

namespace md_showimage
{

ThreadType::TH_ShowImage(Interface *iface)
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
    frame_buffer->unique_lock();
    if(frame_buffer->frame.get().empty())
    {
        frame_buffer->unlock();
        return true;
    }
    cv::imshow("title", frame_buffer->frame.get());
    frame_buffer->unlock();
    cv::waitKey(1);
    return true;
}

void TH_ShowImage::_init_pack_pointers_() noexcept
{
    frame_buffer = static_cast<PK_FrameBuffer*>(packs[FRAME_BUFFER]);
    output_queue = static_cast<md_iosystem::PK_OutputQueue*>(packs[OUTPUT_QUEUE + m_settings.own_packs.size()]);
}

ModuleType::MD_ShowImage()
    : Module(m_settings, &th_key) {
    pk_keys.insert(PK_KEY(m_set.own_packs[FRAME_BUFFER].second, new PK_FrameBuffer::Key())); }

ModuleType::~MD_ShowImage()
{
    for (auto &p_key: pk_keys) {
        delete p_key.second; }
}

void ModuleType::generate_packages(PACKAGES *packs) const noexcept
{
    (*packs)[*static_cast<PK_FrameBuffer::Key*>(pk_keys.at(m_set.own_packs[FRAME_BUFFER].second))]
            = PK_FrameBuffer(m_set.own_packs[FRAME_BUFFER].first,
                             m_set.own_packs[FRAME_BUFFER].second,
                             std::this_thread::get_id());
}

void ModuleType::generate_thread(THRAEDS *threads, INTERFACES *ifaces,
                                 PACKAGES *packs, MODULES *modules) const noexcept
{
    ThreadType thread(&(*ifaces)[if_key]);
    append_packs(std::move(thread), threads, packs, modules, th_key);
}

} // md_showimage

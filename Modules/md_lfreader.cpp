#include "md_lfreader.h"

namespace md_lfreader
{

ThreadType::TH_LFReader(Interface *iface)
    : Thread(iface)
{
    int i = 0;
    for (auto &item: m_settings.own_packs) {
        _add_pack_link_(i++, MODULE_NAME(item.first), PACKAGE_NAME(item.second)); }
    for (auto &item: m_settings.linked_packs) {
        _add_pack_link_(i++, MODULE_NAME(item.first), PACKAGE_NAME(item.second)); }
    cap.open("test_video.mp4", cv::CAP_FFMPEG);
//    cap.open(0);
    if (!cap.isOpened()) {
        throw std::runtime_error("error: VideoCapture wasn't open"); }
//    cap.set(cv::CAP_PROP_POS_FRAMES, 10000);
}

bool ThreadType::thread()
{
    frame_buffer->unique_lock();
    cap.read(frame_buffer->frame.get());
    if(frame_buffer->frame.get().empty())
    {
        frame_buffer->unlock();
        iface->send_command(CV_DISABLE);
        return true;
    }
    frame_buffer->unlock();
    return true;
}

void TH_LFReader::_init_pack_pointers_() noexcept
{
    output_queue = static_cast<md_iosystem::PK_OutputQueue*>(packs[OUTPUT_QUEUE]);
    frame_buffer = static_cast<md_showimage::PK_FrameBuffer*>(packs[FRAME_BUFFER]);
}

ModuleType::MD_LFReader()
    : Module(m_settings, &th_key) {}

ModuleType::~MD_LFReader()
{
    for (auto &p_key: pk_keys) {
        delete p_key.second; }
}

void ModuleType::generate_packages(PACKAGES *packs) const noexcept {}

void ModuleType::generate_thread(THRAEDS *threads, INTERFACES *ifaces,
                                 PACKAGES *packs, MODULES *modules) const noexcept
{
    ThreadType thread(&(*ifaces)[if_key]);
    append_packs(std::move(thread), threads, packs, modules, th_key);
}

} // md_lfreader

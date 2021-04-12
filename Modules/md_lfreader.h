#ifndef MD_LFREADER_H
#define MD_LFREADER_H

#include "std_afx.h"
#include "Core/module.h"
#include "Core/thread.h"
#include "Modules/pk_iosystem.h"
#include "Modules/pk_showimage.h"

namespace md_lfreader
{

using namespace Core;

const MODULE_NAME SELF_NAME = MN_LFReader;
const PACK_LINKS OWN_PACKS = {};
enum THIS_PACKS_NUMS {};
const PACK_LINKS LINKED_PACKS = { PACK_LINK(MN_IOSystem, PN_IOSystem_OutputQueue),
                                PACK_LINK(MN_ShowImage, PN_ShowImage_FrameBuffer) };
enum LINK_PACKS_NUMS { OUTPUT_QUEUE,
                       FRAME_BUFFER };
const IFACE_LINKS SUBORDINATE_IFACE_LINKS = { MN_ShowImage };
enum SUBORDINATE_IFACE_NUMS { SBTI_ShowImage }; // SBTI_
const IFACE_LINKS CONTROLLED_IFACE_LINKS = {};
enum CONTROLLED_IFACE_NUMS {}; // CTRI_
const IFACE_LINKS CONCURRENT_IFACE_LINKS = {};
enum CONCURRENT_IFACE_NUMS {}; // CCRI_
const ACCESS_SETTINGS AC_SETTINGS = {};

const ModuleSettings m_settings(SELF_NAME,
                                OWN_PACKS,
                                LINKED_PACKS,
                                SUBORDINATE_IFACE_LINKS,
                                CONTROLLED_IFACE_LINKS,
                                CONCURRENT_IFACE_LINKS,
                                AC_SETTINGS);

typedef class TH_LFReader : public Thread
{
    md_iosystem::PK_OutputQueue *output_queue = nullptr;
    md_showimage::PK_FrameBuffer *frame_buffer = nullptr;
private:
    cv::VideoCapture cap;
public:
    TH_LFReader(Interface *iface);
public:
    bool thread() override;
public:
    void _init_pack_pointers_() noexcept override;
public:
    class Key : public __BaseThreadKey__ {
    public: typedef std::thread type; };
} ThreadType;

typedef class MD_LFReader : public Module
{
    ThreadType::Key th_key;
public:
    MD_LFReader();
    ~MD_LFReader();
public:
    void generate_packages(PACKAGES *packs) const noexcept override;
    void generate_thread(THRAEDS *threads, INTERFACES *ifaces,
                         PACKAGES *packs, MODULES *modules) const noexcept override;
} ModuleType;

} // md_lfreader

#endif // MD_LFREADER_H

#ifndef MD_SHOWIMAGE_H
#define MD_SHOWIMAGE_H

#include "std_afx.h"
#include "Core/module.h"
#include "Core/thread.h"
#include "Modules/pk_iosystem.h"
//#include "Modules/pk_lfreader.h"
#include "Modules/pk_showimage.h"

namespace md_showimage
{

using namespace Core;

const MODULE_NAME SELF_NAME = MN_ShowImage;
const PACK_LINKS OWN_PACKS = { PACK_LINK(SELF_NAME, PN_ShowImage_FrameBuffer) };
enum THIS_PACKS_NUMS { FRAME_BUFFER };
const PACK_LINKS LINKED_PACKS = { PACK_LINK(MN_IOSystem, PN_IOSystem_OutputQueue) };
enum LINK_PACKS_NUMS { OUTPUT_QUEUE };
const IFACE_LINKS SUBORDINATE_IFACE_LINKS = {};
enum SUBORDINATE_IFACE_NUMS {}; // SBTI_
const IFACE_LINKS CONTROLLED_IFACE_LINKS = {};
enum CONTROLLED_IFACE_NUMS {}; // CTRI_
const IFACE_LINKS CONCURRENT_IFACE_LINKS = {};
enum CONCURRENT_IFACE_NUMS {}; // CCRI_
const ACCESS_SETTINGS AC_SETTINGS = {
    ACCESS_ITEM(PN_ShowImage_FrameBuffer,
        ACCESS_TO_PACK({ ACCESS_VALUE(AT_UNIQUE, SELF_NAME),
                         ACCESS_VALUE(AT_UNIQUE, MN_LFReader) }))
};

const ModuleSettings m_settings(SELF_NAME,
                                OWN_PACKS,
                                LINKED_PACKS,
                                SUBORDINATE_IFACE_LINKS,
                                CONTROLLED_IFACE_LINKS,
                                CONCURRENT_IFACE_LINKS,
                                AC_SETTINGS);

typedef class TH_ShowImage : public Thread
{
    PK_FrameBuffer *frame_buffer = nullptr;
    md_iosystem::PK_OutputQueue *output_queue = nullptr;
public:
    TH_ShowImage(Interface *iface);
public:
    bool thread() override;
public:
    void _init_pack_pointers_() noexcept override;
public:
    class Key : public __BaseThreadKey__ {
    public: typedef std::thread type; };
} ThreadType;

typedef class MD_ShowImage : public Module
{
    ThreadType::Key th_key;
public:
    MD_ShowImage();
    ~MD_ShowImage();
public:
    void generate_packages(PACKAGES *packs) const noexcept override;
    void generate_thread(THRAEDS *threads, INTERFACES *ifaces,
                         PACKAGES *packs, MODULES *modules) const noexcept override;
} ModuleType;

} // md_showimage

#endif // MD_SHOWIMAGE_H

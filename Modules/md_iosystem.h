#ifndef MD_IOSYSTEM_H
#define MD_IOSYSTEM_H

#include <iostream>
#include "std_afx.h"
#include "Core/module.h"
#include "Core/thread.h"
#include "Modules/pk_iosystem.h"

namespace md_iosystem
{

using namespace Core;

const MODULE_NAME SELF_NAME = MN_IOSystem;
const PACK_LINKS OWN_PACKS = { PACK_LINK(SELF_NAME, PN_IOSystem_OutputQueue) };
enum THIS_PACKS_NUMS { OUTPUT_QUEUE };
const PACK_LINKS LINKED_PACKS = {};
enum LINK_PACKS_NUMS {};
const IFACE_LINKS SUBORDINATE_IFACE_LINKS = {};
enum SUBORDINATE_IFACE_NUMS {}; // SBTI_
const IFACE_LINKS CONTROLLED_IFACE_LINKS = { MN_LFReader,
                                             MN_ShowImage };
enum CONTROLLED_IFACE_NUMS { CTRI_LFReader,
                             CTRI_ShowImage }; // CTRI_
const IFACE_LINKS CONCURRENT_IFACE_LINKS = {};
enum CONCURRENT_IFACE_NUMS {}; // CCRI_
const ACCESS_SETTINGS AC_SETTINGS = {
    ACCESS_ITEM(PN_IOSystem_OutputQueue,
        ACCESS_TO_PACK({ ACCESS_VALUE(AT_UNIQUE, SELF_NAME),
                         ACCESS_VALUE(AT_UNIQUE, MN_LFReader),
                         ACCESS_VALUE(AT_UNIQUE, MN_ShowImage) }))
};

const ModuleSettings m_settings(SELF_NAME,
                                OWN_PACKS,
                                LINKED_PACKS,
                                SUBORDINATE_IFACE_LINKS,
                                CONTROLLED_IFACE_LINKS,
                                CONCURRENT_IFACE_LINKS,
                                AC_SETTINGS);

typedef class TH_IOSystem : public Thread
{
    PK_OutputQueue *p_output_queue = nullptr;
private:
    std::string cmd;
public:
    TH_IOSystem(Interface *iface);
public:
    bool thread() override;
public:
    void _init_pack_pointers_() noexcept override;
public:
    class Key : public __BaseThreadKey__ {
    public: typedef std::thread type; };
} ThreadType;

typedef class MD_IOSystem : public Module
{
    ThreadType::Key th_key;
public:
    MD_IOSystem();
    ~MD_IOSystem();
public:
    void generate_packages(PACKAGES *packs) const noexcept override;
    void generate_thread(THRAEDS *threads, INTERFACES *ifaces,
                         PACKAGES *packs, MODULES *modules) const noexcept override;
} ModuleType;

} // md_iosystem

#endif // MD_IOSYSTEM_H

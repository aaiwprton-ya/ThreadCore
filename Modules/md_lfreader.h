#ifndef MD_LFREADER_H
#define MD_LFREADER_H

#include "std_afx.h"
#include "Core/module.h"
#include "Core/thread.h"
#include "Modules/pk_iosystem.h"
#include "Modules/pk_lfreader.h"

namespace md_lfreader
{

using namespace Core;

const std::string THIS_NAME = "LFReader";
const PACK_LINKS THIS_PACKS = { PACK_LINK(THIS_NAME, "FrameBuffer") };
enum THIS_PACKS_NUMS { FRAME_BUFFER };
const unsigned int COUNT_PACKS = 1;
const PACK_LINKS LINK_PACKS = { PACK_LINK("IOSystem", "OutputQueue") };
enum LINK_PACKS_NUMS { OUTPUT_QUEUE };
const IFACE_LINKS LINK_IFACES = {};

typedef class TH_LFReader : public Thread
{
    cv::VideoCapture cap;
public:
    TH_LFReader(Interface *iface);
public:
    bool thread() override;
public:
    class Key : public __BaseThreadKey__ {
    public: typedef std::thread type; };
} ThreadType;

typedef class MD_LFReader : public Module
{
    PK_KEYS pk_keys;
    BASE_IFKEY if_key;
    ThreadType::Key th_key;
public:
    MD_LFReader();
    ~MD_LFReader();
public:
    void generate_packages(Pool<Package> *packs) const noexcept override;
    void generate_interface(Pool<Interface> *ifaces) const noexcept override;
    void link_interfaces(Pool<Interface> *ifaces,
                         std::map<std::string,
                         Module*> *modules) const noexcept override;
    void generate_thread(Pool<std::thread> *threads,
                         Pool<Interface> *ifaces,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override;
    void link_threads(Pool<std::thread> *threads,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override;
    void disconnect(Pool<std::thread> *threads,
                         Pool<Package> *packs,
                         std::map<std::string, Module*> *modules) const noexcept override;
public:
    BASE_IFKEY get_if_key() const noexcept override;
    void get_pack_key(const BASE_PKEY *&p_key, std::string pack_name) const noexcept override;
    void get_thread_key(const BASE_THKEY *&t_key) const noexcept override;
} ModuleType;

} // md_lfreader

#endif // MD_LFREADER_H

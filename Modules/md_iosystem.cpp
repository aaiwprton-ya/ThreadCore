#include "md_iosystem.h"

namespace md_iosystem
{

MD_IOSystem::MD_IOSystem()
    : Module(M_NAME)
{
    pk_keys.insert(std::pair<std::string, Package::__BasePackKey__*>(P_NAME[0], new PK_OutputQueue::Key()));
    // links this iface to other ifaces
//    iface_lincks.push_back("module_name");
    // links this thread to any packages
    pack_links.insert(std::pair<std::string, std::string>(M_NAME, P_NAME[0]));
    // links these packages to any threads
    thread_links.insert(std::pair<std::string, std::vector<std::pair<bool, std::string>>>
                        (P_NAME[0], std::vector<std::pair<bool, std::string>>({std::pair<bool, std::string>(false, M_NAME)})));
}

MD_IOSystem::~MD_IOSystem()
{
    for (auto &p_key: pk_keys) {
        delete p_key.second; }
}

} // md_iosystem

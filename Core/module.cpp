#include "module.h"

namespace Core {

Module::Module(std::string name)
    : name(name) {}

Module::~Module() {}

BASE_PKEY *Module::_get_pack_key_(const std::string &pack_name,
                                  const PACK_LINKS &this_packs,
                                  const PACK_LINKS &link_packs,
                                  const PK_KEYS &pk_keys) const noexcept
{
    Package::__BasePackKey__ *p_key = nullptr;
    for (auto &_item: this_packs)
    {
        if (pack_name == _item.second) {
            p_key = pk_keys.at(_item.second); }
        if (p_key != nullptr) {
            break; }
    }
    for (auto &_item: link_packs)
    {
        if (pack_name == _item.second) {
            p_key = pk_keys.at(_item.second); }
        if (p_key != nullptr) {
            break; }
    }
    return p_key;
}

} // namespace Core

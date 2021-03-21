#include "thread.h"

namespace Core {

Utils::IDGen Core::Thread::__BaseThreadKey__::__id_gen__;

Thread::Thread(Interface *iface)
    : iface(iface),
      _temp_pack_struct(new std::list<__PackStructure__>()) {}

Thread::Thread(Thread &&thread)
{
    __first_append__ = thread.__first_append__;
    iface = thread.iface;
    thread.iface = nullptr;
    _temp_pack_struct = thread._temp_pack_struct;
    thread._temp_pack_struct = nullptr;
    packs = std::move(thread.packs);
}

Thread::~Thread()
{
    if (_temp_pack_struct != nullptr)
    {
        _temp_pack_struct->clear();
        delete _temp_pack_struct;
    }
}

void Thread::operator()()
{
    iface->post_init_stopped();
    bool __exit__ = false;
    while (thread() && !__exit__)
    {
        switch (iface->check_command()) {
        case CV_NOCMD:
            break;
        case CV_STOP:
            iface->stopped();
            break;
        case CV_TERMINATE:
            __exit__ = true;
            break;
        }
    }
}

void Thread::_add_pack_link_(id_t &&index, std::string &&module_name, std::string &&name)
{
    _temp_pack_struct->emplace_back(std::move(index), std::move(name), std::move(module_name));
    _temp_pack_struct->sort();
}

void Thread::append_pack(Package *pack)
{
    if (__first_append__)
    {
        packs.resize(_temp_pack_struct->size(), nullptr);
        __first_append__ = false;
    }
    size_t i = 0;
    for (auto _pack: *_temp_pack_struct)
    {
        if (pack->getName() == _pack.name && pack->getModuleName() == _pack.module_name)
        {
            _pack.pack = pack;
            packs[i] = _pack.pack;
            return;
        }
        ++i;
    }
    throw(std::runtime_error("error: invalid package name"));
}

} // namespace Core

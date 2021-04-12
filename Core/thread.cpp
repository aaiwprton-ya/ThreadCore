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
    bool __exit__ = false;
    iface->post_init_stopped();
    iface->send_command_all(CV_DISABLE);
    CTRL_VAR command = CV_NOCMD;
    while (true)
    {
        iface->set_state(TS_CHECKING);
        command = iface->check_command();
        do {
            switch (command) {
            case CV_NOCMD:
                iface->wait();
                break;
            case CV_STOP:
                iface->stop();
                break;
            case CV_DISABLE:
                iface->disable();
                break;
            case CV_TERMINATE:
                iface->terminate();
                __exit__ = true;
                break;
            default:
                break;
            }
            command = iface->check_command();
        } while (command != CV_NOCMD);
        if (__exit__) {
            break; }
        iface->set_state(TS_RUNNING);
        if (!thread()) {
            iface->send_command(CV_DISABLE); }
        iface->wakeup_all();
    }
}

void Thread::_add_pack_link_(id_t &&index, MODULE_NAME &&module_name, PACKAGE_NAME &&name)
{
    _temp_pack_struct->emplace_back(std::move(index), std::move(module_name), std::move(name));
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

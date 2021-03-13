#include "interface.h"

namespace Core {

Utils::IDGen Core::Interface::__BaseIfaceKey__::__id_gen__;

Interface::Interface()
    : mtx(new std::mutex()),
      con_var(new std::condition_variable()) {}

Interface::~Interface()
{
    if (mtx != nullptr) {
        delete mtx; }
    if (con_var != nullptr) {
        delete con_var; }
}

Interface &Interface::operator=(Interface &&iface)
{
    if (this != &iface)
    {
        __ignore__ = iface.__ignore__;
        if (mtx != nullptr) {
            delete mtx; }
        if (con_var != nullptr) {
            delete con_var; }
        mtx = iface.mtx;
        iface.mtx = nullptr;
        con_var = iface.con_var;
        iface.con_var = nullptr;
        state = iface.state;
        ifaces = std::move(iface.ifaces);
    }
    return *this;
}

void Interface::post_init_stopped() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    state = POST_INIT;
    con_var->wait(un_lock);
    state = RUN;
}

void Interface::stopped() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    state = STOP;
    con_var->wait(un_lock);
    state = RUN;
}

void Interface::start() const noexcept
{
    while (true)
    {
        if (this->get_state() == POST_INIT)
        {
            con_var->notify_all();
            break;
        }
    }
}

void Interface::activate() noexcept
{
    while (!__ignore__)
    {
        if (this->get_state() == STOP)
        {
            con_var->notify_all();
            break;
        }
    }
    __ignore__ = false;
}

void Interface::try_activate() noexcept
{
    if (this->get_state() == STOP)
    {
        con_var->notify_all();
        __ignore__ = true;
    }
}

void Interface::activate_all() const noexcept
{
    for (auto iface: ifaces) {
        iface->activate(); }
}

void Interface::try_activate_all() const noexcept
{
    for (auto iface: ifaces) {
        iface->try_activate(); }
}

void Interface::append_iface(Interface *iface) noexcept {
    ifaces.push_back(iface); }

std::mutex *Interface::get_mtx() const noexcept {
    return mtx; }

std::condition_variable *Interface::get_con_var() const noexcept {
    return con_var; }

th_state Interface::get_state() const noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    return state;
}

Interface *Interface::get_iface(int index) const noexcept {
    return ifaces[index]; }

} // namespace Core

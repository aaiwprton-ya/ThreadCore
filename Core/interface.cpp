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
    if (this == &iface) {
        return *this; }
    if (mtx != nullptr) {
        delete mtx; }
    if (con_var != nullptr) {
        delete con_var; }
    ignore = iface.ignore;
    mtx = iface.mtx;
    iface.mtx = nullptr;
    con_var = iface.con_var;
    iface.con_var = nullptr;
    ctrl_var = iface.ctrl_var;
    state = iface.state;
    ifaces = std::move(iface.ifaces);
    return *this;
}

void Interface::post_init_stopped() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    state = TS_POST_INIT;
    con_var->wait(un_lock);
    state = TS_RUN;
}

void Interface::stopped() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    state = TS_STOP;
    con_var->wait(un_lock);
    state = TS_RUN;
}

void Interface::start() const noexcept
{
    while (true)
    {
        if (this->get_state() == TS_POST_INIT)
        {
            con_var->notify_all();
            break;
        }
    }
}

void Interface::activate() noexcept
{
    while (!ignore)
    {
        if (this->get_state() == TS_STOP)
        {
            con_var->notify_all();
            break;
        }
    }
    ignore = false;
}

void Interface::try_activate() noexcept
{
    if (this->get_state() == TS_STOP)
    {
        con_var->notify_all();
        ignore = true;
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

void Interface::send_command(CTRL_VAR value) noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    ctrl_var = value;
    ctrl_flag = true;
}

void Interface::send_command_all(CTRL_VAR value) noexcept
{
    for (auto iface: ifaces) {
        iface->send_command(value); }
}

CTRL_VAR Interface::check_command() noexcept
{
    if (ctrl_flag)
    {
        ctrl_flag = false;
        std::unique_lock<std::mutex> un_lock(*mtx);
        return ctrl_var;
    }
    else {
        return CV_NOCMD; }
    return CV_NOCMD;
}

TH_STATE Interface::get_state() const noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    return state;
}

Interface *Interface::get_iface(int index) const noexcept {
    return ifaces[index]; }

} // namespace Core

#include "interface.h"

namespace Core {

Utils::IDGen Core::Interface::__BaseIfaceKey__::__id_gen__;

Interface::Interface()
    : mtx(new std::mutex()),
      con_var(new std::condition_variable()) {}

Interface::Interface(std::string self_name)
    : self_name(self_name),
      mtx(new std::mutex()),
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
    self_name = iface.self_name;
    if (this == &iface) {
        return *this; }
    if (mtx != nullptr) {
        delete mtx; }
    if (con_var != nullptr) {
        delete con_var; }
    mtx = iface.mtx;
    iface.mtx = nullptr;
    con_var = iface.con_var;
    iface.con_var = nullptr;
    ctrl_flag = static_cast<bool>(iface.ctrl_flag);
    ctrl_var = static_cast<CTRL_VAR>(iface.ctrl_var);
    state = static_cast<THREAD_STATE>(iface.state);
    subordinate_ifaces = std::move(iface.subordinate_ifaces);
    controlled_ifaces = std::move(iface.controlled_ifaces);
    concurrent_ifaces = std::move(iface.concurrent_ifaces);
    return *this;
}

void Interface::post_init_stopped() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    state = TS_READY;
    con_var->wait(un_lock);
}

void Interface::start() const noexcept
{
    THREAD_STATE __state__ = TS_INITIALIZING;
    while (true)
    {
        __state__ = get_state();
        if (__state__ == TS_READY)
        {
            con_var->notify_all();
            break;
        }
        else if (__state__ != TS_INITIALIZING && __state__ != TS_READY) {
            return; }
    }
}

void Interface::stop() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    send_command_all(CV_STOP);
    state = TS_STOPPED;
    con_var->wait(un_lock);
    state = TS_CHECKING;
}

void Interface::wait() noexcept
{
    if (!get_waitebl()) {
        return; }
    std::unique_lock<std::mutex> un_lock(*mtx);
    state = TS_WAITING;
    con_var->wait(un_lock);
    state = TS_CHECKING;
}

void Interface::activate() noexcept
{
    THREAD_STATE __state__;
    while (true)
    {
        __state__ = get_state();
        if (__state__ == TS_STOPPED)
        {
            con_var->notify_all();
            break;
        }
        else if (__state__ == TS_TERMINATED) {
            return; }
    }
}

void Interface::activate_all() const noexcept
{
    for (auto iface: subordinate_ifaces) {
        iface->activate(); }
}

bool Interface::wakeup() noexcept
{
    THREAD_STATE __state__ = TS_RUNNING;
    while (true)
    {
        if (!get_waitebl()) {
            return false; }
        __state__ = get_state();
        if (__state__ == TS_WAITING)
        {
            con_var->notify_all();
            return true;
        }
        else if (__state__ == TS_STOPPED) {
            this->send_command(CV_START); }
        else if (__state__ == TS_TERMINATED) {
            return false; }
    }
}

void Interface::wakeup_all() noexcept
{
    size_t __size__ = subordinate_ifaces.size();
    if (__size__ == 0) {
        return; }
    bool disabling = true;
    std::vector<bool> temp_last_active;
    temp_last_active.resize(__size__);
    for (size_t i = 0; i < __size__; ++ i)
    {
        if (subordinate_ifaces[i]->wakeup())
        {
            temp_last_active[i] = true;
            disabling = false;
        }
        else {
            temp_last_active[i] = false; }
    }
    if (disabling) {
        send_command(CV_DISABLE); }
    else {
        last_active_ifaces = temp_last_active; }
}

void Interface::append_subordinate_iface(Interface *iface) noexcept
{
    subordinate_ifaces.push_back(iface);
    last_active_ifaces.push_back(true);
}

void Interface::append_controlled_iface(Interface *iface) noexcept {
    controlled_ifaces.push_back(iface); }

void Interface::append_concurrent_iface(Interface *iface) noexcept {
    concurrent_ifaces.push_back(iface); }

void Interface::send_command(CTRL_VAR value) noexcept
{
    while (get_state() == TS_CHECKING);
    std::unique_lock<std::mutex> un_lock(*mtx);
    CTRL_VAR var_now;
    bool flag_now;
    var_now = ctrl_var;
    flag_now = ctrl_flag;
    bool cancel = false;
    bool activate = false;
    bool enable_right = false;
    if (flag_now)
    {
        switch (var_now) {
        case CV_NOCMD:
            cancel = true;
            break;
        case CV_START:
            if (value == CV_STOP) {
                cancel = true; }
            break;
        case CV_STOP:
            if (value == CV_START) {
                cancel = true; }
            break;
        case CV_DISABLE:
            if (value == CV_ENABLE) {
                cancel = true; }
            break;
        case CV_ENABLE:
            if (value == CV_DISABLE) {
                cancel = true; }
            break;
        case CV_TERMINATE:
            return;
        }
    }
    if (cancel)
    {
        ctrl_var = CV_NOCMD;
        ctrl_flag = false;
        return;
    }
    switch (state) {
    case TS_INITIALIZING:
        switch (value) {
        case CV_NOCMD:
            cancel = true;
            break;
        case CV_START:
            cancel = true;
            break;
        case CV_STOP:
            break;
        case CV_DISABLE:
            break;
        case CV_ENABLE:
            cancel = true;
            break;
        case CV_TERMINATE:
            break;
        }
        break;
    case TS_READY:
        switch (value) {
        case CV_NOCMD:
            cancel = true;
            break;
        case CV_START:
            cancel = true;
            break;
        case CV_STOP:
            break;
        case CV_DISABLE:
            break;
        case CV_ENABLE:
            cancel = true;
            break;
        case CV_TERMINATE:
            break;
        }
        break;
    case TS_CHECKING:
        break;
    case TS_RUNNING:
        switch (value) {
        case CV_NOCMD:
            cancel = true;
            break;
        case CV_START:
            cancel = true;
            break;
        case CV_STOP:
            break;
        case CV_DISABLE:
            break;
        case CV_ENABLE:
            cancel = true;
            break;
        case CV_TERMINATE:
            break;
        }
        break;
    case TS_WAITING:
        switch (value) {
        case CV_NOCMD:
            cancel = true;
            break;
        case CV_START:
            cancel = true;
            break;
        case CV_STOP:
            activate = true;
            break;
        case CV_DISABLE:
            activate = true;
            break;
        case CV_ENABLE:
            cancel = true;
            break;
        case CV_TERMINATE:
            activate = true;
            break;
        }
        break;
    case TS_STOPPED:
        switch (value) {
        case CV_NOCMD:
            cancel = true;
            break;
        case CV_START:
            cancel = true;
            activate = true;
            break;
        case CV_STOP:
            cancel = true;
            break;
        case CV_DISABLE:
            activate = true;
            break;
        case CV_ENABLE:
            cancel = true;
            break;
        case CV_TERMINATE:
            activate = true;
            break;
        }
        break;
    case TS_DISABLED:
        switch (value) {
        case CV_NOCMD:
            cancel = true;
            break;
        case CV_START:
            cancel = true;
            break;
        case CV_STOP:
            cancel = true;
            break;
        case CV_DISABLE:
            cancel = true;
            break;
        case CV_ENABLE:
            cancel = true;
            activate = true;
            enable_right = true;
            break;
        case CV_TERMINATE:
            activate = true;
            break;
        }
        break;
    case TS_TERMINATED:
        cancel = true;
        break;
    }
    if (cancel)
    {
        ctrl_var = CV_NOCMD;
        ctrl_flag = false;
    }
    else
    {
        ctrl_var = value;
        ctrl_flag = true;
    }
    if (enable_right)
    {
        for (size_t i = 0; i < subordinate_ifaces.size(); ++i)
        {
            if (last_active_ifaces[i]) {
                subordinate_ifaces[i]->set_waitebl(true);
                subordinate_ifaces[i]->send_command(CV_ENABLE); }
        }
    }
    if (activate) {
        con_var->notify_all(); }
    return;
}

void Interface::send_command_all(CTRL_VAR value) const noexcept
{
    for (auto iface: subordinate_ifaces) {
        iface->send_command(value); }
    for (auto iface: controlled_ifaces) {
        iface->send_command(value); }
}

CTRL_VAR Interface::check_command() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    if (ctrl_flag)
    {
        ctrl_flag = false;
        return static_cast<CTRL_VAR>(ctrl_var);
    }
    else {
        return CV_NOCMD; }
}

void Interface::terminate() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    state = TS_TERMINATED;
    waitebl = false;
}

void Interface::disable() noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    send_command_all(CV_STOP);
    waitebl = false;
    state = TS_DISABLED;
    con_var->wait(un_lock);
    state = TS_CHECKING;
}

void Interface::disable_subordinate(size_t index) const noexcept {
    subordinate_ifaces[index]->send_command(CV_DISABLE); }

void Interface::disable_controlled(size_t index) const noexcept {
    controlled_ifaces[index]->send_command(CV_DISABLE); }

void Interface::disable_concurrents() const noexcept
{
    for (auto iface: concurrent_ifaces) {
        iface->disable(); }
}

void Interface::disable_all() const noexcept {
    send_command_all(CV_DISABLE); }

void Interface::enable() noexcept
{
    THREAD_STATE state_now = get_state();
    if (state_now == TS_DISABLED)
    {
        disable_concurrents();
        con_var->notify_all();
    }
}

void Interface::enable_subordinate(size_t index) const noexcept
{
    subordinate_ifaces[index]->set_waitebl(true);
    subordinate_ifaces[index]->send_command(CV_ENABLE);
}

void Interface::enable_controlled(size_t index) const noexcept {
    controlled_ifaces[index]->send_command(CV_ENABLE); }

void Interface::enable_all() const noexcept
{
    for (auto iface: subordinate_ifaces)
    {
        iface->set_waitebl(true);
        iface->send_command(CV_ENABLE);
    }
    for (auto iface: controlled_ifaces) {
        iface->send_command(CV_ENABLE); }
}

void Interface::set_state(THREAD_STATE state) noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    this->state = state;
}

std::string Interface::get_name() const noexcept {
    return self_name; }

THREAD_STATE Interface::get_state() const noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    return static_cast<THREAD_STATE>(state);
}

Interface *Interface::get_subordinate_iface(int index) const noexcept {
    return subordinate_ifaces[index]; }

Interface *Interface::get_controlled_iface(int index) const noexcept {
    return controlled_ifaces[index]; }

std::vector<Interface *> Interface::get_subordinate_ifaces() const noexcept {
    return subordinate_ifaces; }

std::vector<Interface *> Interface::get_controlled_ifaces() const noexcept {
    return controlled_ifaces; }

size_t Interface::get_subordinate_iface_nums() {
    return subordinate_ifaces.size(); }

size_t Interface::get_controlled_iface_nums() {
    return controlled_ifaces.size(); }

bool Interface::get_waitebl() const noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    return waitebl;
}

void Interface::set_waitebl(bool value) noexcept
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    waitebl = value;
}

std::string ts_to_str(THREAD_STATE th_state)
{
    std::string state_name;
    switch (th_state) {
    case TS_INITIALIZING:
        state_name = "INITIALIZING";
        break;
    case TS_READY:
        state_name = "READY";
        break;
    case TS_CHECKING:
        state_name = "CHECKING";
    case TS_RUNNING:
        state_name = "RUNNING";
        break;
    case TS_STOPPED:
        state_name = "STOPPED";
        break;
    case TS_WAITING:
        state_name = "WAITING";
        break;
    case TS_DISABLED:
        state_name = "DISABLED";
        break;
    case TS_TERMINATED:
        state_name = "TERMINATED";
        break;
    }
    return state_name;
}

} // namespace Core

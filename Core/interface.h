#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H

#include "std_afx.h"
//#include <string>
//#include <vector>
//#include <mutex>
//#include <condition_variable>
#include "Utils/idgen.h"

namespace Core {

using namespace Utils;

enum TH_STATE {TS_INIT, TS_POST_INIT, TS_RUN, TS_STOP};     // MOVE TO THE ENUMS FILE LATER
enum CTRL_VAR {CV_NOCMD, CV_STOP, CV_TERMINATE};

class Interface
{
    bool ignore = false;
    std::mutex *mtx = nullptr;
    std::condition_variable *con_var = nullptr;
    std::atomic<bool> ctrl_flag = false;
    CTRL_VAR ctrl_var = CV_STOP;
private:
    TH_STATE state = TS_INIT;
    std::vector<Interface*> ifaces;
public:
    Interface();
    ~Interface();
public:
    Interface &operator=(Interface &&iface);
public:
    void post_init_stopped() noexcept;
    void stopped() noexcept;
    void start() const noexcept;
    void activate() noexcept;
    void try_activate() noexcept;
    void activate_all() const noexcept;
    void try_activate_all() const noexcept;
    void append_iface(Interface *iface) noexcept;
    void send_command(CTRL_VAR value) noexcept;
    void send_command_all(CTRL_VAR value) noexcept;
    CTRL_VAR check_command() noexcept;
public:
    TH_STATE get_state() const noexcept;
    Interface *get_iface(int index) const noexcept;
public:
    class __BaseIfaceKey__
    {
        static IDGen __id_gen__;
        t_id _id;
    public:
        __BaseIfaceKey__() : _id(Interface::__BaseIfaceKey__::__id_gen__.next) {
            Interface::__BaseIfaceKey__::__id_gen__.flush(); }
        ~__BaseIfaceKey__() {
            Interface::__BaseIfaceKey__::__id_gen__.release(_id); }
        bool operator==(const __BaseIfaceKey__ &value) const noexcept
        {
            if (_id == value._id) {
                return true; }
            return false;
        }
    public:
        typedef Core::Interface type;
    };
};

} // namespace Core

#endif // CORE_INTERFACE_H

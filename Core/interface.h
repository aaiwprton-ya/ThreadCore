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

enum th_state {INIT, POST_INIT, RUN, STOP};     // MOVE TO THE ENUMS FILE LATER

class Interface
{
    bool __ignore__ = false;
private:
    std::mutex *mtx = nullptr;
    std::condition_variable *con_var = nullptr;
    th_state state = INIT;
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
public:
    std::mutex *get_mtx() const noexcept;
    std::condition_variable *get_con_var() const noexcept;
    th_state get_state() const noexcept;
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

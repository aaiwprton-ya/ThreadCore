#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H

#include "std_afx.h"
#include "Utils/idgen.h"

namespace Core {

using namespace Utils;

enum THREAD_STATE { TS_INITIALIZING,
                    TS_READY,
                    TS_CHECKING,
                    TS_RUNNING,
                    TS_WAITING,
                    TS_STOPPED,
                    TS_DISABLED,
                    TS_TERMINATED
                  };
enum CTRL_VAR { CV_NOCMD, CV_START, CV_STOP, CV_DISABLE, CV_ENABLE, CV_TERMINATE };

std::string ts_to_str(THREAD_STATE th_state);

class Thread;

class Interface
{
    std::string self_name;
    std::mutex *mtx = nullptr;
    std::condition_variable *con_var = nullptr;
    std::atomic<bool> ctrl_flag = false;
    std::atomic<CTRL_VAR> ctrl_var = CV_NOCMD;
    std::atomic<THREAD_STATE> state = TS_INITIALIZING;
    std::vector<Interface*> subordinate_ifaces;
    std::vector<Interface*> controlled_ifaces;
    std::vector<Interface*> concurrent_ifaces;
    std::vector<bool> last_active_ifaces;
    bool waitebl = false;
public:
    Interface();
    Interface(std::string self_name);
    ~Interface();
    friend class Thread;
public:
    Interface &operator=(Interface &&iface);
public:
    void post_init_stopped() noexcept;
    void start() const noexcept;
    void stop() noexcept;
    void wait() noexcept;
    void activate() noexcept;
    void activate_all() const noexcept;
    bool wakeup() noexcept;
    void wakeup_all() noexcept;
    void append_subordinate_iface(Interface *iface) noexcept;
    void append_controlled_iface(Interface *iface) noexcept;
    void append_concurrent_iface(Interface *iface) noexcept;
public:
    void send_command(CTRL_VAR value) noexcept;
    void send_command_all(CTRL_VAR value) const noexcept;
    CTRL_VAR check_command() noexcept;
public:
    void terminate() noexcept;
    void disable() noexcept;
    void disable_subordinate(size_t index) const noexcept;
    void disable_controlled(size_t index) const noexcept;
    void disable_concurrents() const noexcept;
    void disable_all() const noexcept;
    void enable() noexcept;
    void enable_subordinate(size_t index) const noexcept;
    void enable_controlled(size_t index) const noexcept;
    void enable_all() const noexcept;
private:
    void set_state(THREAD_STATE state) noexcept;
public:
    std::string get_name() const noexcept;
    THREAD_STATE get_state() const noexcept;
    Interface *get_subordinate_iface(int index) const noexcept;
    Interface *get_controlled_iface(int index) const noexcept;
    std::vector<Interface*> get_subordinate_ifaces() const noexcept;
    std::vector<Interface*> get_controlled_ifaces() const noexcept;
    size_t get_subordinate_iface_nums();
    size_t get_controlled_iface_nums();
    bool get_waitebl() const noexcept;
    void set_waitebl(bool value) noexcept;
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

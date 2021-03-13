#ifndef THREAD_H
#define THREAD_H

//#include "std_afx.h"
//#include <thread>
//#include <vector>
//#include <list>
#include "Core/package.h"
#include "Core/interface.h"

namespace Core {

class __PackStructure__
{
    id_t index = -1;
    std::string name = "";
    std::string module_name = "";
    Package *pack = nullptr;
public:
    __PackStructure__(id_t &&index, std::string &&name, std::string &&module_name)
        : index(std::move(index)),
          name(std::move(name)),
          module_name(std::move(module_name)) {}
    friend class Thread;
public:
    bool operator==(const __PackStructure__ &value) {
        return (this->index == value.index); }
    bool operator<(const __PackStructure__ &value) {
        return (this->index < value.index); }
};

class Thread
{
    bool __first_append__ = true;
protected:
    Interface *iface = nullptr;
    std::list<__PackStructure__> *_temp_pack_struct = nullptr;
    std::vector<Package*> packs;
protected:
    Thread(Interface *iface);
    Thread(Thread &&thread);
    virtual ~Thread();
public:
    void operator()();
protected:
    virtual bool thread() = 0;
    void _add_pack_link_(id_t &&index, std::string &&name, std::string &&module_name);
public:
    void append_pack(Package *pack);
public:
    class __BaseThreadKey__
    {
        static IDGen __id_gen__;
        t_id _id;
    public:
        __BaseThreadKey__() : _id(Thread::__BaseThreadKey__::__id_gen__.next) {
            Thread::__BaseThreadKey__::__id_gen__.flush(); }
        ~__BaseThreadKey__() {
            Thread::__BaseThreadKey__::__id_gen__.release(_id); }
        bool operator==(const __BaseThreadKey__ &value) const noexcept
        {
            if (_id == value._id) {
                return true; }
            return false;
        }
    public:
        typedef std::thread type;
    };
};

} // namespace Core

#endif // THREAD_H

#ifndef CORE_PACKAGE_H
#define CORE_PACKAGE_H

#include "std_afx.h"
#include "Utils/sortedlist.h"
#include "Utils/idgen.h"

namespace Core {

using namespace Utils;

typedef std::string MODULE_NAME;
typedef std::string PACKAGE_NAME;

class __Access__
{
    void *pval = nullptr;
    size_t size = 0;
protected:
    __Access__(void* pval, size_t size) : pval(pval), size(size) {}
    void *__get__() noexcept {
        return pval; }
    const void *__get__() const noexcept {
        return pval; }
    void __set__(void *buff, size_t size, size_t offset = 0)
    {
        if (((size + offset)) > this->size) {
            throw std::runtime_error("sigmentation fault: out of range in the Access class\n"); }   // TRANSLATE LATER
        memcpy((char*)pval + offset, buff, size);
    }
};

class __PackageBase__
{
    Utils::SortedList<std::vector, std::thread::id> un_lock_threads;    // how can append / remove elements?
    Utils::SortedList<std::vector, std::thread::id> sh_lock_threads;
    bool shared = false;                                                // un_lock/sh_lock trigger
    std::thread::id current_lock;                                       // current locking thread
private:
    __PackageBase__() {}
    friend class Package;
    friend class Access;
};

class Package
{
    __PackageBase__ __p_base__;
    std::mutex *__mtx__ = nullptr;                  // for inside use only
    MODULE_NAME module_name = "";
    PACKAGE_NAME name = "";                         // only for system
    std::mutex *mtx = nullptr;                      // for shared use
    std::thread::id system;                         // system thread
    u_int joined = 0;                               // how many threads is shared joined now
    u_int disconnected = 0;                         // how many threads is disconnected after shered_lock
public:
    Package();
    Package(MODULE_NAME module_name, PACKAGE_NAME name, std::thread::id system);
    virtual ~Package();
    friend class System;
public:
    Package &operator=(Package &&package);
public:
    void unique_lock();
    void shared_lock();
    bool unlock() noexcept;
public:                                             // only for system
    std::string getName() const;
    std::string getModuleName() const;
    void connect_un_lock(std::thread::id value);
    void connect_sh_lock(std::thread::id value);
    void disconnect_un_lock(std::thread::id value);
    void disconnect_sh_lock(std::thread::id value);
    const __PackageBase__ &getP_base() const;

private:
    void __check_un_lock__(std::thread::id value);
    void __check_sh_lock__(std::thread::id value);
    void __check_system_id__() const;
public:
    class __BasePackKey__
    {
        static IDGen __id_gen__;
        t_id _id;
    public:
        __BasePackKey__() : _id(Package::__BasePackKey__::__id_gen__.next) {
            Package::__BasePackKey__::__id_gen__.flush(); }
        ~__BasePackKey__() {
            Package::__BasePackKey__::__id_gen__.release(_id); }
        bool operator==(const __BasePackKey__ &value) const noexcept
        {
            if (_id == value._id) {
                return true; }
            return false;
        }
    public:
        typedef Core::Package type;
    };
};

class Access
{
public:
    template<class T>
    class To : public __Access__
    {
        const __PackageBase__ &p_base;
    private:
        void __check_access_un_lock__() const
        {
            if (p_base.current_lock != std::this_thread::get_id()) {
                throw std::runtime_error("error: non-lock thread is not able to access\n"); } // TRANSLATE LATER
        }
        void __check_access_sh_lock__() const
        {
            if (!(p_base.shared && p_base.sh_lock_threads.exist(std::this_thread::get_id()))) {
                throw std::runtime_error("error: non-shared_lock thread can't get available\n"); }  // TRANSLATE LATER
        }
    public:
        To(void *pval, size_t size, const __PackageBase__ &p_base)
            : __Access__(pval, size),
              p_base(p_base) {}
        const T *operator->()
        {
            __check_access_sh_lock__();
            return static_cast<T*>(__Access__::__get__());
        }
        T &get()
        {
            __check_access_un_lock__();
            return *(static_cast<T*>(__Access__::__get__()));
        }
        void get(T &var) const
        {
            __check_access_sh_lock__();
            var = *(static_cast<const T*>(__Access__::__get__()));
        }
        void set(T &value)
        {
            __check_access_un_lock__();
            __Access__::__set__(&value, sizeof(value));
        }
        void set(void *buff, size_t size, size_t offset = 0)
        {
            __check_access_un_lock__();
            __Access__::__set__(buff, size, offset);
        }
    };
};

} // namespace Core

#endif // CORE_PACKAGE_H

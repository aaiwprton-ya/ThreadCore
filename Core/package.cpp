#include "package.h"
#include <thread>

namespace Core {

Utils::IDGen Core::Package::__BasePackKey__::__id_gen__;

const __PackageBase__ &Package::getP_base() const
{
    return __p_base__;
}

Package::Package()
    : __mtx__(new std::mutex()),
      mtx(new std::mutex()),
      system(-1) {}

Package::Package(std::string module_name, std::string name, std::thread::id system)
    : __mtx__(new std::mutex()),
      module_name(module_name),
      name(name),
      mtx(new std::mutex()),
      system(system) {}

Package &Package::operator=(Package &&package)
{
    if (this != &package)
    {
        if (__mtx__ != nullptr) {
            delete __mtx__; }
        if (mtx != nullptr) {
            delete mtx; }
        __mtx__ = package.__mtx__;
        package.__mtx__ = nullptr;
        module_name = package.module_name;
        name = package.name;
        mtx = package.mtx;
        package.mtx = nullptr;
        system = package.system;
    }
    return *this;
}

void Package::unique_lock()
{
    {
        std::unique_lock<std::mutex> un_lock(*__mtx__);
        if (!__p_base__.un_lock_threads.exist(std::this_thread::get_id())) {
            throw std::runtime_error("error: the not unique connected thread is not able to unique_lock\n"); }      // TRANSLATE LATER
    }
    mtx->lock();
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    __p_base__.shared = false;
    __p_base__.current_lock = std::this_thread::get_id();
}

Package::~Package()
{
    if (__mtx__ != nullptr) {
        delete __mtx__; }
    if (mtx != nullptr) {
        delete mtx; }
}

void Package::shared_lock()
{
    {
        std::unique_lock<std::mutex> un_lock(*__mtx__);
        if (!__p_base__.sh_lock_threads.exist(std::this_thread::get_id())) {
            throw std::runtime_error("error: the not shared connected thread is not able to shared_lock\n"); }      // TRANSLATE LATER
        if (mtx->try_lock())
        {
            __p_base__.shared = true;
            ++joined;
            return;
        }
        else if (__p_base__.shared)
        {
            ++joined;
            return;
        }
    }
    mtx->lock();
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    __p_base__.shared = true;
    ++joined;
}

bool Package::unlock() noexcept
{
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    if (__p_base__.shared)
    {
        if ((__p_base__.sh_lock_threads.size() - 1) == disconnected)
        {
            mtx->unlock();
            disconnected = 0;
            joined = 0;
            return true;
        }
        else
        {
            ++disconnected;
            return false;
        }
    }
    else
    {
        mtx->unlock();
        return true;
    }
}

std::string Package::getName() const
{
    __check_system_id__();
    return name;
}

std::string Package::getModuleName() const
{
    __check_system_id__();
    return module_name;
}

// connection by the system the threads to this package
void Package::connect_un_lock(std::thread::id value)
{
    __check_system_id__();
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    __p_base__.un_lock_threads.insert(value);
}

void Package::connect_sh_lock(std::thread::id value)
{
    __check_system_id__();
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    __p_base__.sh_lock_threads.insert(value);
}

void Package::disconnect_un_lock(std::thread::id value)
{
    __check_system_id__();
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    __p_base__.un_lock_threads.release(value);
}

void Package::disconnect_sh_lock(std::thread::id value)
{
    __check_system_id__();
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    __p_base__.sh_lock_threads.release(value);
}

void Package::__check_un_lock__(std::thread::id value)
{
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    if (__p_base__.un_lock_threads.exist(value)) {
        throw std::runtime_error("error: the thread doesn't have unique access to this package\n"); }   // TRANSLATE LATER
}

void Package::__check_sh_lock__(std::thread::id value)
{
    std::unique_lock<std::mutex> un_lock(*__mtx__);
    if (__p_base__.sh_lock_threads.exist(value)) {
        throw std::runtime_error("error: the thread doesn't have shared access to this package\n"); }   // TRANSLATE LATER
}

void Package::__check_system_id__() const
{
    if (std::this_thread::get_id() != system) {
        throw std::runtime_error("error: non-system thread can't access\n"); }   // TRANSLATE LATER
}

} // namespace Core

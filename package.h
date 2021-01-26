#ifndef CORE_PACKAGE_H
#define CORE_PACKAGE_H

#include <mutex>

namespace Core {

class Package
{
public:
    const std::string name = "unnamed";
    std::mutex mtx;
public:
    Package(std::string name);
    virtual ~Package();
public:
    virtual bool execute(std::string cmd) = 0;
};

} // namespace Core

#endif // CORE_PACKAGE_H

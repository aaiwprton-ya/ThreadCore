#include "pk_iosystem.h"

namespace md_iosystem
{

PK_OutputQueue::PK_OutputQueue() : queue(&_queue, sizeof(_queue), getP_base()) {}

PK_OutputQueue::PK_OutputQueue(MODULE_NAME module_name, PACKAGE_NAME name, std::thread::id system)
    : Package(module_name, name, system),
      queue(&_queue, sizeof(_queue), getP_base()) {}

PK_OutputQueue &PK_OutputQueue::operator=(PK_OutputQueue &&pack)
{
    static_cast<Package&>(*this) = std::move(pack);
    return *this;
}

} // md_iosystem

#include "pk_iosystem.h"

namespace md_iosystem
{

PK_OutputQueue::PK_OutputQueue() : queue(&_queue, sizeof(_queue), getP_base()) {}

PK_OutputQueue::PK_OutputQueue(std::string name, std::string modul_name, std::thread::id system)
    : Package(name, modul_name, system),
      queue(&_queue, sizeof(_queue), getP_base()) {}

PK_OutputQueue &PK_OutputQueue::operator=(PK_OutputQueue &&pack)
{
    static_cast<Package&>(*this) = std::move(pack);
    return *this;
}

} // md_iosystem

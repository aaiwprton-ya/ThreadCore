#include "thread.h"

namespace Core {

Thread::Thread(Interface *iface)
    :
      iface(iface)
{}

void Thread::operator()()
{
    while (true)
    {
        thread();
    }
}

} // namespace Core

#ifndef THREAD_H
#define THREAD_H

namespace Core {

class Interface;

class Thread
{
protected:
    Interface *iface = nullptr;
public:
    Thread(Interface *iface);
    void operator()();
protected:
    virtual void thread() = 0;
};

} // namespace Core

#endif // THREAD_H

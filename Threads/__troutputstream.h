#ifndef CORE_TROUTPUTSTREAM_H
#define CORE_TROUTPUTSTREAM_H

#include <string>
#include <queue>
#include "thread.h"
#include "ifoutputstream.h"

namespace Core {

class TrOutputStream : public Thread
{
public:
    TrOutputStream(IFOutputStream *iface);
private:
    void thread() override;
};

} // namespace Core

#endif // CORE_TROUTPUTSTREAM_H

#ifndef CORE_TRINPUTSTREAM_H
#define CORE_TRINPUTSTREAM_H

#include <string>
#include <queue>
#include "thread.h"
#include "ifinputstream.h"

namespace Core {

class TrInputStream : public Thread
{
public:
    TrInputStream(IFInputStream *iface);
private:
    void thread() override;
};

} // namespace Core

#endif // CORE_TRINPUTSTREAM_H

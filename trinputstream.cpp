#include "trinputstream.h"

namespace Core {

TrInputStream::TrInputStream(IFInputStream *iface)
    :
      Thread(iface)
{}

void TrInputStream::thread()
{
    while (true)
    {
        if (iface->state == STOP)
        {
            // TODO: sleep
        }
        else
        {

        }
    }
}

} // namespace Core

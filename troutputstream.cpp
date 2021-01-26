#include "troutputstream.h"

namespace Core {

TrOutputStream::TrOutputStream(IFOutputStream *iface)
    :
      Thread(iface)
{}

void TrOutputStream::thread()
{

}

} // namespace Core

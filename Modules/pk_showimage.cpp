#include "pk_showimage.h"

namespace md_showimage
{

PK_FrameBuffer::PK_FrameBuffer() : frame(&_frame, sizeof(_frame), getP_base()) {}

PK_FrameBuffer::PK_FrameBuffer(MODULE_NAME module_name, PACKAGE_NAME name, std::thread::id system)
    : Package(module_name, name, system),
      frame(&_frame, sizeof(_frame), getP_base()) {}

PK_FrameBuffer &PK_FrameBuffer::operator=(PK_FrameBuffer &&pack)
{
    static_cast<Package&>(*this) = std::move(pack);
    return *this;
}

} // md_showimage

#include "pk_lfreader.h"

namespace md_lfreader
{

PK_FrameBuffer::PK_FrameBuffer() : frame(&_frame, sizeof(_frame), getP_base()) {}

PK_FrameBuffer::PK_FrameBuffer(std::string name, std::string modul_name, std::thread::id system)
    : Package(name, modul_name, system),
      frame(&_frame, sizeof(_frame), getP_base()) {}

PK_FrameBuffer &PK_FrameBuffer::operator=(PK_FrameBuffer &&pack)
{
    static_cast<Package&>(*this) = std::move(pack);
    return *this;
}

} // md_lfreader

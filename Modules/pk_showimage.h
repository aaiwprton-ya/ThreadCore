#ifndef PK_SHOWIMAGE_H
#define PK_SHOWIMAGE_H

#include "std_afx.h"
#include "Core/package.h"

namespace md_showimage
{

using namespace Core;

class PK_FrameBuffer : public Package
{
    cv::Mat _frame;
public:
    Access::To<cv::Mat> frame;
public:
    PK_FrameBuffer();
    PK_FrameBuffer(MODULE_NAME module_name, PACKAGE_NAME name, std::thread::id system);
public:
    PK_FrameBuffer &operator=(PK_FrameBuffer &&pack);
public:
    class Key : public __BasePackKey__ {
    public: typedef PK_FrameBuffer type; };
};

} // md_showimage

#endif // PK_SHOWIMAGE_H

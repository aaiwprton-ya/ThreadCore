#ifndef PK_IOSYSTEM_H
#define PK_IOSYSTEM_H

#include "std_afx.h"
#include "Core/package.h"

namespace md_iosystem
{

using namespace Core;

class PK_OutputQueue : public Package
{
    std::list<std::string> _queue;
public:
    Access::To<std::list<std::string>> queue;
public:
    PK_OutputQueue();
    PK_OutputQueue(std::string name, std::string modul_name, std::thread::id system);
public:
    PK_OutputQueue &operator=(PK_OutputQueue &&pack);
public:
    class Key : public __BasePackKey__ {
    public: typedef PK_OutputQueue type; };
};

} // md_iosystem

#endif // PK_IOSYSTEM_H

#ifndef CORE_PKOUTPUTSTREAM_H
#define CORE_PKOUTPUTSTREAM_H

#include <queue>
#include <string>
#include "package.h"
#include "defines.h"

namespace Core {

class PKOutputStream : public Core::Package
{
    std::queue<std::string> queue;
public:
    PKOutputStream();
public:
    bool execute(std::string cmd) override;
};

} // namespace Core

#endif // CORE_PKOUTPUTSTREAM_H

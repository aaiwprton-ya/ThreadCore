#ifndef CORE_PKINPUTSTREAM_H
#define CORE_PKINPUTSTREAM_H

#include <queue>
#include <string>
#include "package.h"
#include "defines.h"

namespace Core {

class PKInputStream : public Core::Package
{
    std::queue<std::string> queue;
public:
    PKInputStream();
public:
    bool execute(std::string cmd) override;
};

} // namespace Core

#endif // CORE_PKINPUTSTREAM_H

#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

namespace Core {

class ArgumentParser
{
public:
    ArgumentParser();
};
// module comand flags=(F1 F2 F3) args=({object1:{name1:32,name2:"string",name3:[[1,2],[3,4],[5,6]]}})
// {"module":"module_name", "command":"command_name", "flags":["f1", "f2"], "args":"__json__"}
} // namespace Core

#endif // ARGUMENTPARSER_H

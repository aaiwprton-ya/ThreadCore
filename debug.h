#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>

namespace Core {

class Debug
{
    std::mutex *mtx = nullptr;
    std::condition_variable *con_var = nullptr;
    std::string context = "[DEBUG]";
public:
    Debug(std::mutex *mtx, std::condition_variable *con_var);
    void print();
};

} // namespace Core

#endif // DEBUG_H

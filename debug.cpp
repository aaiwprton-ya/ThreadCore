#include "debug.h"

namespace Core {

Debug::Debug(std::mutex *mtx, std::condition_variable *con_var)
    :
      mtx(mtx),
      con_var(con_var)
{

}

void Debug::print()
{
    std::unique_lock<std::mutex> un_lock(*mtx);
    con_var->wait(un_lock, [=](){return true;});
}

} // namespace Core

#include "system.h"

namespace Core {

System::System()
{
    r_main_room = rooms.generate_new();
    m_system = modules.insert(new Module(new IFSystem(this, r_main_room)));
    m_input = modules.insert(new Module(new IFInputStream(this, r_main_room)));
    const_cast<Interface*>(modules.get(m_input)->iface)->packages.push_back(
                data.insert(new DataPack(new PKInputStream())));
    const_cast<Interface*>(modules.get(m_input)->iface)->connects.push_back(m_system);
    std::thread t_input{(TrInputStream((IFInputStream*)modules.get(m_input)->iface))};
    t_input.detach();
    m_output = modules.insert(new Module(new IFOutputStream(this, r_main_room)));
    const_cast<Interface*>(modules.get(m_output)->iface)->packages.push_back(
                data.insert(new DataPack(new PKOutputStream())));
    const_cast<Interface*>(modules.get(m_output)->iface)->connects.push_back(m_system);
    std::thread t_output{(TrOutputStream((IFOutputStream*)modules.get(m_output)->iface))};
    t_output.detach();
}

System::~System()
{}
/*
* the main stream behavior:
*  - wait a user input
*  - lock the input queue
*  - push the data to the input queue
*  - set the input stream state as "RUNNING"
*  - send a wake up signal to the input stream
*  - unlock the input queue
*  - set the self state as "STOPPED"
*  - sleep and wait for the output stream send a wake up signal
*/

int System::exec()
{
    std::string cmd;
    while (true)
    {
        State st = modules.get(m_system)->iface->state;
        if (st == INIT)
        {
            const_cast<Interface*>(modules.get(m_system)->iface)->state = RUN;
        }
        else if (st == STOP)
        {
            std::cout << "STOPPED" << std::endl;
            const_cast<Interface*>(modules.get(m_system)->iface)->state = QUIT;
        }
        else if (st == RUN)
        {
            std::cout << ">>>";
            std::getline(std::cin, cmd);
            if (cmd == "exit")
            {
                const_cast<Interface*>(modules.get(m_system)->iface)->state = STOP;
            }
            else
            {
                //
                const_cast<Interface*>(modules.get(m_input)->iface)->execute(m_system, cmd);
            }
        }
        else if (st == QUIT)
        {
            std::cout << "QUIT" << std::endl;
            break;
        }
    }
    return 0;
}

} // namespace Core

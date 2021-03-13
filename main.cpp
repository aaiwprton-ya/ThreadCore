#include "std_afx.h"
//#include <iostream>
//#include <vector>
#include "Core/system.h"
#include "Modules/md_iosystem.h"

using namespace Core;
using namespace md_iosystem;

int main(int argc, char *argv[])
{
    System system;
    MD_IOSystem iosystem;
    system.include_module(&iosystem);
    std::cout << "Hello!" << std::endl;
    return system.exec();
}

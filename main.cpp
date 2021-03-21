#include "std_afx.h"
#include "Core/system.h"
#include "Modules/md_iosystem.h"
#include "Modules/md_lfreader.h"

using namespace Core;
using namespace md_iosystem;
using namespace md_lfreader;

int main(int argc, char *argv[])
{
    System system;
    MD_IOSystem iosystem;
    MD_LFReader lfreader;
    system.include_module(&iosystem);
    system.include_module(&lfreader);
    std::cout << "Hello!" << std::endl;
    return system.exec();
}

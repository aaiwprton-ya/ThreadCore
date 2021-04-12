#include "std_afx.h"
#include "Core/system.h"
#include "Modules/md_iosystem.h"
#include "Modules/md_lfreader.h"
#include "Modules/md_showimage.h"

using namespace Core;
using namespace md_iosystem;
using namespace md_lfreader;
using namespace md_showimage;

int main(int argc, char *argv[])
{
    System system;
    MD_IOSystem iosystem;
    MD_LFReader lfreader;
    MD_ShowImage show_image;
    system.include_module(&iosystem);
    system.include_module(&lfreader);
    system.include_module(&show_image);
    std::cout << "Hello!" << std::endl;
    return system.exec();
}

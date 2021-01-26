#include "interface.h"

namespace Core {

Interface::Interface(std::string name, System *system, id_type room, State state)
    :
      name(name),
      system(system),
      room(room),
      state(state)
{}

Interface::~Interface()
{}

} // namespace Core

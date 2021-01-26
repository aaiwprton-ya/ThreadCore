#ifndef ID_H
#define ID_H

#include "sortedlist.h"

typedef int id_type;

namespace Core {

struct ID
{
    SortedList<id_type> ocpd_id;
    SortedList<id_type> free_id;
    id_type next = 0;
    id_type max = -1;
public:
    void flush();
    void release(id_type id);
    void clear();
private:
    void update();
    void release_free();
};

} // namespace Core

#endif // ID_H

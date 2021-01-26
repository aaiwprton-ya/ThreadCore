#include "id.h"

namespace Core {

void ID::flush()
{
    ocpd_id.insert(next);
    free_id.release(next);
    update();
}

void ID::release(id_type id)
{
    ocpd_id.release(id);
    free_id.insert(id);
    release_free();
    update();
}

void ID::clear()
{
    ocpd_id.clear();
    free_id.clear();
    max = -1;
    next = 0;
}

void ID::update()
{
    max = ocpd_id.back();
    if (ocpd_id.empty())
    {
        max = -1;
    }
    if (!free_id.empty())
    {
        next = free_id.front();
    }
    else
    {
        next = max + 1;
    }
}

void ID::release_free()
{
    if (ocpd_id.empty())
    {
        free_id.clear();
        return;
    }
    while (!free_id.empty() && ocpd_id.back() < free_id.back())
    {
        free_id.pop_back();
    }
}

} // namespace Core

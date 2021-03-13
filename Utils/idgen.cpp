#include "idgen.h"

namespace Utils {

#include "idgen.h"

void IDGen::flush()
{
    ocpd_id.insert(next);
    free_id.release(next);
    update();
}

void IDGen::release(t_id id)
{
    ocpd_id.release(id);
    free_id.insert(id);
    release_free();
    update();
}

void IDGen::clear()
{
    ocpd_id.clear();
    free_id.clear();
    max = -1;
    next = 0;
}

bool IDGen::check_exist(t_id id) const noexcept
{
    return ocpd_id.exist(id);
}

void IDGen::update()
{
    max = ocpd_id.back();
    if (ocpd_id.empty()) {
        max = -1; }
    if (!free_id.empty()) {
        next = free_id.front(); }
    else {
        next = max + 1; }
}

void IDGen::release_free()
{
    if (ocpd_id.empty())
    {
        free_id.clear();
        return;
    }
    while (!free_id.empty() && ocpd_id.back() < free_id.back()) {
        free_id.pop_back(); }
}

} // namespace Utils

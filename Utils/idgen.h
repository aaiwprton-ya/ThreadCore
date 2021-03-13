#ifndef UTILS_IDGEN_H
#define UTILS_IDGEN_H

#include "std_afx.h"
//#include <vector>
#include "Utils/sortedlist.h"

namespace Utils {

template<typename id_base>
class __IDType__
{
    id_base id = -1;
public:
    __IDType__() : id(-1) {}
    __IDType__(id_base id) : id(id) {}
    __IDType__(const __IDType__ &id) : id(id.id) {}
    ~__IDType__() {}
public:
    operator id_base() const noexcept {
        return id; }
    explicit operator bool() const noexcept {
        return id >= 0; }
    inline __IDType__ operator=(const __IDType__ &value) noexcept
    {
        this->id = value.id;
        return *this;
    }
};

#define t_id __IDType__<int>

class IDGen
{
    SortedList<std::vector, t_id> ocpd_id;
    SortedList<std::vector, t_id> free_id;
public:
    t_id next = 0;
    t_id max = -1;
public:
    void flush();
    void release(t_id id);
    void clear();
    bool check_exist(t_id id) const noexcept;
private:
    void update();
    void release_free();
};

} // namespace Utils

#endif // UTILS_IDGEN_H

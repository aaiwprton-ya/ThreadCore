#ifndef CORE_POOL_H
#define CORE_POOL_H

#include <map>
#include "id.h"

namespace Core {

template <class T>
class Pool
{
    ID gen_id;
    typename std::map<id_type, T*> data;
public:
    Pool(){}
    ~Pool()
    {
        clear();
    }
public:
    id_type generate_new()
    {
        id_type next = gen_id.next;
        data.insert(std::pair<id_type, T*>(next, new T()));
        gen_id.flush();
        return next;
    }
    id_type insert(T *t)
    {
        id_type next = gen_id.next;
        data.insert(std::pair<id_type, T*>(next, t));
        gen_id.flush();
        return next;
    }
    T *get(id_type id)
    {
        return const_cast<T*>(data.at(id));
    }
    void release(id_type id)
    {
        delete data.at(id);
        data.erase(id);
        gen_id.release(id);
    }
    void clear()
    {
        for (auto item: data)
        {
            delete item.second;
        }
        data.clear();
        gen_id.clear();
    }
};

} // namespace Core

#endif // CORE_POOL_H

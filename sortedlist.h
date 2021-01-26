#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include <list>

template<class T>
class SortedList
{
    typename std::list<T> list;
public:
    void clear()
    {
        list.clear();
    }
    bool exist(typename std::list<T>::iterator it)
    {
        if (it != list.end())
        {
            return true;
        }
        return false;
    }
    T front()
    {
        return list.front();
    }
    T back()
    {
        return list.back();
    }
    void pop_front()
    {
        list.pop_front();
    }
    void pop_back()
    {
        list.pop_back();
    }
    void insert(T value)
    {
        if (find(value) != list.end())
        {
            return;
        }
        typename std::list<T>::iterator it = list.begin();
        while (value > *it && it != list.end())
        {
            ++it;
        }
        list.insert(it, value);
    }
    void release(T value)
    {
        if (find(value) == list.end())
        {
            return;
        }
        list.remove(value);
    }
    typename std::list<T>::iterator find(const T &value)
    {
        typename std::list<T>::iterator it = list.begin();
        for (const T &t: list)
        {
            if (t == value)
            {
                return it;
            }
            ++it;
        }
        return list.end();
    }
    bool empty()
    {
        return list.empty();
    }
};

#endif // SORTEDLIST_H

#ifndef UTILS_POOL_H
#define UTILS_POOL_H

#include "std_afx.h"
//#include <map>
//#include <memory>
#include "Utils/idgen.h"
#include "Core/package.h"
#include "Core/interface.h"
#include "Core/thread.h"

//Utils::IDGen Core::Package::__BasePackKey__::__id_gen__;
//Utils::IDGen Core::Interface::__BaseIfaceKey__::__id_gen__;
//Utils::IDGen Core::Thread::__BaseThreadKey__::__id_gen__;

namespace Utils {

//using namespace Core;


class __KeyBase__
{
protected:
    const void* const _key = nullptr;
protected:
    __KeyBase__(const void* const new_key)
        : _key(new_key)
    {}
public:
    virtual ~__KeyBase__() {}
};

template<class Unique>
class __KeyWrapper__ : public __KeyBase__
{
public:
    __KeyWrapper__(const Unique* const new_key)
        : __KeyBase__(new_key)
    {}
    ~__KeyWrapper__()
    {
        if (_key != nullptr) {
            delete (Unique*)_key; }
    }
public:
    Unique &key() const noexcept {
        return *((Unique*)_key); }
};

template <class Item>
class Pool
{
    IDGen id_gen;
    typename std::map<t_id, std::unique_ptr<Item>> items;
    std::map<t_id, __KeyBase__*> keys;
public:
    Pool() {}
    ~Pool() {
        clear(); }
public:
    template<class Unique, class ... Args>
    t_id generate_new(const Unique &key, Args ... args)
    {
        __check_key_not_exist__(key);
        t_id next = id_gen.next;
        items.insert(std::pair<t_id, std::unique_ptr<Item>>(next, std::make_unique<typename Unique::type>(args ...)));
        typename Unique::type test;
        keys.insert(std::pair<t_id, __KeyBase__*>(next, new __KeyWrapper__<Unique>(new Unique(key))));
        id_gen.flush();
        return next;
    }
    template<class Unique>
    t_id insert(std::unique_ptr<Item> item, const Unique &key)
    {
        __check_key_not_exist__(key);
        t_id next = id_gen.next;
        items.insert(std::pair<t_id, std::unique_ptr<Item>>(next, std::move(item)));
        keys.insert(std::pair<t_id, __KeyBase__*>(next, new __KeyWrapper__<Unique>(new Unique(key))));
        id_gen.flush();
        return next;
    }
    void release(t_id id)
    {
        __check_id_exist__(id);
        items.erase(id);
        delete keys.at(id);
        keys.erase(id);
        id_gen.release(id);
    }
    template<class Unique>
    void release(const Unique &key)
    {
        t_id id;
        __check_key_exist__(key, id);
        if (id)
        {
            items.erase(id);
            delete keys.at(id);
            keys.erase(id);
            id_gen.release(id);
        }
    }
    void clear()
    {
        for (auto k_pair: keys) {
            delete k_pair.second; }
        items.clear();
        keys.clear();
        id_gen.clear();
    }
    template<class Unique>
    typename Unique::type &operator[](const Unique &key) noexcept
    {
        t_id id;
        if (__check_key_exist_nex__(key, id)) {
            return static_cast<typename Unique::type&>(*(items.at(id).get())); }
        else {
            t_id id = generate_new(key);
            return static_cast<typename Unique::type&>((*this)[key]); }
    }
    template<class Unique>
    typename Unique::type &operator[](const Unique &key) const
    {
        t_id id;
        __check_key_exist__(key, id);
        return static_cast<typename Unique::type&>(*(items.at(id).get()));
    }
    Item &operator[](t_id id)
    {
        __check_id_exist__(id);
        return *(items.at(id).get());
    }
    Item &operator[](t_id id) const
    {
        __check_id_exist__(id);
        return *(items.at(id).get());
    }
    template<class Unique>
    t_id find(const Unique &key) const noexcept
    {
        for (auto k_pair: keys)
        {
            // holy code
            if ((static_cast<__KeyWrapper__<Unique>*>(k_pair.second))->key() == key) {
                return k_pair.first; }
        }
        return t_id(-1);
    }
private:
    template<class Unique>
    void __check_key_not_exist__(const Unique &key) const
    {
        if (this->find(key)) {
            throw std::runtime_error("error: duplicate key value"); }
    }
    template<class Unique>
    void __check_key_exist__(const Unique &key, t_id &id) const
    {
        if (!(id = this->find(key))) {
            throw std::runtime_error("error: the key does not exist"); }
    }
    template<class Unique>
    bool __check_key_exist_nex__(const Unique &key, t_id &id) const noexcept
    {
        try {
            __check_key_exist__(key, id); }
        catch (std::runtime_error ex) {
            return false; }
        return true;
    }
    void __check_id_not_exist__(t_id id) const
    {
        if (id_gen.check_exist(id)) {
            throw std::runtime_error("error: duplicate id value"); }
    }
    void __check_id_exist__(t_id id) const
    {
        if (!id_gen.check_exist(id)) {
            throw std::runtime_error("error: the id does not exist"); }
    }
};

} // namespace Utils

#endif // UTILS_POOL_H

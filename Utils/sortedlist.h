#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include "std_afx.h"
//#include <cstddef>

namespace Utils {

#define __const_iter__ typename ContainerType<ValueType>::const_iterator

template<template<class ...> class ContainerType, class ValueType>
class SortedList
{
    ContainerType<ValueType> container;
public:
    size_t size() {
        return container.size(); }
    void clear() noexcept {
        container.clear(); }
    bool exist(const ValueType &value) const noexcept {
        return (find(value) != container.end()); }
    bool exist(const ValueType &value, __const_iter__ &it) const noexcept {
        return ((it = find(value)) != container.end()); }
    ValueType front() const noexcept {
        return container.front(); }
    ValueType back() const noexcept {
        return container.back(); }
    void pop_front() noexcept {
        container.pop_front(); }
    void pop_back() noexcept {
        container.pop_back(); }
    void insert(ValueType value) noexcept
    {
        if (exist(value)) {
            return; }
        __const_iter__ it = container.begin();
        while (it != container.end() && value > *it) {
            ++it; }
        container.insert(it, value);
    }
    bool release(ValueType value) noexcept
    {
        __const_iter__ it;
        if (!exist(value, it)) {
            return false; }
        container.erase(it);
        return true;
    }
    __const_iter__ find(const ValueType &value) const noexcept
    {
        __const_iter__ it = container.begin();
        for (const ValueType &item: container)
        {
            if (item == value) {
                return it; }
            ++it;
        }
        return container.end();
    }
    bool empty() const noexcept {
        return container.empty(); }
};

} // namespace Utils

#endif // SORTEDLIST_H

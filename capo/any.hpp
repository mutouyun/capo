/*
    The Capo Library
    Code covered by the MIT License

    Modified from The Boost Library
    Modified by : mutouyun (http://orzz.org)

    Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
*/

#pragma once

#include "capo/assert.hpp"

#include <typeinfo> // typeid
#include <utility>  // std::swap

namespace capo {

////////////////////////////////////////////////////////////////

class any
{
    class place_holder
    {
    public:
        virtual ~place_holder(void) {}

        virtual const std::type_info& type(void) const = 0;
        virtual place_holder* clone(void) const = 0;
    };

    template <typename T>
    class holder : public place_holder
    {
    public:
        T held_;

    public:
        holder(const T& v): held_(v) {}

    public:
        const std::type_info & type(void) const { return typeid(T); }
        place_holder* clone(void) const         { return new holder{ held_ }; }
    };

    place_holder* content_;

public:
    any(void) : content_(nullptr) {}

    template <typename T>
    any(const T& v)
        : content_(new holder<T>{ v })
    {}

    any(const any& other)
        : content_((other.content_ == nullptr) ? nullptr : other.content_->clone())
    {}

    any(any&& rhs) : content_(nullptr)
    {
        swap(rhs);
    }

    ~any(void) { delete content_; }

    void swap(any& rhs)
    {
        std::swap(content_, rhs.content_);
    }

    any& operator=(any rhs)
    {
        rhs.swap(*this);
        return (*this);
    }

    const std::type_info & type(void) const
    {
        return (content_ == nullptr) ? typeid(void) : content_->type();
    }

    bool empty(void) const
    {
        return (content_ == nullptr);
    }

    void clear(void)
    {
        any{}.swap(*this);
    }

private:
    template <typename T> friend T* any_cast(any*);
};

/*
    Special swap algorithm
*/

inline void swap(any& x, any& y)
{
    x.swap(y);
}

/*
    cast any to a exact type
*/

template <typename T>
T* any_cast(any* operand)
{
    return (operand != nullptr) && (operand->type() == typeid(T))
            ? &static_cast<any::holder<T>*>(operand->content_)->held_
            : nullptr;
}

template <typename T>
T any_cast(any& operand)
{
    const T* result = any_cast<T>(&operand);
    if (result == nullptr) throw std::bad_cast{};
    return *result;
}

template <typename T>
const T* any_cast(const any* operand)
{
    return any_cast<T>(const_cast<any*>(operand));
}

template <typename T>
T any_cast(const any& operand)
{
    return any_cast<T>(const_cast<any&>(operand));
}

////////////////////////////////////////////////////////////////

} // namespace capo

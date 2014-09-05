/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_PATTERN_ITERATOR_HPP___
#define CAPO_PATTERN_ITERATOR_HPP___

#include "../type/type_list.hpp"
#include "../type/types_to_seq.hpp"

#include <tuple>    // std::tuple, std::get
#include <utility>  // std::move

namespace capo {

enum : size_t
{
    Infinity = static_cast<size_t>(-1)
};

////////////////////////////////////////////////////////////////
/// Iterator pattern
////////////////////////////////////////////////////////////////

template <class A, typename T, int StateSize = A::StateSize>
class iterator
{
public:
    using tp_t = types_assign_t<StateSize, std::tuple<T>>;
    using size_type = size_t;

private:
    tp_t   x_;
    size_t i_ = 0;

    using seq = list_to_seq<tp_t>;

    template <int... N> void next(constant_seq<N...>) { A::next(std::get<N>(x_)...); ++i_; }
    template <int... N> void prev(constant_seq<N...>) { A::prev(std::get<N>(x_)...); --i_; }
    template <int... N> void at(size_t n, constant_seq<N...>)
    {
        if (n != capo::Infinity)
            A::at(std::get<N>(x_)..., n);
        i_ = n;
    }

public:
    iterator(const tp_t& x)           : x_(x) {}
    iterator(const tp_t& x, size_t n) : x_(x) { at(n, seq()); }

    iterator(void)            = default;
    iterator(const iterator&) = default;

    iterator(iterator&& rhs) : x_(std::move(rhs.x_)) {}

public:
    T&       operator*(void)       { return std::get<0>(x_); }
    const T& operator*(void) const { return std::get<0>(x_); }

    T&       operator->(void)       { return this->operator*(); }
    const T& operator->(void) const { return this->operator*(); }

    bool operator==(const iterator& rhs) const { return (this->i_ == rhs.i_); }
    bool operator!=(const iterator& rhs) const { return !operator==(rhs); }

    tp_t& get_tuple(void) { return x_; }

    iterator& operator++(void)
    {
        next(seq());
        return (*this);
    }

    iterator& operator--(void)
    {
        prev(seq());
        return (*this);
    }

    iterator operator++(int)
    {
        iterator old(*this);
        ++(*this);
        return old;
    }

    iterator operator--(int)
    {
        iterator old(*this);
        --(*this);
        return old;
    }
};

////////////////////////////////////////////////////////////////

} // namespace capo

#endif // CAPO_PATTERN_ITERATOR_HPP___

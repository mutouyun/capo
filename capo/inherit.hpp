/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include <type_traits>  // std::is_class

namespace capo {

////////////////////////////////////////////////////////////////
/// Inherit a class type
////////////////////////////////////////////////////////////////

namespace detail_inherit
{
    template <class BaseT, bool = std::is_class<BaseT>::value>
    struct impl
    {
        using type = BaseT;
    };

    template <class BaseT>
    struct impl<BaseT, false>
    {
        using type = impl;
    };
}

template <class BaseT>
using inherit = typename detail_inherit::impl<BaseT>::type;

////////////////////////////////////////////////////////////////
/// Chain inherit
////////////////////////////////////////////////////////////////

namespace detail_inherit
{
    template <typename T,
              template <typename, typename> class...>
    struct chain;

    template <typename T,
              template <typename, typename> class B1,
              template <typename, typename> class... B>
    struct chain<T, B1, B...>
    {
    private:
        using tail = typename chain<T, B...>::type;
    public:
        using type = B1<T, tail>;
    };

    template <typename T>
    struct chain<T>
    {
        using type = void;
    };
}

template <typename T, template <typename, typename> class... BasesT>
using inherit_chain = inherit<typename detail_inherit::chain<T, BasesT...>::type>;

////////////////////////////////////////////////////////////////

} // namespace capo
/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/singleton.hpp"
#include "capo/list.hpp"

namespace capo {

////////////////////////////////////////////////////////////////

template <typename T, template <typename...> class SingleT = capo::use::single_shared>
class trackable
{
public:
    using list_t = capo::list<T*>;

    static list_t& list(void)
    {
        return capo::singleton<list_t, SingleT>();
    }

public:
    trackable(void)
    {
        // Always push the current object to the front.
        list().push_front(static_cast<T*>(this));
    }

    virtual ~trackable(void)
    {
        auto& l = list();
        for (auto it = l.begin(); it != l.end(); ++it)
        {
            T* ptr = (*it);
            if (ptr == nullptr) continue;
            if (ptr == this)
            {
                // If we found it, just erase it and return.
                l.erase(it);
                return;
            }
        }
    }
};

////////////////////////////////////////////////////////////////

} // namespace capo

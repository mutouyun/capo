/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_MEMORY_SCOPE_ALLOC_HPP___
#define CAPO_MEMORY_SCOPE_ALLOC_HPP___

#include "../memory/alloc_concept.hpp"

#include <cstddef>  // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// Scope allocation -- The destructor will release all memory blocks.
////////////////////////////////////////////////////////////////

template <class AllocT>
class scope_alloc
{
public:
    enum { AllocType = alloc_concept::RegionAlloc };
    using alloc_type = AllocT;

private:
    alloc_type alloc_;
    void*      list_;

public:
    scope_alloc(void)
        : list_(nullptr)
    {}

    scope_alloc(const alloc_type& r_alc)
        : alloc_(r_alc)
        , list_(nullptr)
    {}

    scope_alloc(const scope_alloc& rhs)
        : alloc_(rhs.alloc_)
        , list_(nullptr)
    {}

    ~scope_alloc(void) { clear(); }

public:
    size_t remain(void) const
    {
        size_t c = 0;
        void* curr = list_;
        while (curr != nullptr)
        {
            ++c;
            curr = *(void**)curr;
        }
        return c;
    }

    void clear(void)
    {
        while (list_ != nullptr)
        {
            void* curr = list_;
            list_ = *(void**)list_;
            alloc_.free(curr);
        }
        // Now list_ is nullptr.
        alloc_.clear();
    }

    void* alloc(size_t size)
    {
        void* curr = alloc_.alloc(sizeof(void*) + size);
        (*(void**)curr) = list_;
        return (void*)((void**)(list_ = curr) + 1);
    }

    void free(void* /*p*/, size_t /*size*/) {}
};

////////////////////////////////////////////////////////////////

} // namespace capo

#endif // CAPO_MEMORY_SCOPE_ALLOC_HPP___

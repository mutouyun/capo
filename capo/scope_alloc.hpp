/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/alloc_concept.hpp"
#include "capo/construct.hpp"
#include "capo/allocator.hpp"
#include "capo/noncopyable.hpp"

#include <functional>   // std::function
#include <utility>      // std::forward, std::swap
#include <cstddef>      // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// Scope allocation -- The destructor will release all memory blocks.
////////////////////////////////////////////////////////////////

template <class AllocP = CAPO_ALLOCATOR_POLICY_>
class scope_alloc final : capo::noncopyable
{
public:
    enum { AllocType = alloc_concept::GCAlloc };
    using alloc_policy = AllocP;

private:
    alloc_policy alloc_;

    struct block
    {
        block* next_;
        std::function<void()> destructor_;
    } * list_ = nullptr;

public:
    scope_alloc(void) = default;

    scope_alloc(scope_alloc&& rhs)            { this->swap(rhs); }
    scope_alloc& operator=(scope_alloc&& rhs) { this->swap(rhs); }

    ~scope_alloc(void) { clear(); }

public:
    void swap(scope_alloc& rhs)
    {
        std::swap(this->alloc_, rhs.alloc_);
        std::swap(this->list_ , rhs.list_);
    }

    size_t remain(void) const
    {
        size_t c = 0;
        block* curr = list_;
        while (curr != nullptr)
        {
            ++c;
            curr = curr->next_;
        }
        return c;
    }

    void clear(void)
    {
        while (list_ != nullptr)
        {
            block* curr = list_;
            list_ = list_->next_;
            if (curr->destructor_) curr->destructor_();
            capo::destruct(curr);
            alloc_.free(curr);
        }
        // Now list_ is nullptr.
        alloc_.clear();
    }

    void* alloc(size_t size)
    {
        block* curr = capo::construct<block>(alloc_.alloc(sizeof(block) + size));
        curr->next_ = list_;
        return ((list_ = curr) + 1);
    }

    template <typename F>
    void* alloc(size_t size, F&& destructor)
    {
        block* curr = capo::construct<block>(alloc_.alloc(sizeof(block) + size));
        curr->next_ = list_;
        curr->destructor_ = std::forward<F>(destructor);
        return ((list_ = curr) + 1);
    }

    template <typename T, typename... P>
    T* alloc(P&&... args)
    {
        block* curr = capo::construct<block>(alloc_.alloc(sizeof(block) + sizeof(T)));
        curr->next_ = list_;
        T* p = capo::construct<T>((list_ = curr) + 1, std::forward<P>(args)...);
        curr->destructor_ = [p]{ capo::destruct(p); };
        return p;
    }

    void free(void* /*p*/) {}
    void free(void* /*p*/, size_t /*size*/) {}
};

////////////////////////////////////////////////////////////////

} // namespace capo

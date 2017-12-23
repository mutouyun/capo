/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/concept.hpp"
#include "capo/alloc_concept.hpp"
#include "capo/construct.hpp"
#include "capo/allocator.hpp"

#include <functional>   // std::function
#include <utility>      // std::forward, std::swap
#include <cstddef>      // size_t

namespace capo {
namespace detail_scope_alloc_ {

CAPO_CONCEPT_MEMBER_(destructor_, std::function<void()> C::*);

template <class AllocP, class BlockT, bool = has_destructor_<BlockT>::value>
class impl_
{
public:
    enum { AllocType = alloc_concept::GCAlloc };
    using alloc_policy = AllocP;

protected:
    alloc_policy alloc_;
    BlockT* list_ = nullptr;

public:
    impl_(void) = default;

    impl_(impl_&& rhs)            { this->swap(rhs); }
    impl_& operator=(impl_&& rhs) { this->swap(rhs); return (*this); }

    ~impl_(void) { clear(); }

public:
    void swap(impl_& rhs)
    {
        std::swap(this->alloc_, rhs.alloc_);
        std::swap(this->list_ , rhs.list_);
    }

    size_t remain(void) const
    {
        size_t c = 0;
        BlockT* curr = list_;
        while (curr != nullptr)
        {
            ++c;
            curr = static_cast<BlockT*>(curr->next_);
        }
        return c;
    }

    void clear(void)
    {
        while (list_ != nullptr)
        {
            BlockT* curr = list_;
            list_ = static_cast<BlockT*>(list_->next_);
            capo::destruct(curr);
            alloc_.free(curr);
        }
        // Now list_ is nullptr.
        alloc_.clear();
    }

    void* alloc(size_t size)
    {
        BlockT* curr = capo::construct<BlockT>(alloc_.alloc(sizeof(BlockT) + size));
        curr->next_ = list_;
        return ((list_ = curr) + 1);
    }

    void free(void* /*p*/) {}
    void free(void* /*p*/, size_t /*size*/) {}
};

template <class AllocP, class BlockT>
class impl_<AllocP, BlockT, true> : public impl_<AllocP, BlockT, false>
{
    using base_t = impl_<AllocP, BlockT, false>;

protected:
    using base_t::alloc_;
    using base_t::list_;

public:
    using base_t::base_t;
    using base_t::alloc;

    template <typename F>
    void* alloc(size_t size, F&& destructor)
    {
        BlockT* curr = capo::construct<BlockT>(alloc_.alloc(sizeof(BlockT) + size));
        curr->next_ = list_;
        curr->destructor_ = std::forward<F>(destructor);
        return ((list_ = curr) + 1);
    }

    template <typename T, typename... P>
    T* alloc(P&&... args)
    {
        BlockT* curr = capo::construct<BlockT>(alloc_.alloc(sizeof(BlockT) + sizeof(T)));
        curr->next_ = list_;
        T* p = capo::construct<T>((list_ = curr) + 1, std::forward<P>(args)...);
        curr->destructor_ = [p]{ capo::destruct(p); };
        return p;
    }
};

} // namespace detail_scope_alloc_

namespace use {

struct block_normal
{
    block_normal* next_;
};

struct block_guard : block_normal
{
    std::function<void()> destructor_;
    ~block_guard(void) { if (destructor_) destructor_(); }
};

} // namespace use

////////////////////////////////////////////////////////////////
/// Scope allocation -- The destructor will release all memory blocks.
////////////////////////////////////////////////////////////////

template <class AllocP = CAPO_ALLOCATOR_POLICY_, class BlockT = capo::use::block_guard>
class scope_alloc final : public detail_scope_alloc_::impl_<AllocP, BlockT>
{
    using base_t = detail_scope_alloc_::impl_<AllocP, BlockT>;

public:
    using base_t::base_t;
    void swap(scope_alloc& rhs) { base_t::swap(rhs); }
};

////////////////////////////////////////////////////////////////

} // namespace capo

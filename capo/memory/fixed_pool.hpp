/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/sequence.hpp"
#include "capo/assert.hpp"
#include "capo/memory/alloc_concept.hpp"
#include "capo/memory/allocator.hpp"
#include "capo/memory/scope_alloc.hpp"
#include "capo/unused.hpp"

#include <utility>  // std::swap
#include <cstddef>  // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// Fixed-size blocks allocation
////////////////////////////////////////////////////////////////

#if !defined(CAPO_FIXED_POOL_ITERPOLICY_)
#   define CAPO_FIXED_POOL_ITERPOLICY_ capo::use::arithmetic<>
#endif/*!CAPO_FIXED_POOL_ITERPOLICY_*/

template <size_t BlockSize, 
          class PolicyT = CAPO_FIXED_POOL_ITERPOLICY_, 
          class AllocP = capo::scope_alloc<CAPO_ALLOCATOR_POLICY_, capo::use::block_normal>,
          bool = (BlockSize >= sizeof(void*))>
class fixed_pool;

template <size_t BlockSize, class PolicyT, class AllocP>
class fixed_pool<BlockSize, PolicyT, AllocP, true> final
{
public:
    enum { AllocType = alloc_concept::ObjectAlloc };
    using alloc_policy = AllocP;

private:
    alloc_policy alloc_;
    size_t init_expand_;
    void*  cursor_;

    capo::iterator<PolicyT, size_t> iter_;

    void expand(void)
    {
        if (*iter_ <= 0) ++iter_;
        CAPO_ASSERT_(*iter_ > 0)(*iter_);
        void** p = (void**)(cursor_ = alloc_.alloc(BlockSize * (*iter_)));
        CAPO_ASSERT_(p != nullptr);
        for (size_t i = 0; i < (*iter_) - 1; ++i)
            p = (void**)((*p) = ((char*)p) + BlockSize);
        (*p) = nullptr;
        ++iter_;
    }

    void init(void)
    {
        cursor_ = nullptr;
        CAPO_ASSERT_(init_expand_ != 0);
        iter_   = decltype(iter_)(init_expand_);
    }

    void init(size_t init_expand)
    {
        init_expand_ = init_expand;
        init();
    }

public:
    explicit fixed_pool(size_t init_expand = 1)
    {
        init(init_expand);
    }

    explicit fixed_pool(const alloc_policy& r_alc, size_t init_expand = 1)
        : alloc_(r_alc)
    {
        init(init_expand);
    }

    fixed_pool(fixed_pool&& rhs)            { this->swap(rhs); }
    fixed_pool& operator=(fixed_pool&& rhs) { this->swap(rhs); return (*this); }

    ~fixed_pool(void) { clear(); }

public:
    void swap(fixed_pool& rhs)
    {
        std::swap(this->alloc_      , rhs.alloc_);
        std::swap(this->init_expand_, rhs.init_expand_);
        std::swap(this->cursor_     , rhs.cursor_);
        std::swap(this->iter_       , rhs.iter_);
    }

    constexpr size_t block_size(void) const { return BlockSize; }

    size_t remain(void) const
    {
        size_t c = 0;
        void* curr = cursor_;
        while (curr != nullptr)
        {
            ++c;
            curr = *(void**)curr;
        }
        return c * BlockSize;
    }

    void clear(void)
    {
        alloc_.clear();
        init();
    }

    void* alloc(void)
    {
        if (cursor_ == nullptr) expand();
        CAPO_ASSERT_(cursor_ != nullptr);
        void* p = cursor_;
        cursor_ = *(void**)p;
        return p;
    }

    void free(void* p)
    {
        if (p == nullptr) return;
        *(void**)p = cursor_;
        cursor_ = p;
    }

    void* alloc(CAPO_UNUSED_ size_t size)
    {
        CAPO_ASSERT_(size <= BlockSize);
        return alloc();
    }

    void free(void* p, CAPO_UNUSED_ size_t size)
    {
        CAPO_ASSERT_(size <= BlockSize);
        free(p);
    }
};

////////////////////////////////////////////////////////////////

} // namespace capo

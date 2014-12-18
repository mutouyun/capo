/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/alloc_concept.hpp"
#include "capo/allocator.hpp"
#include "capo/assert.hpp"

#include <utility>  // std::move
#include <cstddef>  // std::max_align_t

namespace capo {
namespace detail_variable_pool {

struct head_t
{
    head_t* next_;
    size_t  size_;
    size_t  free_;
};

////////////////////////////////////////////////////////////////
/// head_t blocks cache
////////////////////////////////////////////////////////////////

class cache
{
    head_t* root_ = nullptr;

public:
    bool empty(void) const
    {
        return !root_;
    }

    size_t check_size(void) const
    {
        return root_ ? root_->free_ : 0;
    }

    head_t* pop(void)
    {
        head_t* curr = root_;
        if (root_)
            root_ = root_->next_;
        return curr;
    }

    void push(head_t* curr)
    {
        CAPO_ASSERT_(curr);
        if (curr->free_ >= check_size())
        {
            curr->next_ = root_;
            root_ = curr;
        }
        else
        {
            head_t* iter = root_;
            while (iter->next_ && iter->next_->free_ > curr->free_)
            {
                iter = iter->next_;
            }
            curr->next_ = iter->next_;
            iter->next_ = curr;
        }
    }
};

} // namespace detail_variable_pool

////////////////////////////////////////////////////////////////
/// Variable-size blocks allocation
////////////////////////////////////////////////////////////////

#if !defined(CAPO_VARIABLE_POOL_CHUNKSIZE_)
#   define CAPO_VARIABLE_POOL_CHUNKSIZE_ (sizeof(void*) << 10) /* 4K */
#endif/*!CAPO_VARIABLE_POOL_CHUNKSIZE_*/

template <class AllocP, size_t ChunkSize = CAPO_VARIABLE_POOL_CHUNKSIZE_>
class variable_pool
{
public:
    enum { AllocType = alloc_concept::RegionAlloc };
    using alloc_policy = AllocP;

private:
    using head_t = detail_variable_pool::head_t;

    variable_pool* father_ = nullptr;
    variable_pool* child_  = nullptr;
    detail_variable_pool::cache cache_;

    alloc_policy alloc_;

    char* head_;
    char* tail_;

    // List operations

    void init(void)
    {
        head_ = tail_ = reinterpret_cast<char*>(HeadSize);
    }

    head_t* chain(void)
    {
        return reinterpret_cast<head_t*>(head_ - HeadSize);
    }

    // Use cache for allocating a head

    head_t* alloc_head(size_t size)
    {
        CAPO_ASSERT_(size > HeadSize)(size);

        if (cache_.check_size() >= size)
            return cache_.pop();
        else
        if (father_ && father_->cache_.check_size() >= size)
            return father_->cache_.pop();
        else
        {
            size = (size < RequestSize) ? RequestSize : size;
            head_t* p = static_cast<head_t*>(alloc_.alloc(size));
            p->free_ = (p->size_ = size) - HeadSize;
            return p;
        }
    }

    void free_head(head_t* curr)
    {
        CAPO_ASSERT_(curr);
        if (father_)
            father_->cache_.push(curr);
        else
            alloc_.free(curr, curr->size_);
    }

    // Allocate memory block with alignment

    char* adjust_alignment(char* buff, size_t offset)
    {
        return reinterpret_cast<char*>(reinterpret_cast<size_t>(buff) & (~offset));
    }

    void* alloc_new_chunk(size_t size, size_t alignment)
    {
        size_t offset = alignment - 1;
        size_t adjust_size = offset + size;

        head_t* p = alloc_head(HeadSize + adjust_size);
        if (!p) return nullptr;
        head_t* list = chain();
        if (adjust_size > BufferSize && list)
        {
            p->next_ = list->next_;
            list->next_ = p;
            char* head = reinterpret_cast<char*>(p + 1);
            char* tail = adjust_alignment(head + p->free_ - size, offset);
            p->free_ = tail - head;
            return tail;
        }
        else
        {
            p->next_ = list;
            head_ = reinterpret_cast<char*>(p + 1);
            tail_ = adjust_alignment(head_ + p->free_ - size, offset);
            p->free_ = remain();
            return tail_;
        }
    }

public:
    enum : size_t
    {
        HeadSize    = sizeof(head_t),
        RequestSize = ChunkSize,
        BufferSize  = RequestSize - HeadSize
    };

public:
    variable_pool(void)
    {
        init();
    }

    variable_pool(const variable_pool& rhs)
        : father_(const_cast<variable_pool*>(&rhs))
        , alloc_(rhs.alloc_)
    {
        init();
        father_->child_ = this;
    }

    variable_pool(variable_pool&& rhs)
        : father_(rhs.father_)
        , child_(rhs.child_)
        , alloc_(std::move(rhs.alloc_))
        , head_(rhs.head_)
        , tail_(rhs.tail_)
    {
        rhs.child_ = rhs.father_ = nullptr;
        rhs.init();
    }

    ~variable_pool(void)
    {
        clear();
        // To remove self from father-child chain
        if (father_)
        {
            CAPO_ASSERT_(father_->child_ == this);
            father_->child_ = child_;
        }
        if (child_)
        {
            CAPO_ASSERT_(child_->father_ == this);
            child_->father_ = father_;
        }
    }

public:
    size_t remain(void) const
    {
        return (tail_ - head_);
    }

    void clear(void)
    {
        head_t* list = chain();
        while (list)
        {
            head_t* curr = list;
            list = list->next_;
            free_head(curr);
        }
        init();
        // Clean the cache
        while (!cache_.empty())
        {
            free_head(cache_.pop());
        }
    }

#if defined(__GNUC__)
    /*
        <GNUC> max_align_t should be in std namespace
        See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56019
    */
    void* alloc(size_t size, size_t alignment = alignof(::max_align_t))
#else /*!__GNUC__*/
    void* alloc(size_t size, size_t alignment = alignof(std::max_align_t))
#endif/*!__GNUC__*/
    {
        CAPO_ASSERT_(!(alignment & (alignment - 1)))(alignment);

        if (remain() < size)
        {
            return alloc_new_chunk(size, alignment);
        }

        char* buff = tail_ - size;
        size_t x = reinterpret_cast<size_t>(buff);
        x &= ~(x - 1); // calculate the alignment of buffer
        if (x < alignment)
        {
            buff = adjust_alignment(buff, alignment - 1);
            if (buff < head_)
            {
                return alloc_new_chunk(size, alignment);
            }
        }
        tail_ = buff;
        chain()->free_ = remain();
        return tail_;
    }

    void free(void* /*p*/) {}
    void free(void* /*p*/, size_t /*size*/) {}
};

} // namespace capo

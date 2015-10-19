/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/noncopyable.hpp"
#include "capo/alloc_concept.hpp"
#include "capo/allocator.hpp"
#include "capo/assert.hpp"

#include <utility>  // std::move, std::swap
#include <cstddef>  // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// Variable-size blocks allocation
////////////////////////////////////////////////////////////////

#if !defined(CAPO_VARIABLE_POOL_CHUNKSIZE_)
#   define CAPO_VARIABLE_POOL_CHUNKSIZE_ (sizeof(void*) << 10) /* 4K */
#endif/*!CAPO_VARIABLE_POOL_CHUNKSIZE_*/

template <size_t ChunkSize = CAPO_VARIABLE_POOL_CHUNKSIZE_, class AllocP = CAPO_ALLOCATOR_POLICY_>
class variable_pool final : capo::noncopyable
{
private:
    struct head_t
    {
        head_t* next_;
        size_t  size_;
        size_t  free_;
    };

public:
    enum { AllocType = alloc_concept::RegionAlloc };

    enum : size_t
    {
        HeadSize    = sizeof(head_t),
        RequestSize = ChunkSize,
        BufferSize  = RequestSize - HeadSize
    };

    using alloc_policy = AllocP;

private:
    alloc_policy alloc_;
    char* head_, * tail_;

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

        size = (size < RequestSize) ? RequestSize : size;
        head_t* p = static_cast<head_t*>(alloc_.alloc(size));
        p->free_ = (p->size_ = size) - HeadSize;
        return p;
    }

    void free_head(head_t* curr)
    {
        CAPO_ASSERT_(curr != nullptr);
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
        if (p == nullptr) return nullptr;
        head_t* list = chain();
        if (adjust_size > BufferSize && list != nullptr)
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

    void* alloc_new_chunk(size_t size)
    {
        head_t* p = alloc_head(HeadSize + size);
        if (p == nullptr) return nullptr;
        head_t* list = chain();
        if (size > BufferSize && list != nullptr)
        {
            p->next_ = list->next_;
            list->next_ = p;
            char* head = reinterpret_cast<char*>(p + 1);
            char* tail = head + p->free_ - size;
            p->free_ = tail - head;
            return tail;
        }
        else
        {
            p->next_ = list;
            head_ = reinterpret_cast<char*>(p + 1);
            tail_ = head_ + p->free_ - size;
            p->free_ = remain();
            return tail_;
        }
    }

public:
    variable_pool(void)
    {
        init();
    }

    variable_pool(variable_pool&& rhs)            { this->swap(rhs); }
    variable_pool& operator=(variable_pool&& rhs) { this->swap(rhs); }

    ~variable_pool(void)
    {
        clear();
    }

public:
    void swap(variable_pool& rhs)
    {
        std::swap(this->alloc_, rhs.alloc_);
        std::swap(this->head_ , rhs.head_);
        std::swap(this->tail_ , rhs.tail_);
    }

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
        alloc_.clear();
        init();
    }

    void* alloc(size_t size, size_t alignment)
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

    void* alloc(size_t size)
    {
        if (remain() < size)
        {
            return alloc_new_chunk(size);
        }
        char* buff = tail_ - size;
        if (buff < head_)
        {
            return alloc_new_chunk(size);
        }
        tail_ = buff;
        chain()->free_ = remain();
        return tail_;
    }

    void free(void* /*p*/) {}
    void free(void* /*p*/, size_t /*size*/) {}
};

} // namespace capo

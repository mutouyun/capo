/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_MEMORY_ALLOC_MALLOC_HPP___
#define CAPO_MEMORY_ALLOC_MALLOC_HPP___

#include <cstdlib>  // malloc, free, realloc
#include <cstddef>  // size_t

namespace capo {
namespace use {

struct alloc_malloc
{
    static void* alloc(size_t size)
    {
        return (size ? ::malloc(size) : nullptr);
    }

    static void free(void* p, size_t /*size*/)
    {
        if (p) ::free(p);
    }

    static void* realloc(void* p, size_t old_size, size_t new_size)
    {
        return (((p && old_size) || new_size) ? ::realloc(p, new_size) : nullptr);
    }
};

} // namespace use
} // namespace capo

#endif // CAPO_MEMORY_ALLOC_MALLOC_HPP___

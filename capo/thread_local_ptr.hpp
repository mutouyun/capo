/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/detect_plat.hpp"

#include <utility>      // std::forward

#if defined(CAPO_OS_WIN_)
#   include <map>       // std::map
#   include <windows.h> // Tls...
#else /*!CAPO_OS_WIN_*/
#   include <pthread.h> // pthread_...
#endif/*!CAPO_OS_WIN_*/

#if defined(_MSC_VER)
#   define CAPO_THREAD_LOCAL_POD_ __declspec(thread)
#else /*!_MSC_VER*/
#   define CAPO_THREAD_LOCAL_POD_ __thread
#endif/*!_MSC_VER*/

namespace capo {

#if defined(CAPO_OS_WIN_)

#define CAPO_THREAD_LOCAL_KEY_          DWORD
#define CAPO_THREAD_LOCAL_CREATE()      TlsAlloc()
#define CAPO_THREAD_LOCAL_DELETE(KEY)   TlsFree(KEY)
#define CAPO_THREAD_LOCAL_SET(KEY, PTR) (TlsSetValue(KEY, (LPVOID)PTR) == TRUE)
#define CAPO_THREAD_LOCAL_GET(KEY)      TlsGetValue(KEY)

/*
    <Remarks> Windows doesn't support a per-thread destructor with its TLS primitives.
    So we have to use the tls_data and thread_local_ptr.cpp to record destructors and do clean.
*/
namespace detail_thread_local_ptr
{
    struct tls_data
    {
        typedef void(*destructor_t)(void*);
        typedef std::map<CAPO_THREAD_LOCAL_KEY_, tls_data> map_t;

        static DWORD& key(void)
        {
            static CAPO_THREAD_LOCAL_KEY_ rec_key = CAPO_THREAD_LOCAL_CREATE();
            return rec_key;
        }

        static map_t* records(map_t* rec)
        {
            CAPO_THREAD_LOCAL_SET(key(), rec);
            return rec;
        }

        static map_t* records(void)
        {
            return static_cast<map_t*>(CAPO_THREAD_LOCAL_GET(key()));
        }

        CAPO_THREAD_LOCAL_KEY_ key_;
        destructor_t           destructor_;

        tls_data(CAPO_THREAD_LOCAL_KEY_ key, destructor_t destructor)
            : key_       (key)
            , destructor_(destructor)
        {}

        tls_data(tls_data&& rhs)
            : tls_data(rhs.key_, rhs.destructor_)
        {
            rhs.destructor_ = nullptr;
        }

        ~tls_data(void)
        {
            if (destructor_)
                destructor_(CAPO_THREAD_LOCAL_GET(key_));
        }
    };
}

#undef  CAPO_THREAD_LOCAL_CREATE
#define CAPO_THREAD_LOCAL_CREATE(KEY, DESTRUCTOR)               \
    do                                                          \
    {                                                           \
        KEY = TlsAlloc();                                       \
        using namespace detail_thread_local_ptr;                \
        auto rec = tls_data::records();                         \
        if (!rec) rec = tls_data::records(new tls_data::map_t); \
        if (!rec) break;                                        \
        rec->emplace(KEY, KEY, DESTRUCTOR);                     \
    } while(false)

#undef  CAPO_THREAD_LOCAL_DELETE
#define CAPO_THREAD_LOCAL_DELETE(KEY)            \
    do                                           \
    {                                            \
        using namespace detail_thread_local_ptr; \
        auto rec = tls_data::records();          \
        if (!rec) break;                         \
        rec->erase(KEY);                         \
        TlsFree(KEY);                            \
    } while(false)

#else /*!CAPO_OS_WIN_*/

#define CAPO_THREAD_LOCAL_KEY_                    pthread_key_t
#define CAPO_THREAD_LOCAL_CREATE(KEY, DESTRUCTOR) pthread_key_create(&KEY, DESTRUCTOR)
#define CAPO_THREAD_LOCAL_DELETE(KEY)             pthread_key_delete(KEY)
#define CAPO_THREAD_LOCAL_SET(KEY, PTR)           (pthread_setspecific(KEY, (void*)PTR) == 0)
#define CAPO_THREAD_LOCAL_GET(KEY)                pthread_getspecific(KEY)

#endif/*!CAPO_OS_WIN_*/

////////////////////////////////////////////////////////////////
/// Thread-local pointer
////////////////////////////////////////////////////////////////

/*
    <Remarks>

    1. In Windows, if you do not compile thread_local_ptr.cpp,
    use thread_local_ptr will cause memory leaks.

    2. If the first parameter of thread_local_ptr's constructor is false,
    you need to set the thread_local_ptr's storage manually:
    <code>
        thread_local_ptr<int> p; // or p(false);
        if (!p) p = new int(123);
        // ...
    <code/>
    Just like an ordinary pointer.
*/

template <typename T>
class thread_local_ptr
{
    CAPO_THREAD_LOCAL_KEY_ key_;

public:
    thread_local_ptr(void)
    {
        CAPO_THREAD_LOCAL_CREATE(key_, [](void* p)
        {
            delete static_cast<T*>(p);
        });
    }

    ~thread_local_ptr(void)
    {
        CAPO_THREAD_LOCAL_DELETE(key_);
    }

    T* operator=(T* ptr)
    {
        CAPO_THREAD_LOCAL_SET(key_, ptr);
        return ptr;
    }

    operator T*(void) const { return static_cast<T*>(CAPO_THREAD_LOCAL_GET(key_)); }

    T&       operator*(void)       { return *static_cast<T*>(*this); }
    const T& operator*(void) const { return *static_cast<T*>(*this); }
};

} // namespace capo

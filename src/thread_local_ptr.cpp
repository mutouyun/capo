/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#include "capo/detect_plat.hpp"

#if defined(CAPO_OS_WIN_)

#include "capo/thread_local_ptr.hpp"

#include <windows.h>

namespace capo {

using namespace detail_thread_local_ptr;

////////////////////////////////////////////////////////////////
/// Call destructors on thread exit
////////////////////////////////////////////////////////////////

void OnThreadExit(void)
{
    auto rec = tls_data::records();
    if (rec)
    {
        delete rec;
        tls_data::records(nullptr);
    }
}

////////////////////////////////////////////////////////////////
/*
    <Remarks> Windows doesn't support a per-thread destructor with its TLS primitives.
    So, here will build it manually by inserting a function to be called on each thread's exit.
    See: http://www.codeproject.com/threads/tls.asp
         http://src.chromium.org/chrome/trunk/src/base/threading/thread_local_storage_win.cc
         http://sourceforge.net/apps/trac/mingw-w64/browser/trunk/mingw-w64-crt/crt/tlssup.c
         http://svn.boost.org/svn/boost/trunk/libs/thread/src/win32/tss_pe.cpp
*/
////////////////////////////////////////////////////////////////

void NTAPI OnTlsCallback(PVOID, DWORD dwReason, PVOID)
{
    if (dwReason == DLL_THREAD_DETACH) OnThreadExit();
}

#if defined(_MSC_VER)

#if defined(CAPO_OS_WIN64_)

#pragma comment(linker, "/INCLUDE:_tls_used")
#pragma comment(linker, "/INCLUDE:_tls_xl_b__")

extern "C"
{
#   pragma const_seg(".CRT$XLB")
    extern const PIMAGE_TLS_CALLBACK _tls_xl_b__;
    const PIMAGE_TLS_CALLBACK _tls_xl_b__ = OnTlsCallback;
#   pragma const_seg()
}

#else /*!CAPO_OS_WIN64_*/

#pragma comment(linker, "/INCLUDE:__tls_used")
#pragma comment(linker, "/INCLUDE:__tls_xl_b__")

extern "C"
{
#   pragma data_seg(".CRT$XLB")
    PIMAGE_TLS_CALLBACK _tls_xl_b__ = OnTlsCallback;
#   pragma data_seg()
}

#endif/*!CAPO_OS_WIN64_*/

#elif defined(__GNUC__)

#define CAPO_CRTALLOC__(x) __attribute__ ((section (x) ))

#if defined(__MINGW64__) || (__MINGW64_VERSION_MAJOR) || \
    (__MINGW32_MAJOR_VERSION > 3) || ((__MINGW32_MAJOR_VERSION == 3) && (__MINGW32_MINOR_VERSION >= 18))

extern "C"
{
    CAPO_CRTALLOC__(".CRT$XLB") PIMAGE_TLS_CALLBACK _tls_xl_b__ = OnTlsCallback;
}

#else /*!__MINGW*/

extern "C"
{
    ULONG _tls_index__ = 0;

    CAPO_CRTALLOC__(".tls$AAA") char _tls_start__ = 0;
    CAPO_CRTALLOC__(".tls$ZZZ") char _tls_end__   = 0;

    CAPO_CRTALLOC__(".CRT$XLA") PIMAGE_TLS_CALLBACK _tls_xl_a__ = 0;
    CAPO_CRTALLOC__(".CRT$XLB") PIMAGE_TLS_CALLBACK _tls_xl_b__ = OnTlsCallback;
    CAPO_CRTALLOC__(".CRT$XLZ") PIMAGE_TLS_CALLBACK _tls_xl_z__ = 0;
}

extern "C" NX_CRTALLOC_(".tls") const IMAGE_TLS_DIRECTORY _tls_used =
{
    (ULONG_PTR)(&_tls_start__ + 1),
    (ULONG_PTR) &_tls_end__,
    (ULONG_PTR) &_tls_index__,
    (ULONG_PTR) &_tls_xl_b__,
    (ULONG)0, (ULONG)0
}

#endif/*!__MINGW*/

#undef CAPO_CRTALLOC__

#endif/*_MSC_VER, __GNUC__*/

} // namespace capo

#endif/*CAPO_OS_WIN_*/

/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

////////////////////////////////////////////////////////////////

#if defined(WINCE) || defined(_WIN32_WCE)
#   define CAPO_OS_WINCE_
#elif defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#   define CAPO_OS_WIN64_
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   define CAPO_OS_WIN32_
#elif defined(__linux__) || defined(__linux)
#   define CAPO_OS_LINUX_
#else
#   error "This OS is unsupported."
#endif

#if defined(CAPO_OS_WIN32_) || defined(CAPO_OS_WIN64_) || defined(CAPO_OS_WINCE_)
#   define CAPO_OS_WIN_
#endif

#if defined(CAPO_OS_WIN_)

#   ifndef _WIN32_WINNT
#       define _WIN32_WINNT     0x0501
#   endif
#   ifndef WINVER
#       define WINVER           _WIN32_WINNT
#   endif
#   ifndef _WIN32_WINDOWS
#       define _WIN32_WINDOWS   0x0410
#   endif
#   ifndef _WIN32_IE
#       define _WIN32_IE        0x0600
#   endif

#endif

////////////////////////////////////////////////////////////////

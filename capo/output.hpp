/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/type_traits.hpp"

#include <cstdio>       // vsnprintf
#include <cstdarg>      // va_list, va_start, va_end

namespace capo {

////////////////////////////////////////////////////////////////
/// Perpare for type-safe output
////////////////////////////////////////////////////////////////

namespace detail_output {

template <typename T>
struct pf;

template <> struct pf<char              > { static const char* val(void) { return "c"  ; } };
template <> struct pf<unsigned char     > { static const char* val(void) { return "c"  ; } };
template <> struct pf<wchar_t           > { static const char* val(void) { return "lc" ; } };
template <> struct pf<short             > { static const char* val(void) { return "d"  ; } };
template <> struct pf<unsigned short    > { static const char* val(void) { return "u"  ; } };
template <> struct pf<int               > { static const char* val(void) { return "d"  ; } };
template <> struct pf<unsigned int      > { static const char* val(void) { return "u"  ; } };
template <> struct pf<long              > { static const char* val(void) { return "ld" ; } };
template <> struct pf<unsigned long     > { static const char* val(void) { return "lu" ; } };
template <> struct pf<long long         > { static const char* val(void) { return "lld"; } };
template <> struct pf<unsigned long long> { static const char* val(void) { return "llu"; } };
template <> struct pf<float             > { static const char* val(void) { return "f"  ; } };
template <> struct pf<double            > { static const char* val(void) { return "g"  ; } };
template <> struct pf<long double       > { static const char* val(void) { return "Lg" ; } };
template <> struct pf<char*             > { static const char* val(void) { return "s"  ; } };
template <> struct pf<wchar_t*          > { static const char* val(void) { return "ls" ; } };
template <> struct pf<void*             > { static const char* val(void) { return "p"  ; } };
template <> struct pf<unsigned char*    > : pf<char*> {};

template <size_t N> struct pf<char[N]         > : pf<char*   > {};
template <size_t N> struct pf<unsigned char[N]> : pf<char*   > {};
template <size_t N> struct pf<wchar_t[N]      > : pf<wchar_t*> {};

template <typename F> typename std::enable_if<capo::is_closure<F>::value>::type 
    do_out(F&& out, const std::string& buf)
{
    out(buf);
}

template <typename F> typename std::enable_if<!capo::is_closure<F>::value>::type
    do_out(F&& out, const std::string& buf)
{
    out << buf;
}

template <typename F>
int output(F&& out, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string buf;
    int n = ::vsnprintf(NULL, 0, fmt, args);
    if (n <= 0) goto exit_output;
    buf.resize(n);
    n = ::vsnprintf(const_cast<char*>(buf.data()), n + 1, fmt, args);
    if (n <= 0) goto exit_output;
    do_out(out, buf);
exit_output:
    va_end(args);
    return n;
}



} // namespace detail_output

////////////////////////////////////////////////////////////////
/// Print data to output stream
////////////////////////////////////////////////////////////////

template <typename F, typename... T>
int output(F&& out, const char* fmt, T&&... args)
{
}

} // namespace capo

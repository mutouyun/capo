/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/type_traits.hpp"
#include "capo/printf.hpp"

#include <cstddef>      // std::size_t
#include <string>       // std::string
#include <utility>      // std::forward

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
template <> struct pf<double            > { static const char* val(void) { return "f"  ; } };
template <> struct pf<long double       > { static const char* val(void) { return "Lf" ; } };
template <> struct pf<char*             > { static const char* val(void) { return "s"  ; } };
template <> struct pf<wchar_t*          > { static const char* val(void) { return "ls" ; } };
template <> struct pf<void*             > { static const char* val(void) { return "p"  ; } };
template <> struct pf<unsigned char*    > : pf<char*> {};

template <size_t N> struct pf<char[N]         > : pf<char*   > {};
template <size_t N> struct pf<unsigned char[N]> : pf<char*   > {};
template <size_t N> struct pf<wchar_t[N]      > : pf<wchar_t*> {};

template <std::size_t N = 0>
void replace_placeholders(std::string& /*fmt*/)
{
    // Do Nothing.
}

template <std::size_t N = 0, typename A, typename... T>
void replace_placeholders(std::string& fmt, const A& a, T&&... args)
{
    using rep_t = typename std::remove_volatile<A>::type;
    std::string phstr = '{' + std::to_string(N);
    std::size_t found = 0;
    do
    {
    next_loop:
        found = fmt.find(phstr.c_str(), found);
        if (found == std::string::npos)
            break;
        else
        {
            if (found > 0)
            {
                if (fmt[found - 1] == '\\')
                {
                    found += phstr.size();
                    goto next_loop;
                }
            }
            std::size_t start = found + phstr.size();
            std::string buf("%");
            auto buf_out = [&buf](const std::string& str)
            {
                buf = std::move(str);
            };
            switch (fmt[start])
            {
            case '}':
                {
                    buf += pf<rep_t>::val();
                    capo::printf(buf_out, buf.c_str(), a);
                    fmt.replace(found, phstr.size() + 1, buf);
                }
                break;
            case ':':
                {
                    start += 1;
                    std::size_t brac = fmt.find('}', start);
                    if (brac == std::string::npos || fmt[brac - 1] == '\\')
                    {
                        found = start;
                        goto next_loop;
                    }
                    std::string cfg = fmt.substr(start, brac - start);
                    buf += cfg + pf<rep_t>::val();
                    capo::printf(buf_out, buf.c_str(), a);
                    fmt.replace(found, brac - found + 1, buf);
                }
                break;
            default:
                found = start;
                goto next_loop;
            }
        }
    } while (true);
    replace_placeholders<N + 1>(fmt, std::forward<T>(args)...);
}

} // namespace detail_output

////////////////////////////////////////////////////////////////
/// Print data to output stream
////////////////////////////////////////////////////////////////

template <typename F, typename... T>
void output(F&& out, const char* fmt, T&&... args)
{
    std::string buf(fmt);
    detail_output::replace_placeholders(buf, std::forward<T>(args)...);
    detail_printf::do_out(std::forward<F>(out), buf);
}

} // namespace capo

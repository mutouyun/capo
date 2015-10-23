/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/printf.hpp"

#include <string>       // std::string
#include <utility>      // std::forward
#include <cstdlib>      // std::atoi
#include <cstddef>      // size_t

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

template <typename T          > struct pf<T*                 > : pf<void*> {};
template <typename T          > struct pf<const T*           > : pf<T*   > {};
template <typename T          > struct pf<volatile T*        > : pf<T*   > {};
template <typename T          > struct pf<const volatile T*  > : pf<T*   > {};
template <typename T, size_t N> struct pf<T[N]               > : pf<T*   > {};
template <typename T, size_t N> struct pf<const T[N]         > : pf<T[N] > {};
template <typename T, size_t N> struct pf<volatile T[N]      > : pf<T[N] > {};
template <typename T, size_t N> struct pf<const volatile T[N]> : pf<T[N] > {};

template <std::size_t N>
bool is_match(std::string& cfg)
{
    if (cfg.empty()) return false;
    size_t sep_found = cfg.find(':');
    std::string num = cfg.substr(0, sep_found);
    if (N == std::atoi(num.c_str()))
    {
        if (sep_found == std::string::npos)
            cfg = "";
        else
            cfg = cfg.substr(sep_found + 1);
        return true;
    }
    return false;
}

template <std::size_t N = 0, typename A, typename... T>
void replace_placeholders(std::string& fmt, A&& a, T&&... args)
{
    using rep_t = typename std::decay<A>::type;
    size_t pos = 0;
    bool is_empty = false;
    do
    {
        do
        {
            pos = fmt.find('{', pos);
            if (pos == std::string::npos) goto continue_replace;
            if (fmt[++pos] == '{') ++pos;
            else break;
        } while (true);
        size_t end = fmt.find('}', pos);
        if (end == std::string::npos) break;
        std::string cfg = fmt.substr(pos, end - pos);
        // Removes blank characters. 
        auto cfg_remove = [&cfg](char c)
        {
            size_t pos = 0;
            do
            {
                pos = cfg.find(c, pos);
                if (pos == std::string::npos) break;
                cfg.erase(pos, 1);
            } while (true);
        };
        cfg_remove(' ');
        cfg_remove('\t');
        // Replaces the matching placeholder.
        auto fmt_replace = [&]
        {
            std::string buf("%");
            auto buf_out = [&buf](const std::string& str) { buf = std::move(str); };
            buf += cfg + pf<rep_t>::val();
            capo::printf(buf_out, buf.c_str(), std::forward<A>(a));
            fmt.replace(pos - 1, end - pos + 2, buf);
            pos += buf.size();
        };
        if (!is_empty && cfg.empty())
        {
            fmt_replace();
            is_empty = true;
        }
        else if (is_match<N>(cfg)) fmt_replace();
    } while (true);
    // Recurs next argument.
continue_replace:
    replace_placeholders<N + 1>(fmt, std::forward<T>(args)...);
}

template <std::size_t N = 0>
void replace_placeholders(std::string& fmt)
{
    auto replace_bracket = [&fmt](const char* s)
    {
        size_t pos = 0;
        do
        {
            pos = fmt.find(s, pos);
            if (pos == std::string::npos) break;
            fmt.replace(pos, 2, 1, s[0]);
        } while (true);
    };
    replace_bracket("{{");
    replace_bracket("}}");
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

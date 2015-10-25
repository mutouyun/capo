/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/printf.hpp"

#include <string>   // std::string
#include <utility>  // std::forward
#include <iostream> // std::cout
#include <cstdlib>  // std::atoi
#include <cstddef>  // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// Perpare for type-safe output
////////////////////////////////////////////////////////////////

namespace detail_output {

/*
    Predefine pf struct for making a type to the suitable format string.
*/

template <typename T> struct pf                     : std::false_type { constexpr static const char* val(void) { return "s"  ; } };
template <>           struct pf<char              > : std::true_type  { constexpr static const char* val(void) { return "c"  ; } };
template <>           struct pf<unsigned char     > : std::true_type  { constexpr static const char* val(void) { return "c"  ; } };
template <>           struct pf<wchar_t           > : std::true_type  { constexpr static const char* val(void) { return "lc" ; } };
template <>           struct pf<short             > : std::true_type  { constexpr static const char* val(void) { return "d"  ; } };
template <>           struct pf<unsigned short    > : std::true_type  { constexpr static const char* val(void) { return "u"  ; } };
template <>           struct pf<int               > : std::true_type  { constexpr static const char* val(void) { return "d"  ; } };
template <>           struct pf<unsigned int      > : std::true_type  { constexpr static const char* val(void) { return "u"  ; } };
template <>           struct pf<long              > : std::true_type  { constexpr static const char* val(void) { return "ld" ; } };
template <>           struct pf<unsigned long     > : std::true_type  { constexpr static const char* val(void) { return "lu" ; } };
template <>           struct pf<long long         > : std::true_type  { constexpr static const char* val(void) { return "lld"; } };
template <>           struct pf<unsigned long long> : std::true_type  { constexpr static const char* val(void) { return "llu"; } };
template <>           struct pf<float             > : std::true_type  { constexpr static const char* val(void) { return "f"  ; } };
template <>           struct pf<double            > : std::true_type  { constexpr static const char* val(void) { return "f"  ; } };
template <>           struct pf<long double       > : std::true_type  { constexpr static const char* val(void) { return "Lf" ; } };
template <>           struct pf<char*             > : std::true_type  { constexpr static const char* val(void) { return "s"  ; } };
template <>           struct pf<wchar_t*          > : std::true_type  { constexpr static const char* val(void) { return "ls" ; } };
template <>           struct pf<void*             > : std::true_type  { constexpr static const char* val(void) { return "p"  ; } };

template <>                     struct pf<unsigned char*     > : pf<char*> {};
template <typename T>           struct pf<T*                 > : pf<void*> {};
template <typename T>           struct pf<const T*           > : pf<T*   > {};
template <typename T>           struct pf<volatile T*        > : pf<T*   > {};
template <typename T>           struct pf<const volatile T*  > : pf<T*   > {};
template <typename T, size_t N> struct pf<T[N]               > : pf<T*   > {};
template <typename T, size_t N> struct pf<const T[N]         > : pf<T[N] > {};
template <typename T, size_t N> struct pf<volatile T[N]      > : pf<T[N] > {};
template <typename T, size_t N> struct pf<const volatile T[N]> : pf<T[N] > {};

template <std::size_t N = 0>
void replace_placeholders(std::string& fmt);

template <std::size_t N = 0, typename A, typename... T>
void replace_placeholders(std::string& fmt, A&& a, T&&... args);

template <typename F>
class impl_
{
    mutable std::string buf_;
    F out_;

public:
    template <typename T, CAPO_REQUIRE_(Different<impl_, T>::value)>
    impl_(T&& out) : out_(std::forward<T>(out)) {}

    impl_(impl_&&)            = default;
    impl_& operator=(impl_&&) = default;

    ~impl_(void)
    {
        if (buf_.empty()) return;
        typedef typename std::conditional<std::is_reference<F>::value, F, F&>::type out_t;
        detail_printf_::do_out((out_t)out_, std::move(buf_));
    }

    template <typename... T>
    const impl_& operator()(const char* fmt, T&&... args) const
    {
        std::string buf(fmt);
        replace_placeholders(buf, std::forward<T>(args)...);
        buf_ += std::move(buf);
        return (*this);
    }

    const impl_& ln(void) const
    {
        buf_ += "\n";
        return (*this);
    }
};

/*
    Judge the format flags is or not matching the number of argument.
*/

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

/*
    Print the buffer on the basis of the argument.
*/

template <typename A>
using rep_t = typename std::decay<A>::type;

template <typename A, CAPO_REQUIRE_(pf<rep_t<A>>::value)>
void printf_buffer(std::string& buf, std::string&& cfg, A&& a)
{
    auto buf_out = [&buf](std::string&& str) { buf = std::move(str); };
    buf = "%";
    if (cfg.empty())
    {
        buf += pf<rep_t<A>>::val();
    }
    else if (detail_printf_::is_specifier(cfg.back()))
    {
        buf += std::move(cfg);
    }
    else
    {
        buf += std::move(cfg) + pf<rep_t<A>>::val();
    }
    capo::printf(buf_out, buf.c_str(), std::forward<A>(a));
}

template <typename A, CAPO_REQUIRE_(!pf<rep_t<A>>::value)>
void printf_buffer(std::string& buf, std::string&& /*cfg*/, A&& a)
{
    std::forward<A>(a)(impl_<decltype(use::strout(buf))>{ use::strout(buf) });
}

/*
    Replace the placeholders in sequence.
*/

template <std::size_t N>
void replace_placeholders(std::string& fmt)
{
    size_t unmatched_l = 0, unmatched_r = 0;
    std::string tmp;
    for (size_t i = 0; i < fmt.size();)
    {
        auto enforce_matched = [&](char c) -> bool
        {
            if (fmt[i] == c)
            {
                ++i;
                return (i >= fmt.size() || fmt[i] != c);
            }
            return false;
        };
        if (enforce_matched('{')) { ++unmatched_l; continue; }
        if (enforce_matched('}')) { ++unmatched_r; continue; }
        if (unmatched_l == 0 && unmatched_r == 0)
        {
            tmp.push_back(fmt[i]);
        }
        ++i;
    }
    if (unmatched_l == unmatched_r)
    {
        if (unmatched_l == 0)
        {
            fmt = std::move(tmp);
            return;
        }
        detail_printf_::enforce("Invalid arguments's count");
    }
    else
    {
        std::string txt("Invalid format string: unmatched \'");
        txt += (unmatched_l < unmatched_r) ? '}' : '{';
        txt += "\' found";
        detail_printf_::enforce(std::move(txt));
    }
}

template <std::size_t N, typename A, typename... T>
void replace_placeholders(std::string& fmt, A&& a, T&&... args)
{
    size_t pos = 0;
    bool is_empty = false, has_used = false;
    do
    {
        // Finds brackets.
        do
        {
            pos = fmt.find('{', pos);
            if (pos == std::string::npos) goto continue_replace;
            if (fmt[++pos] == '{') ++pos;
            else break;
        } while (true);
        size_t end = fmt.find('}', pos);
        if (end == std::string::npos) goto continue_replace;
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
            std::string buf;
            printf_buffer(buf, std::move(cfg), std::forward<A>(a));
            fmt.replace(pos - 1, end - pos + 2, buf);
            pos += buf.size() - 1;
            has_used = true;
        };
        if (!is_empty && cfg.empty())
        {
            fmt_replace();
            is_empty = true;
        }
        else if (is_match<N>(cfg)) fmt_replace();
        else pos = end + 1;
    } while (true);
continue_replace:
    if (!has_used)
    {
        detail_printf_::enforce("Argument never used");
    }
    // Recurs next argument.
    replace_placeholders<N + 1>(fmt, std::forward<T>(args)...);
}

} // namespace detail_output

////////////////////////////////////////////////////////////////
/// Print data to output stream
////////////////////////////////////////////////////////////////

template <typename F>
using follower = detail_output::impl_<F>;

template <typename F, typename... T, CAPO_REQUIRE_(detail_printf_::OutputPred<F>::value)>
auto output(F&& out, const char* fmt, T&&... args)
{
    follower<F> flw { std::forward<F>(out) };
    flw(fmt, std::forward<T>(args)...);
    return std::move(flw);
}

template <typename... T>
auto output(const char* fmt, T&&... args)
{
    return capo::output(std::cout, fmt, std::forward<T>(args)...);
}

} // namespace capo

/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/type_name.hpp"
#include "capo/type_traits.hpp"
#include "capo/concept.hpp"

#include <string>       // std::string
#include <stdexcept>    // std::invalid_argument
#include <iostream>     // std::cout
#include <utility>      // std::forward, std::move
#include <cstdint>      // intmax_t, uintmax_t
#include <cstddef>      // size_t, ptrdiff_t
#include <cstdio>       // snprintf
#include <cstdarg>      // va_list, va_start, va_end

namespace capo {

////////////////////////////////////////////////////////////////
/// Perpare for type-safe printf
////////////////////////////////////////////////////////////////

namespace detail_printf_ {

inline void enforce(std::string&& what)
{
    throw std::invalid_argument
    {
        "Invalid format: " + what + "."
    };
}

template <typename T, typename U>
inline void enforce(void)
{
    if (!std::is_convertible<T, U>::value)
    {
        enforce(type_name<T>() + " => " + type_name<U>());
    }
}

template <typename T>
inline void enforce_argument(const char* fmt)
{
    using t_t = typename std::decay<T>::type;
    enum class length_t
    {
        none, h, hh, l, ll, j, z, t, L
    }
    state = length_t::none;
    for (; *fmt; ++fmt)
    {
        switch(*fmt)
        {
        // check specifiers's length
        case 'h':
                if (state == length_t::h) state = length_t::hh;
                else state = length_t::h; break;
        case 'l':
                if (state == length_t::l) state = length_t::ll;
                else state = length_t::l; break;
        case 'j': state = length_t::j; break;
        case 'z': state = length_t::z; break;
        case 't': state = length_t::t; break;
        case 'L': state = length_t::L; break;

        // check specifiers
        case 'd': case 'i':
            switch(state)
            {
            default:
            case length_t::none: enforce<t_t, int>();       break;
            case length_t::h:    enforce<t_t, short>();     break;
            case length_t::hh:   enforce<t_t, char>();      break;
            case length_t::l:    enforce<t_t, long>();      break;
            case length_t::ll:   enforce<t_t, long long>(); break;
            case length_t::j:    enforce<t_t, intmax_t>();  break;
            case length_t::z:    enforce<t_t, size_t>();    break;
            case length_t::t:    enforce<t_t, ptrdiff_t>(); break;
            }
            return;
        case 'u': case 'o': case 'x': case 'X':
            switch(state)
            {
            default:
            case length_t::none: enforce<t_t, unsigned int>();       break;
            case length_t::h:    enforce<t_t, unsigned short>();     break;
            case length_t::hh:   enforce<t_t, unsigned char>();      break;
            case length_t::l:    enforce<t_t, unsigned long>();      break;
            case length_t::ll:   enforce<t_t, unsigned long long>(); break;
            case length_t::j:    enforce<t_t, uintmax_t>();          break;
            case length_t::z:    enforce<t_t, size_t>();             break;
            case length_t::t:    enforce<t_t, ptrdiff_t>();          break;
            }
            return;
        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': case 'a': case 'A':
            switch(state)
            {
            default:
            case length_t::none: enforce<t_t, double>();      break;
            case length_t::L:    enforce<t_t, long double>(); break;
            }
            return;
        case 'c':
            switch(state)
            {
            default:
            case length_t::none: enforce<t_t, char>();    break;
            case length_t::l:    enforce<t_t, wchar_t>(); break;
            }
            return;
        case 's':
            switch(state)
            {
            default:
            case length_t::none: enforce<t_t, const char*>();    break;
            case length_t::l:    enforce<t_t, const wchar_t*>(); break;
            }
            return;
        case 'p':
            enforce<t_t, void*>();
            return;
        case 'n':
            switch(state)
            {
            default:
            case length_t::none: enforce<t_t, int*>();       break;
            case length_t::h:    enforce<t_t, short*>();     break;
            case length_t::hh:   enforce<t_t, char*>();      break;
            case length_t::l:    enforce<t_t, long*>();      break;
            case length_t::ll:   enforce<t_t, long long*>(); break;
            case length_t::j:    enforce<t_t, intmax_t*>();  break;
            case length_t::z:    enforce<t_t, size_t*>();    break;
            case length_t::t:    enforce<t_t, ptrdiff_t*>(); break;
            }
            return;
        }
    }
    enforce("Has no specifier");
}

inline void check(const char* fmt)
{
    for (; *fmt; ++fmt)
    {
        if (*fmt != '%' || *++fmt == '%') continue;
        enforce("Bad format");
    }
}

template <typename T1, typename... T>
inline void check(const char* fmt, T1&& /*a1*/, T&&... args)
{
    for (; *fmt; ++fmt)
    {
        if (*fmt != '%' || *++fmt == '%') continue;
        enforce_argument<T1>(fmt);
        return check(++fmt, args...);
    }
    enforce("Too few format specifiers");
}

template <typename F, CAPO_REQUIRE_(capo::is_closure<F>::value)>
inline void do_out(F&& out, std::string&& buf)
{
    out(std::move(buf));
}

template <typename F, CAPO_REQUIRE_(!capo::is_closure<F>::value)>
inline void do_out(F&& out, std::string&& buf)
{
    out << std::move(buf);
}

inline bool is_specifier(char c)
{
    static const char sps[] =
    {
        'd', 'i', 'u', 'o', 'x', 'X', 'f', 'F', 'e',
        'E', 'g', 'G', 'a', 'A', 'c', 's', 'p', 'n'
    };
    for (char s : sps) if (s == c) return true;
    return false;
}

#if defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat-security"
#endif/*__GNUC__*/
template <typename F, typename... A>
inline int impl_(F&& out, const char* fmt, A&&... args)
{
    std::string buf;
    int n = ::snprintf(nullptr, 0, fmt, std::forward<A>(args)...);
    if (n > 0)
    {
        buf.resize(n);
        n = ::snprintf(const_cast<char*>(buf.data()), n + 1, fmt, std::forward<A>(args)...);
        if (n != static_cast<int>(buf.size())) return n;
    }
    do_out(std::forward<F>(out), std::move(buf));
    return n;
}
#if defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif/*__GNUC__*/

CAPO_CONCEPT_TYPING_(can_shift_left, std::declval<T>() << std::declval<std::string>());

template <typename T>
CAPO_CONCEPT_(OutputPred, capo::is_closure<T>::value || can_shift_left<underlying<T>>::value);

} // namespace detail_printf_

////////////////////////////////////////////////////////////////
/// Print formatted data to output stream
////////////////////////////////////////////////////////////////

namespace use
{
    inline auto strout(std::string& buf)
    {
        return [&](std::string&& str)
        {
            buf = std::move(str);
        };
    }
}

template <typename F, typename... A, CAPO_REQUIRE_(detail_printf_::OutputPred<F>::value)>
inline int printf(F&& out, const char* fmt, A&&... args)
{
    if (fmt == nullptr) return 0;
    detail_printf_::check(fmt, std::forward<A>(args)...);
    return detail_printf_::impl_(std::forward<F>(out), fmt, std::forward<A>(args)...);
}

template <typename... A>
inline int printf(const char* fmt, A&&... args)
{
    return capo::printf(std::cout, fmt, std::forward<A>(args)...);
}

} // namespace capo

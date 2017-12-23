/*
    The Capo Library
    Code covered by the MIT License

    Modified from <<Enhancing Assertions>>
    (http://www.drdobbs.com/cpp/enhancing-assertions/184403745)
    Modified by : mutouyun (http://orzz.org)

    Copyright by Andrei Alexandrescu and John Torjo, August 01, 2003
*/

#pragma once

#include "capo/printf.hpp"
#include "capo/unused.hpp"

#include <map>          // std::map
#include <string>       // std::string
#include <sstream>      // std::ostringstream
#include <exception>    // std::exception
#include <cstdlib>      // abort

namespace capo {
namespace assert {

////////////////////////////////////////////////////////////////
/// Assert context storage
////////////////////////////////////////////////////////////////

struct context : std::exception
{
    enum level_t : int
    {
        LevelAbort,
        LevelExcept,
        LevelUser
    };

    typedef std::string        string_type;
    typedef std::ostringstream stream_type;
    typedef std::map<string_type, string_type> context_vals_type;
    context_vals_type context_vals_;

    const char* expr_;
    int         level_;
    const char* file_;
    unsigned    line_;
    const char* message_;

    string_type what_;

    context(const char* expr) noexcept
        : expr_(expr)
        , level_(LevelAbort)
        , file_(nullptr)
        , line_(0)
        , message_(nullptr)
    {}

    virtual ~context(void) noexcept
    {}

    template <typename T>
    void set_val(const char* str, const T& val) noexcept
    {
        try
        {
            stream_type ss;
            ss << val;
            context_vals_[str] = ss.str();
        }
        catch(...) {}
    }

    const char* get_val(const char* str) const noexcept
    {
        try
        {
            context_vals_type::const_iterator found = context_vals_.find(str);
            if (found != context_vals_.end())
                return found->second.c_str();
            else
                return "";
        }
        catch(...) { return ""; }
    }

    void flush(void) noexcept
    {
        try
        {
            stream_type ss;
            ss << "Failed in \"" << file_ << "\" ->: " << line_ << std::endl;
            ss << "Expression: \"" << expr_ << "\"" << std::endl;
            if (!context_vals_.empty())
            {
                ss << "Context Variables: " << std::endl;
                context_vals_type::iterator cc = context_vals_.begin();
                for (; cc != context_vals_.end(); ++cc)
                {
                    ss << "\t" << cc->first << " = " << cc->second << std::endl;
                }
            }
            if (message_)
            {
                ss << "Message: " << message_ << std::endl;
            }
            what_ = ss.str();
        }
        catch(...) {}
    }

    virtual const char* what() const noexcept
    {
        try
        {
            return what_.c_str();
        }
        catch(...) { return ""; }
    }
};

} // namespace assert

namespace detail_assert {

////////////////////////////////////////////////////////////////
/// Define assert impl
////////////////////////////////////////////////////////////////

#ifndef CAPO_ASSERT_HANDLER_
#define CAPO_ASSERT_HANDLER_ &capo::detail_assert::impl::assert_handler
#endif/*CAPO_ASSERT_HANDLER_*/

class impl
{
    using context_t = assert::context;

public:
    typedef void(*handler_t)(context_t&);

    // default assert handler
    static void assert_handler(context_t& context)
    {
        capo::printf(std::cerr, context.what());
        if (context.level_ == context_t::LevelAbort) ::abort();
    }

private:
    context_t context_;
    handler_t handler_;

public:
    impl(const char* expr) noexcept
        : context_(expr)
        , handler_(CAPO_ASSERT_HANDLER_)
    {}

    ~impl(void) noexcept
    {
        if (context_.level_ == context_t::LevelAbort) do_assert();
    }

    impl& do_assert(void) noexcept
    {
        context_.flush();
        handler_(context_);
        return (*this);
    }

    impl& handler(handler_t hd) noexcept
    {
        handler_ = hd;
        return (*this);
    }

    impl& context(const char* file, unsigned line) noexcept
    {
        context_.file_ = file;
        context_.line_ = line;
        return (*this);
    }

    impl& msg(const char* message) noexcept
    {
        context_.message_ = message;
        return (*this);
    }

    template <typename T>
    impl& add(const char* str, const T& val) noexcept
    {
        context_.set_val(str, val);
        return (*this);
    }

    /*
        Level settings
    */

    impl& level(int lvl) noexcept
    {
        switch(lvl)
        {
        case context_t::LevelAbort : abort() ; break;
        case context_t::LevelExcept: except(); break;
        default: context_.level_ = lvl;
        }
        return (*this);
    }

    impl& abort(void) noexcept
    {
        context_.level_ = context_t::LevelAbort;
        return (*this);
    }

    template <typename T>
    impl& except(const T& ept)
    {
        context_.level_ = context_t::LevelExcept;
        do_assert();
        throw ept;
        return (*this);
    }

    template <typename T>
    impl& except(void)
    {
        return except(T());
    }

    impl& except(void)
    {
        return except(context_);
    }
};

////////////////////////////////////////////////////////////////
/// Define assert wrapper
////////////////////////////////////////////////////////////////

template <bool>
class wrapper : public impl
{
public:
    using impl::handler_t;
    using impl::impl;

    wrapper& CAPO_ASSERT_1__ = (*this);
    wrapper& CAPO_ASSERT_2__ = (*this);

    wrapper& context(const char* file, unsigned line)
    {
        return static_cast<wrapper&>(impl::context(file, line));
    }

    template <typename T>
    wrapper& add(const char* str, const T& val)
    {
        return static_cast<wrapper&>(impl::add(str, val));
    }
};

template <>
class wrapper<false> : public impl
{
public:
    using impl::handler_t;
    using impl::impl;

    wrapper& CAPO_ASSERT_1__ = (*this);
    wrapper& CAPO_ASSERT_2__ = (*this);

    // Let the compiler can automatically make wrapper<false> invalid

    wrapper& context(const char* /*file*/, unsigned /*line*/) { return (*this); }

    template <typename T>
    wrapper& add(const char* /*str*/, const T& /*val*/) { return (*this); }
};

} // namespace detail_assert

////////////////////////////////////////////////////////////////
/// Evaluate assertion
////////////////////////////////////////////////////////////////

#define CAPO_ASSERT_1__(X)  CAPO_ASSERT_OP__(X, 2__)
#define CAPO_ASSERT_2__(X)  CAPO_ASSERT_OP__(X, 1__)
#define CAPO_ASSERT_OP__(X, next) \
        CAPO_ASSERT_1__.add(#X, (X)).CAPO_ASSERT_##next

#define CAPO_ENSURE_IMPL__(COND, ...) \
        const capo::detail_assert::impl& CAPO_UNUSED_ dummy_ = \
        capo::detail_assert::wrapper<COND>(#__VA_ARGS__).context(__FILE__, __LINE__).CAPO_ASSERT_1__

#define CAPO_ENSURE_(...) \
        if (!!(__VA_ARGS__)) ; \
        else CAPO_ENSURE_IMPL__(true, __VA_ARGS__)

#ifdef NDEBUG
#define CAPO_ASSERT_(...) \
        if (true) ; \
        else CAPO_ENSURE_IMPL__(false, __VA_ARGS__)
#else /*!NDEBUG*/
#define CAPO_ASSERT_(...) CAPO_ENSURE_(__VA_ARGS__)
#endif/*!NDEBUG*/

#ifdef NDEBUG
#define CAPO_VERIFY_(...) \
        if (!!(__VA_ARGS__) || true) ; \
        else CAPO_ENSURE_IMPL__(false, __VA_ARGS__)
#else
#define CAPO_VERIFY_(...) CAPO_ASSERT_(__VA_ARGS__)
#endif/*NDEBUG*/

} // namespace capo

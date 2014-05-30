/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_PROOFING_ASSERT_HPP___
#define CAPO_PROOFING_ASSERT_HPP___

#include "../proofing/printf.hpp"
#include "../macro/macro_unused.hpp"

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
            ss << "Context Variables: " << std::endl;
            context_vals_type::iterator cc = context_vals_.begin();
            for(; cc != context_vals_.end(); ++cc)
            {
                ss << "\t" << cc->first << " = " << cc->second << std::endl;
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

////////////////////////////////////////////////////////////////
/// Define assert impl
////////////////////////////////////////////////////////////////

#ifndef CAPO_ASSERT_HANDLER
#define CAPO_ASSERT_HANDLER &capo::assert::impl::assert_handler
#endif

class impl
{
    using context_t = assert::context;

public:
    typedef void(*handler_t)(context_t&);

    // default assert handler
    static void assert_handler(context_t& context)
    {
        capo::printf<capo::use::std_cerr>(context.what());
        if (context.level_ == context_t::LevelAbort) ::abort();
    }

private:
    context_t context_;
    handler_t handler_;

public:
    impl(const char* expr) noexcept
        : context_(expr)
        , handler_(CAPO_ASSERT_HANDLER)
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

} // namespace assert

////////////////////////////////////////////////////////////////
/// Evaluate assertion
////////////////////////////////////////////////////////////////

#define CAPO_ASSERT_1__(x)  CAPO_ASSERT_OP__(x, 2__)
#define CAPO_ASSERT_2__(x)  CAPO_ASSERT_OP__(x, 1__)
#define CAPO_ASSERT_OP__(x, next) \
        CAPO_ASSERT_1__.add(#x, (x)).CAPO_ASSERT_##next

#define CAPO_ENSURE_IMPL__(COND, ...) \
        const capo::assert::impl& CAPO_UNUSED_ dummy_ = \
        capo::assert::wrapper<COND>(#__VA_ARGS__).context(__FILE__, __LINE__).CAPO_ASSERT_1__

#define capo_ensure(...) \
        if (!!(__VA_ARGS__)) ; \
        else CAPO_ENSURE_IMPL__(true, __VA_ARGS__)

#ifdef NDEBUG
#define capo_assert(...) \
        if (true) ; \
        else CAPO_ENSURE_IMPL__(false, __VA_ARGS__)
#else /*NDEBUG*/
#define capo_assert(...) capo_ensure(__VA_ARGS__)
#endif/*NDEBUG*/

#ifdef NDEBUG
#define capo_verify(...) \
        if (!!(__VA_ARGS__) || true) ; \
        else CAPO_ENSURE_IMPL__(false, __VA_ARGS__)
#else
#define capo_verify(...) capo_assert(__VA_ARGS__)
#endif/*NDEBUG*/

} // namespace capo

#endif // CAPO_PROOFING_ASSERT_HPP___

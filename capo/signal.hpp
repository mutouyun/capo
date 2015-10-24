/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/vector.hpp"
#include "capo/type_traits.hpp"
#include "capo/types_to_seq.hpp"
#include "capo/max_min.hpp"
#include "capo/concept.hpp"

#include <functional>   // std::function
#include <utility>      // std::forward, std::move
#include <cstddef>      // size_t

namespace capo {
namespace detail_signal_ {

////////////////////////////////////////////////////////////////

// Function traits

template <typename S, typename F>
struct traits
     : std::conditional<std::is_same<void, typename capo::function_traits<F>::type>::value,
                        capo::function_traits<S>, capo::function_traits<F>>::type
{};

// Get suitable constant_seq<N...>

template <typename F, typename... P>
using suitable_size = 
    size_to_seq<min_number<std::tuple_size<typename traits<void(P...), F>::parameters>::value, sizeof...(P)>::value>;

// The concept for checking different underlying types

template <typename T, typename U>
CAPO_CONCEPT_(Different, !std::is_same<capo::underlying<T>, capo::underlying<U>>::value);

/*
    Define slot interface & the implementations
*/

template <typename R, typename... P>
struct slot_i
{
    virtual ~slot_i(void) {}
    virtual slot_i* clone(void) const = 0;
    virtual R call(P...) = 0;
};

template <class C, typename F, typename R, typename... P>
struct slot_fn;

template <typename F, typename... P>
struct slot_fn<void, F, void, P...> : slot_i<void, P...>
{
    F f_;

    template <typename F_, CAPO_REQUIRE_(Different<slot_fn, F_>::value)>
    slot_fn(F_&& f) : f_(std::forward<F_>(f)) {}

    slot_i<void, P...>* clone(void) const { return new slot_fn{ *this }; }

    template <int... N, typename Tp>
    void forward(constant_seq<N...>, Tp&& tp)
    {
        f_(std::get<N>(std::forward<Tp>(tp))...);
    }

    void call(P... args) override
    {
        this->forward(suitable_size<F, P...>{}, std::forward_as_tuple(static_cast<P&&>(args)...));
    }
};

template <typename F, typename R, typename... P>
struct slot_fn<void, F, R, P...> : slot_i<R, P...>
{
    F f_;

    template <typename F_, CAPO_REQUIRE_(Different<slot_fn, F_>::value)>
    slot_fn(F_&& f) : f_(std::forward<F_>(f)) {}

    slot_i<R, P...>* clone(void) const { return new slot_fn{ *this }; }

    template <int... N, typename Tp>
    R forward(constant_seq<N...>, Tp&& tp)
    {
        return f_(std::get<N>(std::forward<Tp>(tp))...);
    }

    R call(P... args) override
    {
        return this->forward(suitable_size<F, P...>{}, std::forward_as_tuple(static_cast<P&&>(args)...));
    }
};

template <class C, typename F, typename... P>
struct slot_fn<C, F, void, P...> : slot_i<void, P...>
{
    C c_;
    F f_;

    template <class C_, typename F_>
    slot_fn(C_&& c, F_&& f) : c_(std::forward<C_>(c)), f_(std::forward<F_>(f)) {}

    slot_i<void, P...>* clone(void) const { return new slot_fn{ *this }; }

    template <int... N, typename Tp>
    void forward(constant_seq<N...>, Tp&& tp)
    {
        (c_->*f_)(std::get<N>(std::forward<Tp>(tp))...);
    }

    void call(P... args) override
    {
        this->forward(suitable_size<F, P...>{}, std::forward_as_tuple(static_cast<P&&>(args)...));
    }
};

template <class C, typename F, typename R, typename... P>
struct slot_fn : slot_i<R, P...>
{
    C c_;
    F f_;

    template <class C_, typename F_>
    slot_fn(C_&& c, F_&& f) : c_(std::forward<C_>(c)), f_(std::forward<F_>(f)) {}

    slot_i<R, P...>* clone(void) const { return new slot_fn{ *this }; }

    template <int... N, typename Tp>
    R forward(constant_seq<N...>, Tp&& tp)
    {
        return (c_->*f_)(std::get<N>(std::forward<Tp>(tp))...);
    }

    R call(P... args) override
    {
        return this->forward(suitable_size<F, P...>{}, std::forward_as_tuple(static_cast<P&&>(args)...));
    }
};

/*
    Define slot class
*/

template <typename R, typename... P>
class slot
{
    using type = slot_i<R, P...>;

    type* s_ptr_   = nullptr;
    bool  blocked_ = false; // Slots can be temporarily "blocked".

public:
    slot(void) = default;

    slot(type* p)
        : s_ptr_(p)
    {}

    slot(const slot& rhs)
    {
        if (rhs.s_ptr_ != nullptr)
        {
            s_ptr_ = rhs.s_ptr_->clone();
        }
    }

    slot(slot&& rhs)
    {
        this->swap(rhs);
    }

    ~slot(void) { delete s_ptr_; }

    slot& operator=(slot rhs)
    {
        this->swap(rhs);
        return (*this);
    }

    void swap(slot& rhs)
    {
        std::swap(this->s_ptr_, rhs.s_ptr_);
    }

    template <typename T>
    T* cast(void) const
    {
        return dynamic_cast<T*>(s_ptr_);
    }

    void block(bool b)
    {
        blocked_ = b;
    }

    bool blocked(void) const
    {
        return blocked_;
    }

    explicit operator bool(void) const
    {
        return (!blocked()) && (s_ptr_ != nullptr);
    }

    template <typename... A>
    R operator()(A&&... args) const
    {
        return s_ptr_->call(std::forward<A>(args)...);
    }
};

/*
    Define signal base class
*/

template <typename F>
class signal_b;

template <typename R, typename... P>
class signal_b<R(P...)>
{
    using slots_t = capo::vector<slot<R, P...>>;

public:
    using value_type             = typename slots_t::value_type;
    using reference              = typename slots_t::reference;
    using const_reference        = typename slots_t::const_reference;
    using size_type              = typename slots_t::size_type;
    using iterator               = typename slots_t::iterator;
    using const_iterator         = typename slots_t::const_iterator;
    using reverse_iterator       = typename slots_t::reverse_iterator;
    using const_reverse_iterator = typename slots_t::const_reverse_iterator;

protected:
    slots_t slots_;

public:
    signal_b(void)            = default;
    signal_b(const signal_b&) = default;
    signal_b(signal_b&&)      = default;

    virtual ~signal_b(void) { disconnect(); }

public:
    void swap(signal_b& rhs)
    {
        slots_.swap(rhs.slots_);
    }

    size_t size(void) const
    {
        return slots_.size();
    }

    bool empty(void) const
    {
        return slots_.empty();
    }

    const_reference at(size_type pos) const { return slots_.at(pos); }
          reference at(size_type pos)       { return slots_.at(pos); }

    const_reference operator[](size_type pos) const { return slots_[pos]; }
          reference operator[](size_type pos)       { return slots_[pos]; }

    const_iterator begin(void) const { return slots_.begin(); }
          iterator begin(void)       { return slots_.begin(); }
    const_iterator end  (void) const { return slots_.end(); }
          iterator end  (void)       { return slots_.end(); }

    const_reverse_iterator rbegin(void) const { return slots_.rbegin(); }
          reverse_iterator rbegin(void)       { return slots_.rbegin(); }
    const_reverse_iterator rend  (void) const { return slots_.rend(); }
          reverse_iterator rend  (void)       { return slots_.rend(); }

    template <typename C, typename F>
    void connect(C&& receiver, F&& slot)
    {
        slots_.emplace_back(new slot_fn<typename std::remove_reference<C>::type, 
                                        typename std::remove_reference<F>::type, R, P...>
        {
            std::forward<C>(receiver), std::forward<F>(slot)
        });
    }

    template <typename F>
    void connect(F&& slot)
    {
        slots_.emplace_back(new slot_fn<void, typename std::remove_reference<F>::type, R, P...>
        {
            std::forward<F>(slot)
        });
    }

    template <typename C, typename F>
    void disconnect(C&& receiver, F&& slot)
    {
        for (auto it = slots_.begin(); it != slots_.end(); ++it)
        {
            auto sp = it->template cast<slot_fn<typename std::remove_reference<C>::type,
                                                typename std::remove_reference<F>::type, R, P...>>();
            if (sp == nullptr) continue;
            if ( (sp->c_ == std::forward<C>(receiver)) && 
                 (sp->f_ == std::forward<F>(slot)) )
            {
                slots_.erase(it);
                return;
            }
        }
    }

    template <typename F>
    void disconnect(F&& slot)
    {
        for (auto it = slots_.begin(); it != slots_.end(); ++it)
        {
            auto sp = it->template cast<slot_fn<void, typename std::remove_reference<F>::type, R, P...>>();
            if (sp == nullptr) continue;
            if (sp->f_ == std::forward<F>(slot))
            {
                slots_.erase(it);
                return;
            }
        }
    }

    void disconnect(void)
    {
        slots_.clear();
    }
};

} // namespace detail_signal_

////////////////////////////////////////////////////////////////
/// Define signal-slot service class
////////////////////////////////////////////////////////////////

template <typename F>
class signal;

template <typename... P>
class signal<void(P...)> : public detail_signal_::signal_b<void(P...)>
{
public:
    signal(void)          = default;
    signal(const signal&) = default;
    signal(signal&&)      = default;

    signal& operator=(signal rhs)
    {
        this->swap(rhs);
        return (*this);
    }

    template <typename... A>
    void operator()(A&&... args) const
    {
        for (auto& sp : this->slots_)
        {
            if (!sp) continue;
            sp(std::forward<A>(args)...);
        }
    }
};

template <typename R, typename... P>
class signal<R(P...)> : public detail_signal_::signal_b<R(P...)>
{
    std::function<R(capo::vector<R>&)> combiner_;

public:
    signal(void)          = default;
    signal(const signal&) = default;
    signal(signal&&)      = default;

    signal& operator=(signal rhs)
    {
        this->swap(rhs);
        return (*this);
    }

    void swap(signal& rhs)
    {
        detail_signal_::signal_b<R(P...)>::swap(rhs);
        combiner_.swap(rhs.combiner_);
    }

    explicit signal(std::function<R(capo::vector<R>&)> f)
        : combiner_(std::move(f))
    {}

    void combiner(std::function<R(capo::vector<R>&)> f)
    {
        combiner_ = std::move(f);
    }

    template <typename... A>
    R operator()(A&&... args) const
    {
        if (this->slots_.empty()) return {};
        capo::vector<R> results;
        for (auto& sp : this->slots_)
        {
            if (!sp) continue;
            results.push_back(sp(std::forward<A>(args)...));
        }
        return combiner_ ? combiner_(results) : results.back();
    }
};

////////////////////////////////////////////////////////////////

} // namespace capo

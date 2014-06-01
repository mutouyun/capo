/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_THREAD_SPIN_LOCK_HPP___
#define CAPO_THREAD_SPIN_LOCK_HPP___

#include <atomic>       // std::atomic_flag, std::atomic_signal_fence
#include <thread>       // std::this_thread
#include <chrono>       // std::chrono::milliseconds
#if defined(_MSC_VER)
#include <windows.h>    // YieldProcessor
#endif/*_MSC_VER*/

namespace capo {
namespace detail_spinlock {

////////////////////////////////////////////////////////////////
/// Gives hint to processor that improves performance of spin-wait loops.
////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
/*
    See: http://msdn.microsoft.com/en-us/library/windows/desktop/ms687419(v=vs.85).aspx
    Not for intel c++ compiler, so ignore http://software.intel.com/en-us/forums/topic/296168
*/
#   define CAPO_THREADOPS_PAUSE__() YieldProcessor()
#elif defined(__GNUC__)
#if defined(__i386__) || defined(__x86_64__)
/*
    See: Intel(R) 64 and IA-32 Architectures Software Developer's Manual V2
         PAUSE-Spin Loop Hint, 4-57
         http://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-instruction-set-reference-manual-325383.html?wapkw=instruction+set+reference
*/
#   define CAPO_THREADOPS_PAUSE__() __asm__ __volatile__("pause")
#elif defined(__ia64__) || defined(__ia64)
/*
    See: Intel(R) Itanium(R) Architecture Developer's Manual, Vol.3
         hint - Performance Hint, 3:145
         http://www.intel.com/content/www/us/en/processors/itanium/itanium-architecture-vol-3-manual.html
*/
#   define CAPO_THREADOPS_PAUSE__() __asm__ __volatile__ ("hint @pause")
#elif defined(__arm__)
/*
    See: ARM Architecture Reference Manuals (YIELD)
         http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.subset.architecture.reference/index.html
*/
#   define CAPO_THREADOPS_PAUSE__() __asm__ __volatile__ ("yield")
#endif
#endif/*compilers*/

#if !defined(CAPO_THREADOPS_PAUSE__)
/*
    Just use a compiler fence, prevent compiler from optimizing loop
*/
#   define CAPO_THREADOPS_PAUSE__() std::atomic_signal_fence(std::memory_order_seq_cst)
#endif/*!defined(CAPO_THREADOPS_PAUSE__)*/

////////////////////////////////////////////////////////////////
/// Yield to other threads
////////////////////////////////////////////////////////////////

inline void yield(unsigned k)
{
    if (k < 4)  { /* Do nothing */ }
#ifdef CAPO_THREADOPS_PAUSE__
    else
    if (k < 16) { CAPO_THREADOPS_PAUSE__(); }
#endif/*CAPO_THREADOPS_PAUSE__*/
    else
    if (k < 32) { std::this_thread::yield(); }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

} // namespace detail_spinlock

////////////////////////////////////////////////////////////////
/// Spinlock
////////////////////////////////////////////////////////////////

class spin_lock
{
#if defined(_MSC_VER) && (_MSC_VER <= 1800)
    /*
        <MSVC 2013> Atomic flag cannot be initialized in a constructor initializer.
        See: http://connect.microsoft.com/VisualStudio/feedback/details/800243/
    */
    std::atomic_flag lc_;
public:
    spin_lock(void) { lc_.clear(std::memory_order_relaxed); }
#else /*_MSC_VER*/
    std::atomic_flag lc_ = ATOMIC_FLAG_INIT;
#endif/*_MSC_VER*/

public:
    bool try_lock(void)
    {
        return !lc_.test_and_set(std::memory_order_acquire);
    }

    void lock(void)
    {
        for (unsigned k = 0; lc_.test_and_set(std::memory_order_acquire); ++k)
            detail_spinlock::yield(k);
    }

    void unlock(void)
    {
        lc_.clear(std::memory_order_release);
    }
};

} // namespace capo

#endif // CAPO_THREAD_SPIN_LOCK_HPP___

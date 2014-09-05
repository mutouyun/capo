/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_UTILITY_NONCOPYABLE_HPP___
#define CAPO_UTILITY_NONCOPYABLE_HPP___

namespace capo
{
    class noncopyable
    {
    protected:
        noncopyable(void) = default;
       ~noncopyable(void) = default;
    public:
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
}

#endif // CAPO_UTILITY_NONCOPYABLE_HPP___

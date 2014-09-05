/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_MACRO_FUNC_HPP___
#define CAPO_MACRO_FUNC_HPP___

////////////////////////////////////////////////////////////////

#ifdef CAPO_FUNC_
#   error "CAPO_FUNC_ has been defined."
#endif

#if defined(_MSC_VER)
#   define CAPO_FUNC_ __FUNCSIG__
#elif defined(__GNUC__)
#   define CAPO_FUNC_ __PRETTY_FUNCTION__
#else
#   define CAPO_FUNC_ __func__
#endif

////////////////////////////////////////////////////////////////

#endif // CAPO_MACRO_FUNC_HPP___

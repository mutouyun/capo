/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

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

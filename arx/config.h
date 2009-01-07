#ifndef __ARX_CONFIG_H__
#define __ARX_CONFIG_H__

// -------------------------------------------------------------------------- //
// Config settings
// -------------------------------------------------------------------------- //
/** @def ARX_USE_BOOST
 * Define to use implementation of some features from boost library instead of hand-coded one.
 * Normally boost classes offer wider functionality and sometimes produce faster code. */

/** @def ARX_USE_EIGEN
 * Define to use Eigen library in place of ArX LinearAlgebra module. Eigen is a highly-optimized
 * library with a rich set of features, which generally produces better code that ArX LinearAlgebra. */

/** @def ARX_DISABLE_THREADS
 * Define to disable thread support and thread safety in ArX library. May result in
 * faster code for single-threaded applications. */

/** @def ARX_UNROLL_COST_LIMIT
 * Cost limit for templated cycle unrolling in LinearAlgebra module. Default value is 100. */
#ifndef ARX_UNROLL_COST_LIMIT
#  define ARX_UNROLL_COST_LIMIT 100
#endif


// -------------------------------------------------------------------------- //
// Guess defines - do not change
// -------------------------------------------------------------------------- //
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define ARX_WIN32
#elif defined(__linux__)
#  define ARX_LINUX
#endif

#if defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#  define ARX_ICC
#elif defined __GNUC__
#  define ARX_GCC
#elif defined _MSC_VER
#  define ARX_MSVC
#endif

#if defined(_DEBUG) || !defined(NDEBUG)
#  define ARX_DEBUG
#endif

#ifdef ARX_USE_BOOST
#  ifdef ARX_DISABLE_THREADS
#    define BOOST_DISABLE_THREADS
#  endif
#endif


// -------------------------------------------------------------------------- //
// Some useful defines
// -------------------------------------------------------------------------- //
#ifdef ARX_MSVC
#  define FORCEINLINE __forceinline
# else
#  define FORCEINLINE inline
#endif



#endif

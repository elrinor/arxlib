#ifndef __ARX_CONFIG_H__
#define __ARX_CONFIG_H__

// -------------------------------------------------------------------------- //
// General settings - feel free to change
// -------------------------------------------------------------------------- //
/** Use boost? */
// #define ARX_USE_BOOST

/** For statically sized matrices with cols >= ARX_LINEAR_SOLVER_PERMUTATION_VERTOR_USAGE_THRESH
 * we use linear solver that doesn't swap matrix rows directly in memory, but performs all operations
 * indirectly, by means of a permutation vector */
#define ARX_LINEAR_SOLVER_PERMUTATION_VERTOR_USAGE_THRESH 5

/**
 * Cost limit for templated cycle unrolling in LinearAlgebra module.
 */
#define ARX_UNROLL_COST_LIMIT 100

/** Multithreading on? */
// #define ARX_DISABLE_THREADS


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

#if defined(_DEBUG) && !defined(DEBUG)
#  define DEBUG
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
#  define ARX_FORCEINLINE __forceinline
# else
#  define ARX_FORCEINLINE inline
#endif

#endif

#ifndef __ARX_CONFIG_H__
#define __ARX_CONFIG_H__

// -------------------------------------------------------------------------- //
// Config: General & Global
// -------------------------------------------------------------------------- //
/** @def ARX_DISABLE_THREADS
 * Define to disable thread support and thread safety in ArX library. May result in
 * faster code for single-threaded applications. */

/** @def ARX_DISABLE_EXCEPTIONS
 * Define to disable exception handling in ArX Library. When defined, ArX library
 * won't throw exceptions, but also won't try handle exceptions in user code. 
 * Note that in this case all the error will become non-interceptable, since
 * error handling code will revert to assertions instead of exceptions. */


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
#elif defined(__GNUC__) || defined(__GNUG__)
#  define ARX_GCC
#elif defined _MSC_VER
#  define ARX_MSVC
#endif


// -------------------------------------------------------------------------- //
// Derived defines
// -------------------------------------------------------------------------- //
#ifdef ARX_DISABLE_EXCEPTIONS
#  define ARX_TRY        {{
#  define ARX_CATCH_ALL  } if(0) {
#  define ARX_CATCH(X)   } if(0) {
#  define ARX_RETHROW    
#  define ARX_THROW(X)   
#  define ARX_TRY_END    }}
#  define ARX_ASSERT_OR_THROW(CONDITION, HANDLER)                               \
  assert(CONDITION);
#else
#  define ARX_TRY        try {
#  define ARX_CATCH_ALL  } catch(...) {
#  define ARX_CATCH(X)   } catch(X) {
#  define ARX_RETHROW    throw;
#  define ARX_THROW(X)   throw(X);
#  define ARX_TRY_END    }
#  define ARX_ASSERT_OR_THROW(CONDITION, HANDLER)                               \
  if(!(CONDITION)) {                                                            \
    HANDLER;                                                                    \
  }
#endif

#endif // __ARX_CONFIG_H__

/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2010 Alexander Fokin <apfokin@gmail.com>
 *
 * ArXLib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * ArXLib is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ArXLib. If not, see <http://www.gnu.org/licenses/>. 
 * 
 * $Id$ */
#ifndef ARX_CONFIG_H
#define ARX_CONFIG_H

// -------------------------------------------------------------------------- //
// Config: General & Global
// -------------------------------------------------------------------------- //
/** @def ARX_DISABLE_THREADS
 *
 * Define to disable thread support and thread safety in ArX library. May result in
 * faster code for single-threaded applications. 
 */

/** @def ARX_DISABLE_EXCEPTIONS
 *
 * Define to disable exception handling in ArX Library. When defined, ArX library
 * won't throw exceptions, but also won't try handle exceptions in user code. 
 * Note that in this case all the error will become non-interceptable, since
 * error handling code will revert to assertions instead of exceptions. 
 */


// -------------------------------------------------------------------------- //
// Config: Image import & export
// -------------------------------------------------------------------------- //
/** @def ARX_USE_QT_IMAGE_IO
 * 
 * If defined, Qt image manipulation routines are used for loading and saving
 * of vigra images.
 */

/** @def ARX_USE_MAGICK_IMAGE_IO
 * 
 * If defined, ImageMagick image manipulation routines are used for loading and
 * saving of vigra images.
 */

#if !defined(ARX_USE_QT_IMAGE_IO) && !defined(ARX_USE_MAGICK_IMAGE_IO)
#  define ARX_USE_QT_IMAGE_IO
#endif

#if defined(ARX_USE_QT_IMAGE_IO) && defined(ARX_USE_MAGICK_IMAGE_IO)
#  error Both ARX_USE_QT_IMAGE_IO and ARX_USE_MAGICK_IMAGE_IO are defined.
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
#elif defined(__GNUC__) || defined(__GNUG__)
#  define ARX_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined _MSC_VER
#  define ARX_MSVC
#endif


// -------------------------------------------------------------------------- //
// Derived defines
// -------------------------------------------------------------------------- //
#ifdef ARX_DISABLE_EXCEPTIONS
#  include <boost/preprocessor/stringize.hpp>
#  define ARX_TRY        
#  define ARX_CATCH_ALL  if(0)
#  define ARX_CATCH(X)   if(0)
#  define ARX_RETHROW    
#  define ARX_THROW(X)   
#  define ARX_ASSERT_OR_THROW(CONDITION, HANDLER)                               \
  assert((CONDITION) && BOOST_PP_STRINGIZE(HANDLER));
#else
#  define ARX_TRY        try
#  define ARX_CATCH_ALL  catch(...)
#  define ARX_CATCH(X)   catch(X)
#  define ARX_RETHROW    throw
#  define ARX_THROW(X)   throw(X)
#  define ARX_ASSERT_OR_THROW(CONDITION, THROW_EXPRESSION)                      \
  if(!(CONDITION)) {                                                            \
    THROW_EXPRESSION;                                                           \
  }
#endif


#endif // ARX_CONFIG_H

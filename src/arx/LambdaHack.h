#ifndef __ARX_LAMBDAHACK_H__
#define __ARX_LAMBDAHACK_H__

#include <boost/tr1/functional.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/preprocessor.hpp>

#ifdef BOOST_HAS_TR1_FUNCTION
/* We do a really bad thing here... Standard says that adding anything into std
 * namespace results in undefined behavior. Don't ever try it at home! */
namespace std { namespace tr1 {
#else
namespace boost {
#endif

/* The following code snippet was adapted from the cradle project,
 * 	http://cradle.sourceforge.jp. Copyright notice follows. */

// Copyright 2004, 2005, 2006 Cryolite.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define ADAPT_LAMBDA_FOR_RESULT_OF(Z, I, _)                                     \
  template<                                                                     \
    class T                                                                     \
    BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(I, class BOOST_PP_INTERCEPT, A)        \
  >                                                                             \
  struct result_of<                                                             \
    boost::lambda::lambda_functor<T>(BOOST_PP_ENUM_PARAMS(I, A))                \
  >:                                                                            \
  public boost::lambda::lambda_functor<T>::template sig<                        \
    boost::tuple<                                                               \
      boost::lambda::lambda_functor<T>                                          \
      BOOST_PP_ENUM_TRAILING_PARAMS(I, A)                                       \
    >                                                                           \
  > {};                                                                         \
                                                                                \
  template<                                                                     \
    class T                                                                     \
    BOOST_PP_ENUM_TRAILING_BINARY_PARAMS(I, class BOOST_PP_INTERCEPT, A)        \
  >                                                                             \
  struct result_of<                                                             \
    boost::lambda::lambda_functor<T> const(BOOST_PP_ENUM_PARAMS(I, A))          \
  >:                                                                            \
  public boost::lambda::lambda_functor<T>::template sig<                        \
    boost::tuple<                                                               \
      boost::lambda::lambda_functor<T>                                          \
      BOOST_PP_ENUM_TRAILING_PARAMS(I, A)                                       \
    >                                                                           \
  > {};                                                                         \

BOOST_PP_REPEAT(10, ADAPT_LAMBDA_FOR_RESULT_OF, _)

#ifdef BOOST_HAS_TR1_FUNCTION
}} // namespace std::tr1
#else
} // namespace boost
#endif

#endif // __ARX_LAMBDAHACK_H__

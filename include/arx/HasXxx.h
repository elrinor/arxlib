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
#ifndef ARX_HAS_XXX_H
#define ARX_HAS_XXX_H

#include "config.h"
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_class.hpp>


// -------------------------------------------------------------------------- //
// ARX_DEFINE_HAS_TYPE_TRAIT
// -------------------------------------------------------------------------- //
#define ARX_DEFINE_HAS_TYPE_TRAIT(member_name)                                 \
  BOOST_MPL_HAS_XXX_TRAIT_DEF(member_name)

/** 
 * Generates a trait class that detects whether the given type has a nested 
 * type with the given name. 
 *
 * @param trait_name                   Trait name.
 * @param member_name                  Name of a type to check. 
 */
#define ARX_DEFINE_NAMED_HAS_TYPE_TRAIT(trait_name, member_name)               \
  BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait_name, member_name, false)


// -------------------------------------------------------------------------- //
// ARX_DEFINE_HAS_ANY_FUNC_TRAIT
// -------------------------------------------------------------------------- //
#define ARX_DEFINE_NAMED_HAS_ANY_FUNC_TRAIT_I(trait_name, helper_name, func_name) \
struct helper_name {                                                            \
  typedef char true_type;                                                       \
  struct false_type { true_type dummy[2]; };                                    \
                                                                                \
  template<class T, T> struct wrapper {};                                       \
                                                                                \
  struct mixin {                                                                \
    void func_name();                                                           \
  };                                                                            \
                                                                                \
  template<class T> struct mixed {                                              \
    struct type: public T, public mixin {};                                     \
  };                                                                            \
                                                                                \
  template<class T>                                                             \
  static false_type has_member(T *, wrapper<void (mixin::*)(), &mixed<T>::type::func_name> * = NULL); \
  static true_type has_member(...);                                             \
};                                                                              \
                                                                                \
template<class T, bool is_class = boost::is_class<T>::value>                    \
struct trait_name:                                                              \
  boost::mpl::bool_<                                                            \
    sizeof(helper_name::has_member(static_cast<T *>(NULL))) ==                  \
    sizeof(helper_name::true_type)                                              \
  >                                                                             \
{};                                                                             \
                                                                                \
template<class T>                                                               \
struct trait_name<T, false>: public boost::mpl::false_ {};


/** 
 * Generates a trait class that detects whether the given type has a member 
 * function with the given name. Note that it returns true even if that 
 * function is not accessible (e.g. private).
 *
 * @param trait_name                   Trait name.
 * @param func_name                    Name of a function to check.
 */
#define ARX_DEFINE_NAMED_HAS_ANY_FUNC_TRAIT(trait_name, func_name)              \
  ARX_DEFINE_NAMED_HAS_ANY_FUNC_TRAIT_I(trait_name, BOOST_PP_CAT(trait_name, _helper), func_name)

#define ARX_DEFINE_HAS_ANY_FUNC_TRAIT(func_name)                                \
  ARX_DEFINE_NAMED_HAS_ANY_FUNC_TRAIT(BOOST_PP_CAT(has_any_, func_name), func_name)


// -------------------------------------------------------------------------- //
// ARX_DEFINE_HAS_EXACT_FUNC_TRAIT
// -------------------------------------------------------------------------- //
#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_II(trait_name, helper_name, func_name) \
struct helper_name {                                                            \
  typedef char true_type;                                                       \
  struct false_type { true_type dummy[2]; };                                    \
                                                                                \
  template<class Signature, Signature> struct member {};                        \
                                                                                \
  template<class T, class Signature>                                            \
  static true_type has_member(T *, Signature *, helper_name::member<Signature, &T::func_name> * = NULL); \
  static false_type has_member(...);                                            \
};                                                                              \
                                                                                \
template<class T, class Signature, bool is_class = boost::is_class<T>::value>   \
struct trait_name:                                                              \
  boost::mpl::bool_<                                                            \
    sizeof(helper_name::has_member(static_cast<T *>(NULL), static_cast<Signature *>(NULL))) == \
    sizeof(helper_name::true_type)                                              \
  >                                                                             \
{};                                                                             \
                                                                                \
template<class T, class Signature>                                              \
struct trait_name<T, Signature, false>: public boost::mpl::false_ {};

#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(trait_name, func_name)          \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_II(                                     \
    trait_name,                                                                 \
    BOOST_PP_CAT(trait_name, _helper),                                          \
    func_name                                                                   \
  )


/**
 * Generates a traits class that detect if a given type X has a member function
 * named Name with a given signature.
 *
 * Note that invocation of this trait will result in a compilation error if
 * the function being checked is private.
 */
#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT(trait_name, func_name)            \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(trait_name, func_name)

#define ARX_DEFINE_HAS_EXACT_FUNC_TRAIT(func_name)                              \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(BOOST_PP_CAT(has_exact_, func_name), func_name)



// -------------------------------------------------------------------------- //
// ARX_DEFINE_HAS_FUNC_TRAIT
// -------------------------------------------------------------------------- //
#define ARX_DEFINE_NAMED_HAS_FUNC_TRAIT_II(trait_name, any_trait_name, exact_trait_name, func_name) \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(exact_trait_name, func_name)          \
  ARX_DEFINE_NAMED_HAS_ANY_FUNC_TRAIT(any_trait_name, func_name)                \
                                                                                \
  template<class T, class Signature = boost::mpl::na>                           \
  struct trait_name: public exact_trait_name<T, Signature> {};                  \
                                                                                \
  template<class T>                                                             \
  struct trait_name<T, boost::mpl::na>: public any_trait_name<T> {};

#define ARX_DEFINE_NAMED_HAS_FUNC_TRAIT_I(trait_name, func_name)                \
  ARX_DEFINE_NAMED_HAS_FUNC_TRAIT_II(                                           \
    trait_name,                                                                 \
    BOOST_PP_CAT(trait_name, _any),                                             \
    BOOST_PP_CAT(trait_name, _exact),                                           \
    func_name                                                                   \
  )

#define ARX_DEFINE_NAMED_HAS_FUNC_TRAIT(trait_name, func_name)                  \
  ARX_DEFINE_NAMED_HAS_FUNC_TRAIT_I(trait_name, func_name)

#define ARX_DEFINE_HAS_FUNC_TRAIT(func_name)                                    \
  ARX_DEFINE_NAMED_HAS_FUNC_TRAIT_I(BOOST_PP_CAT(has_, func_name), func_name)


#endif // ARX_HAS_XXX_H

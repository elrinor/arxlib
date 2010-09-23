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
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/member_function_pointer.hpp>
#include <boost/function_types/property_tags.hpp>


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
// ARX_DEFINE_HAS_FUNC_TRAIT
// -------------------------------------------------------------------------- //
#define ARX_HAS_FUNC_TRAIT_MEMBER_PREFIX_normal() 
#define ARX_HAS_FUNC_TRAIT_MEMBER_PREFIX_static() static
#define ARX_HAS_FUNC_TRAIT_MEMBER_PREFIX(func_type)                  \
  BOOST_PP_CAT(ARX_HAS_FUNC_TRAIT_MEMBER_PREFIX_, func_type)

#define ARX_HAS_FUNC_TRAIT_MEMBER_POINTER_DECL_normal(type) type::*
#define ARX_HAS_FUNC_TRAIT_MEMBER_POINTER_DECL_static(type) *
#define ARX_HAS_FUNC_TRAIT_MEMBER_POINTER_DECL(func_type)                 \
  BOOST_PP_CAT(ARX_HAS_FUNC_TRAIT_MEMBER_POINTER_DECL_, func_type)

#define ARX_DEFINE_NAMED_EXTENDED_HAS_FUNC_TRAIT_I(trait_name, func_name, func_return_type, func_return_statement, func_params, decl_prefix_macro, pointer_decl_macro) \
struct BOOST_PP_CAT(trait_name, _helper) {                                      \
  typedef char true_type;                                                       \
  struct false_type { true_type dummy[2]; };                                    \
  template<class T, T> struct wrapper {};                                       \
                                                                                \
  struct mixin {                                                                \
    decl_prefix_macro() func_return_type func_name func_params { func_return_statement; } \
  };                                                                            \
                                                                                \
  template<class T> struct mixed {                                              \
    struct type: public T, public mixin {};                                     \
  };                                                                            \
                                                                                \
  template<class T>                                                             \
  static false_type has_member(T *, wrapper<func_return_type (pointer_decl_macro(mixin)) func_params, &mixed<T>::type::func_name>* = NULL); \
  static true_type has_member(...);                                             \
};                                                                              \
                                                                                \
template<class T, bool is_class = boost::is_class<T>::value>                    \
struct trait_name:                                                              \
  boost::mpl::bool_<                                                            \
    sizeof(BOOST_PP_CAT(trait_name, _helper)::has_member(static_cast<T *>(NULL))) == \
    sizeof(BOOST_PP_CAT(trait_name, _helper)::true_type)                        \
  >                                                                             \
{};                                                                             \
                                                                                \
template<class T>                                                               \
struct trait_name<T, false>: public boost::mpl::false_ {};


/** 
 * Generates a trait class that detects whether the given type has a member 
 * function with the given name. Note that it returns true even if that 
 * function is not accessible (i.e. private).
 *
 * @param trait_name                   Trait name.
 * @param func_name                    Name of a function to check.
 * @param func_type                    Type of a function, must be either static or normal.
 * @param func_return_type             Return type of the checking function. 
 *                                     Is meaningful only if C++ constrains the possible return type 
 *                                     of the function being checked (as in case of operator new).
 * @param func_return_statement        Return statement of the checking function.
 * @param func_params                  Parameters of a checking function, in parentheses. 
 */
#define ARX_DEFINE_NAMED_EXTENDED_HAS_FUNC_TRAIT(trait_name, func_name, func_type, func_return_type, func_return_statement, func_params) \
  ARX_DEFINE_NAMED_EXTENDED_HAS_FUNC_TRAIT_I(                                   \
    trait_name,                                                                 \
    func_name,                                                                  \
    func_return_type,                                                           \
    func_return_statement,                                                      \
    func_params,                                                                \
    ARX_HAS_FUNC_TRAIT_MEMBER_PREFIX(func_type),                     \
    ARX_HAS_FUNC_TRAIT_MEMBER_POINTER_DECL(func_type)                     \
  )

#define ARX_DEFINE_NAMED_HAS_FUNC_TRAIT(trait_name, func_name)                  \
  ARX_DEFINE_NAMED_EXTENDED_HAS_FUNC_TRAIT(trait_name, func_name, normal, void, return, ())

#define ARX_DEFINE_HAS_FUNC_TRAIT(func_name)                                    \
  ARX_DEFINE_NAMED_HAS_FUNC_TRAIT(BOOST_PP_CAT(has_, func_name), func_name)



// -------------------------------------------------------------------------- //
// ARX_HAS_EXACT_FUNC_XXX_TRAIT_DEF
// -------------------------------------------------------------------------- //
namespace arx {
  /* The following code is based on boost::introspection by Joel Falcou. */

  /**
   * This metafunction build an MPL sequence corresponding to the components 
   * of a member function type of class X with signature similar to Prototype.
   * 
   * E.g :
   * function_to_member<foo, void(int,long)>::type => <void,foo*,int,long>
   */
  template<class T, class Prototype> struct function_to_member {
    typedef typename boost::function_types::result_type<Prototype>::type     result;
    typedef typename boost::function_types::parameter_types<Prototype>::type args;
    typedef typename boost::mpl::push_front<args, T *>::type                 base;
    typedef typename boost::mpl::push_front<base, result>::type              type;
  };

  /**
   * This metafunction turns a class and a function type into a member function 
   * pointer with the same signature. 
   */
  template<class T, class Prototype> struct build_member_type {
    typedef typename function_to_member<T, Prototype>::type                   root;
    typedef typename boost::function_types::member_function_pointer<root>::type  type;
  };

  /**
   * This metafunction turns a class and a function type into a const member 
   * function pointer with the same signature.
   */
  template<class T, class Prototype> struct build_const_member_type {
    typedef typename function_to_member<T, Prototype>::type                   root;
    typedef typename boost::function_types::member_function_pointer<root, boost::function_types::const_qualified>::type type;
  };

} // namespace arx

#define ARX_HAS_EXACT_FUNC_TRAIT_PROCESSOR_normal(text) text
#define ARX_HAS_EXACT_FUNC_TRAIT_PROCESSOR_inline(text)
#define ARX_HAS_EXACT_FUNC_TRAIT_PROCESSOR(func_type)          \
  BOOST_PP_CAT(ARX_HAS_EXACT_FUNC_TRAIT_PROCESSOR_, func_type)

#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_III(trait_name, helper_name, func_name, func_sig, builder_name, has_member_name, has_member_struct_name, has_member_struct_processor_macro) \
struct helper_name {                                                            \
  typedef char true_type;                                                       \
  struct false_type { true_type dummy[2]; };                                    \
                                                                                \
  template<class T, typename builder_name<T, func_sig>::type> struct member {}; \
                                                                                \
};                                                                              \
                                                                                \
has_member_struct_processor_macro(struct has_member_struct_name {)                     \
  template<class T>                                                             \
  static helper_name::true_type has_member_name(T *, helper_name::member<T, &T::func_name> * = NULL); \
  static helper_name::false_type has_member_name(...);                          \
has_member_struct_processor_macro(};)                                           \
                                                                                \
template<class T, bool is_class = boost::is_class<T>::value>                    \
struct trait_name:                                                              \
  boost::mpl::bool_<                                                            \
    sizeof(has_member_struct_processor_macro(has_member_struct_name::) has_member_name(static_cast<T *>(NULL))) == \
    sizeof(helper_name::true_type)                                              \
  >                                                                             \
{};                                                                             \
                                                                                \
template<class T>                                                               \
struct trait_name<T, false>: public boost::mpl::false_ {};


#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_II(trait_name, func_name, func_sig, builder_name, checker_type) \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_III(                           \
    trait_name,                                                                 \
    BOOST_PP_CAT(trait_name, _helper),                                          \
    func_name,                                                                  \
    func_sig,                                                                   \
    builder_name,                                                               \
    BOOST_PP_CAT(trait_name, _has_member),                                      \
    BOOST_PP_CAT(trait_name, _has_member_struct),                               \
    ARX_HAS_EXACT_FUNC_TRAIT_PROCESSOR(checker_type)                 \
  )

#define ARX_DEFINE_NAMED_HAS_EXACT_NON_CONST_FUNC_TRAIT_I(trait_name, func_name, func_sig, checker_type) \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_II(trait_name, func_name, func_sig, arx::build_member_type, checker_type)

#define ARX_DEFINE_NAMED_HAS_EXACT_CONST_FUNC_TRAIT_I(trait_name, func_name, func_sig, checker_type) \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_II(trait_name, func_name, func_sig, arx::build_const_member_type, checker_type)

#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(trait_name, func_name, func_sig, checker_type) \
ARX_DEFINE_NAMED_HAS_EXACT_CONST_FUNC_TRAIT_I(BOOST_PP_CAT(trait_name, _const_impl), func_name, func_sig, checker_type) \
ARX_DEFINE_NAMED_HAS_EXACT_NON_CONST_FUNC_TRAIT_I(BOOST_PP_CAT(trait_name, _non_const_impl), func_name, func_sig, checker_type) \
                                                                                \
template<class T>                                                               \
struct trait_name :                                                             \
  boost::mpl::or_<                                                              \
    BOOST_PP_CAT(trait_name, _const_impl)<T>,                                   \
    BOOST_PP_CAT(trait_name, _non_const_impl)<T>                                \
  >                                                                             \
{};


/**
 * Generates a traits class that detect if a given type has a non-const member
 * function named func_name with a given signature func_sig.
 */
#define ARX_DEFINE_NAMED_HAS_EXACT_NON_CONST_FUNC_TRAIT(trait_name, func_name, func_sig) \
  ARX_DEFINE_NAMED_HAS_EXACT_NON_CONST_FUNC_TRAIT_I(trait_name, func_name, func_sig, normal)

#define ARX_DEFINE_INLINE_NAMED_HAS_EXACT_NON_CONST_FUNC_TRAIT(trait_name, func_name, func_sig) \
  ARX_DEFINE_NAMED_HAS_EXACT_NON_CONST_FUNC_TRAIT_I(trait_name, func_name, func_sig, inline)


/**
 * Generates a traits class that detect if a given type X has a const member
 * function named func_name with a given signature func_sig.
 */
#define ARX_DEFINE_NAMED_HAS_EXACT_CONST_FUNC_TRAIT(trait_name, func_name, func_sig) \
  ARX_DEFINE_NAMED_HAS_EXACT_CONST_FUNC_TRAIT_I(trait_name, func_name, func_sig, normal)

#define ARX_DEFINE_INLINE_NAMED_HAS_EXACT_CONST_FUNC_TRAIT(trait_name, func_name, func_sig) \
  ARX_DEFINE_NAMED_HAS_EXACT_CONST_FUNC_TRAIT_I(trait_name, func_name, func_sig, inline)


/**
 * Generates a traits class that detect if a given type X has a member function
 * named Name with a given signature Sig which is either const or non-const.
 *
 * Note that invocation of this trait will result in a compilation error if
 * the function being checked is private.
 */
#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT(trait_name, func_name, func_sig) \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(trait_name, func_name, func_sig, normal)

#define ARX_DEFINE_INLINE_NAMED_HAS_EXACT_FUNC_TRAIT(trait_name, func_name, func_sig) \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(trait_name, func_name, func_sig, inline)

#endif // ARX_HAS_XXX_H

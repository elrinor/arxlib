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
#include <boost/function_types/is_member_function_pointer.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/cat.hpp>


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
  static boost::type_traits::no_type has_member(T *, wrapper<void (mixin::*)(), &mixed<T>::type::func_name> * = NULL); \
  static boost::type_traits::yes_type has_member(...);                          \
};                                                                              \
                                                                                \
template<class T, bool is_class = boost::is_class<T>::value>                    \
struct trait_name:                                                              \
  boost::mpl::bool_<                                                            \
    sizeof(helper_name::has_member(static_cast<T *>(NULL))) ==                  \
    sizeof(boost::type_traits::yes_type)                                        \
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
namespace arx { namespace has_xxx_detail {
  template<class T> 
  T make();

  template<class Sig>
  struct parameter_types: 
    boost::function_types::parameter_types<Sig, boost::mpl::identity<boost::mpl::placeholders::_> >
  {};

  template<class Impl, class Sig, bool isStatic = !boost::function_types::is_member_function_pointer<Sig>::value>
  struct is_callable_base2:
    Impl::template apply<
      typename parameter_types<Sig>::type, 
      true, 
      boost::function_types::function_arity<Sig>::value
    >
  {};

  template<class Impl, class Sig>
  struct is_callable_base2<Impl, Sig, false>:
    Impl::template apply<
      typename boost::mpl::pop_front<
        typename parameter_types<Sig>::type
      >::type,
      false,
      boost::function_types::function_arity<Sig>::value - 1
    >
  {};


  template<class T, class Sig, bool isStatic = !boost::function_types::is_member_function_pointer<Sig>::value>
  struct object_type: 
    boost::mpl::identity<const T &> 
  {};

  template<class T, class Sig>
  struct object_type<T, Sig, false>:
    boost::mpl::identity<
    typename boost::mpl::front<
    typename parameter_types<Sig>::type
    >::type &
    >
  {};

  struct not_found {};

  struct void_return {
    template<class T>
    friend T operator,(T, void_return);
  };

  struct any {
    template<class T> any(const T &);
  };

  template<class Impl, class T, class Sig> 
  struct is_callable_base: is_callable_base2<Impl, Sig> {
    using is_callable_base2<Impl, Sig>::invoke;
    static not_found invoke(...);

    typedef typename boost::function_types::result_type<Sig>::type result_type;

    typedef typename boost::mpl::if_<
      boost::is_same<result_type, void>,
      any,
      result_type
    >::type test_result_type;

    static boost::type_traits::yes_type check(const test_result_type &);
    static boost::type_traits::no_type check(not_found);
    static boost::type_traits::no_type check(...);
  };

}} // namespace arx::has_xxx_detail

#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_III(Z, N, D)                      \
  arx::has_xxx_detail::make<typename boost::mpl::at_c<Params, N>::type>()

#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_II(Z, N, func_name)               \
  template<class Params>                                                        \
  struct apply<Params, false, N> {                                              \
    template<class U>                                                           \
    static decltype(arx::has_xxx_detail::make<U &>().func_name(BOOST_PP_ENUM(N, ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_III, ~))) invoke(U &); \
  };                                                                            \
                                                                                \
  template<class Params>                                                        \
  struct apply<Params, true, N> {                                               \
    template<class U>                                                           \
    static decltype(U::func_name(BOOST_PP_ENUM(N, ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_III, ~))) invoke(U &); \
  };


#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(trait_name, trait_impl_name, func_name) \
  struct trait_impl_name {                                                      \
    struct impl {                                                               \
      template<class Params, bool isStatic, int paramCount>                     \
      struct apply;                                                             \
                                                                                \
      BOOST_PP_REPEAT(ARX_HAS_XXX_MAX_ARITY, ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_II, func_name); \
    };                                                                          \
                                                                                \
    template<class T, class Sig>                                                \
    struct base: arx::has_xxx_detail::is_callable_base<impl, T, Sig> {          \
      enum {                                                                    \
        value = sizeof(                                                         \
          check((                                                               \
            invoke(arx::has_xxx_detail::make<typename arx::has_xxx_detail::object_type<T, Sig>::type>()), \
            arx::has_xxx_detail::void_return()                                  \
          ))                                                                    \
        ) == sizeof(boost::type_traits::yes_type)                               \
      };                                                                        \
    };                                                                          \
  };                                                                            \
                                                                                \
  template<class T, class Sig>                                                  \
  struct trait_name:                                                            \
    boost::mpl::bool_<trait_impl_name::base<T, Sig>::value>                     \
  {};

/**
 * Generates a traits class that detect if a given type X has a member function
 * named Name with a given signature.
 *
 * Note that invocation of this trait will result in a compilation error if
 * the function being checked is private.
 */
#define ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT(trait_name, func_name)            \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT_I(trait_name, BOOST_PP_CAT(trait_name, _impl), func_name)


#define ARX_DEFINE_HAS_EXACT_FUNC_TRAIT(func_name)                              \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT(BOOST_PP_CAT(has_exact_, func_name), func_name)


// -------------------------------------------------------------------------- //
// ARX_DEFINE_HAS_FUNC_TRAIT
// -------------------------------------------------------------------------- //
#define ARX_DEFINE_NAMED_HAS_FUNC_TRAIT_II(trait_name, any_trait_name, exact_trait_name, func_name) \
  ARX_DEFINE_NAMED_HAS_EXACT_FUNC_TRAIT(exact_trait_name, func_name)          \
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

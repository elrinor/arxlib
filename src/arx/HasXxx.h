#ifndef __ARX_HAS_XXX_H__
#define __ARX_HAS_XXX_H__

#include "config.h"
#include <boost/mpl/has_xxx.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_class.hpp>

// -------------------------------------------------------------------------- //
// ARX_HAS_TYPE_XXX_TRAIT_DEF
// -------------------------------------------------------------------------- //
#define ARX_HAS_TYPE_XXX_TRAIT_DEF(member_name)                                 \
  BOOST_MPL_HAS_XXX_TRAIT_DEF(member_name)

/** Generates a trait that returns whether the given type has a nested type with the given name. 
 *
 * @param trait_name                   Trait name.
 * @param member_name                  Name of a type to check. */
#define ARX_HAS_TYPE_XXX_TRAIT_NAMED_DEF(trait_name, member_name)               \
  BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait_name, member_name, false)


// -------------------------------------------------------------------------- //
// ARX_HAS_FUNC_XXX_TRAIT_DEF
// -------------------------------------------------------------------------- //
#define ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_DELC_PREFIX_normal() 
#define ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_DELC_PREFIX_static() static
#define ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_DELC_PREFIX(func_type)                  \
  BOOST_PP_CAT(ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_DELC_PREFIX_, func_type)

#define ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_POINTER_DECL_normal(type) type::*
#define ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_POINTER_DECL_static(type) *
#define ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_POINTER_DECL(func_type)                  \
  BOOST_PP_CAT(ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_POINTER_DECL_, func_type)

#define ARX_HAS_FUNC_XXX_TRAIT_NAMED_EXTENDED_DEF_I(trait_name, func_name, func_return_type, func_params, decl_prefix_macro, pointer_decl_macro) \
struct BOOST_PP_CAT(trait_name, _helper) {                                      \
  typedef char true_type;                                                       \
  struct false_type { true_type dummy[2]; };                                    \
  template<class T, T> struct wrapper {};                                       \
                                                                                \
  struct mixin {                                                                \
    decl_prefix_macro() func_return_type func_name func_params {}               \
  };                                                                            \
                                                                                \
  template<class T> struct mixed {                                              \
    struct type: public T, public mixin {};                                     \
  };                                                                            \
                                                                                \
  template<class T>                                                             \
  static false_type has_member(T* u, wrapper<func_return_type (pointer_decl_macro(mixin)) func_params, &mixed<T>::type::func_name>* = 0); \
  static true_type has_member(...);                                             \
};                                                                              \
                                                                                \
template<class T, bool is_class = boost::is_class<T>::value>                    \
struct trait_name: public                                                       \
  boost::mpl::bool_<                                                            \
    sizeof(BOOST_PP_CAT(trait_name, _helper)::has_member(static_cast<T*>(NULL))) == \
    sizeof(BOOST_PP_CAT(trait_name, _helper)::true_type)                        \
  > {};                                                                         \
                                                                                \
template<class T>                                                               \
struct trait_name<T, false>: public boost::mpl::false_ {};


/** Generates a trait that returns whether the given type has a member function with the given name. 
 * Note that it returns true even if that function is not accessible (i.e. private).
 *
 * @param trait_name                   Trait name.
 * @param func_name                    Name of a function to check.
 * @param func_type                    Type of a function, must be either static or normal.
 * @param func_return_type             Return type of a checking function. Is meaningful only if C++ constrains the possible return type of the function being cheched (as in case of operator new).
 * @param func_params                  Parameters of a checking function, in parentheses. */
#define ARX_HAS_FUNC_XXX_TRAIT_NAMED_EXTENDED_DEF(trait_name, func_name, func_type, func_return_type, func_params) \
  ARX_HAS_FUNC_XXX_TRAIT_NAMED_EXTENDED_DEF_I(                                  \
    trait_name,                                                                 \
    func_name,                                                                  \
    func_return_type,                                                           \
    func_params,                                                                \
    ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_DELC_PREFIX(func_type),                     \
    ARX_HAS_FUNC_XXX_TRAIT_FUNCTYPE_POINTER_DECL(func_type)                     \
  )

#define ARX_HAS_FUNC_XXX_TRAIT_NAMED_DEF(trait_name, func_name)                 \
  ARX_HAS_FUNC_XXX_TRAIT_NAMED_EXTENDED_DEF(trait_name, func_name, normal, void, ())

#define ARX_HAS_FUNC_XXX_TRAIT_DEF(member_name)                                 \
  ARX_HAS_FUNC_XXX_TRAIT_NAMED_DEF(BOOST_PP_CAT(has_, member_name), member_name)

#endif // __ARX_HAS_XXX_H__

#ifndef __ARX_HAS_XXX__
#define __ARX_HAS_XXX__

#include "config.h"
#include <boost/mpl/has_xxx.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_class.hpp>

#define ARX_HAS_TYPE_XXX_TRAIT_DEF(member_name)                                 \
  BOOST_MPL_HAS_XXX_TRAIT_DEF(member_name)

#define ARX_HAS_TYPE_XXX_TRAIT_NAMED_DEF(trait_name, member_name)               \
  BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(trait_name, member_name, false)

#define ARX_HAS_FUNC_XXX_TRAIT_NAMED_EXTENDED_DEF(trait_name, func_name, func_params) \
struct BOOST_PP_CAT(trait_name, _helper) {                                      \
  typedef char true_type;                                                       \
  struct false_type { true_type dummy[2]; };                                    \
  template<class T, T> struct wrapper {};                                       \
                                                                                \
  struct mixin {                                                                \
    void func_name func_params {}                                               \
  };                                                                            \
                                                                                \
  template<class T> struct mixed {                                              \
    struct type: public T, public mixin {};                                     \
  };                                                                            \
                                                                                \
  template<class T>                                                             \
  static false_type has_member(T* u, wrapper<void (mixin::*)(), &mixed<T>::type::func_name>* = 0); \
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

#define ARX_HAS_FUNC_XXX_TRAIT_NAMED_DEF(trait_name, func_name)                 \
  ARX_HAS_FUNC_XXX_TRAIT_NAMED_EXTENDED_DEF(trait_name, func_name, ())

#define ARX_HAS_FUNC_XXX_TRAIT_DEF(member_name)                                 \
  ARX_HAS_FUNC_XXX_TRAIT_NAMED_DEF(BOOST_PP_CAT(has_, member_name), member_name)

#endif // __ARX_HAS_XXX__

#ifndef __ARX_HASMEMBERXXX__
#define __ARX_HASMEMBERXXX__

#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/bool.hpp>

#define ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(trait_name, access, type)             \
struct BOOST_PP_CAT(trait_name, _helper) {                                      \
  typedef char true_type;                                                       \
  struct false_type { true_type dummy[2]; };                                    \
  template<class T, type> struct nothing {};                                    \
                                                                                \
  template<class T>                                                             \
  static true_type has_member(T* u, nothing<T, access>* = NULL);                \
  static false_type has_member(...);                                            \
};                                                                              \
                                                                                \
template<class T> struct trait_name: public                                     \
  boost::mpl::bool_<                                                            \
    sizeof(BOOST_PP_CAT(trait_name, _helper)::has_member(static_cast<T*>(NULL))) == \
    sizeof(BOOST_PP_CAT(trait_name, _helper)::true_type)                        \
  > {};

#define ARX_HAS_TYPED_MEMBER_XXX_TRAIT_DEF(member_name, member_type)            \
  ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(BOOST_PP_CAT(has_, member_name), &T::member_name, member_type)

#endif // __ARX_HASMEMBERXXX__

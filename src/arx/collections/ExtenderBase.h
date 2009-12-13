#ifndef __ARX_COLLECTIONS_EXTENDER_BASE_H__
#define __ARX_COLLECTIONS_EXTENDER_BASE_H__

#include "config.h"
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>
#include "Forward.h"
#include "../StaticCounter.h"
#include "../Preprocessor.h"
#include "../HasXxx.h"

namespace arx {
  namespace detail {
    template<int n>
    struct RegisteredExtender;

    ARX_NEW_COUNTER(extender_count);

    ARX_HAS_TYPE_XXX_TRAIT_DEF(mapped_type);
    ARX_HAS_TYPE_XXX_TRAIT_DEF(key_type);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(find);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(insert);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(push_front);
    ARX_HAS_FUNC_XXX_TRAIT_DEF(push_back);

  } // namespace detail


// -------------------------------------------------------------------------- //
// ExtenderBase
// -------------------------------------------------------------------------- //
  template<class Base, class Derived>
  class ExtenderBase: public Base {
  protected:
    typedef Derived derived_type;

  public:
    ARX_FORWARD_CONSTRUCTOR(ExtenderBase, (ARX_FWD_TEXT(Base))(ARX_FWD_ARGS())(ARX_FWD_TEXT({})));
  };

} // namespace arx


// -------------------------------------------------------------------------- //
// Registration
// -------------------------------------------------------------------------- //
#define ARX_REGISTER_EXTENDER(name, ... /* requirements */)                     \
namespace arx {                                                                 \
  namespace detail {                                                            \
    template<>                                                                  \
    struct RegisteredExtender<ARX_GET_COUNTER(extender_count)> {                \
      template<class Base>                                                      \
      struct apply {                                                            \
        typedef name<Base> type;                                                \
      };                                                                        \
                                                                                \
      template<class T>                                                         \
      struct applicable_to {                                                    \
        typedef __VA_ARGS__ type;                                               \
      };                                                                        \
    };                                                                          \
                                                                                \
    ARX_INC_COUNTER(extender_count);                                            \
  }                                                                             \
}


#endif // __ARX_COLLECTIONS_EXTENDER_BASE_H__

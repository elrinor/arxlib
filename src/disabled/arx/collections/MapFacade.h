#ifndef __ARX_COLLECTIONS_MAP_FACADE_H__
#define __ARX_COLLECTIONS_MAP_FACADE_H__

#include "config.h"
#include <exception> /* for std::out_of_range */
#include <typeinfo>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/range.hpp>
#include <boost/iterator.hpp>
#include "FacadeBase.h"
#include "AssociativeFacade.h" /* This one must be registered beforehand. */
#include "UnorderedFacade.h" /* And this one too. */

namespace arx {
// -------------------------------------------------------------------------- //
// MapFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class MapFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(container_type, (mapped_type));
    ARX_INJECT_TYPES_TPL(Base, (key_type));

    mapped_type& operator[] (const key_type& key) {
      return this->container().operator[] (key);
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(MapFacade, boost::mpl::and_<has_mapped_type<T>, has_key_type<T> >)

} // namespace arx

#endif // __ARX_COLLECTIONS_MAP_FACADE_H__

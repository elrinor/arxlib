#ifndef __ARX_VIEWS_H__
#define __ARX_VIEWS_H__

#include "LambdaHack.h"
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/typeof/typeof.hpp>

namespace arx {
  namespace detail {
    template<class tag> class view_traits;
  }

// -------------------------------------------------------------------------- //
// transform
// -------------------------------------------------------------------------- //
  namespace detail {
    template<class Container, class Transformation>
    class transform;

    template<class Container, class Transformation>
    struct view_traits<transform<Container, Transformation> > {
      typedef typename std::tr1::result_of<Transformation(typename boost::range_reference<Container>::type)>::type reference;

      typedef boost::transform_iterator<
        Transformation, 
        typename boost::range_iterator<Container>::type, 
        reference
      > iterator;

      typedef boost::iterator_range<iterator> result_type;
    };
  }

  template<class Container, class Transformation>
  typename detail::view_traits<detail::transform<Container, Transformation> >::result_type
  transform(Container& container, Transformation transformation) {
    typedef detail::view_traits<detail::transform<Container, Transformation> > traits;
    return traits::result_type(
      traits::iterator(boost::begin(container), transformation),
      traits::iterator(boost::end(container), transformation)
    );
  }

  template<class Container, class Transformation>
  struct Transform {
    typedef typename detail::view_traits<detail::transform<Container, Transformation> >::result_type type;
  };


// -------------------------------------------------------------------------- //
// values
// -------------------------------------------------------------------------- //
  namespace detail {
    template<class Map>
    class values;

    template<class Map>
    struct view_traits<values<Map> > {
      typedef BOOST_TYPEOF_TPL(bind(&Map::value_type::second, boost::lambda::_1)) bind_type;
      typedef typename view_traits<transform<Map, bind_type> >::result_type result_type;
    };
  }

  template<class Map>
  typename detail::view_traits<detail::values<Map> >::result_type 
  values(Map& map) {
    return transform(map, boost::lambda::bind(&Map::value_type::second, boost::lambda::_1));
  }

  template<class Map>
  struct Values {
    typedef typename detail::view_traits<detail::values<Map> >::result_type type;
  };


// -------------------------------------------------------------------------- //
// keys
// -------------------------------------------------------------------------- //
  namespace detail {
    template<class Map>
    class keys;

    template<class Map>
    struct view_traits<keys<Map> > {
      typedef BOOST_TYPEOF_TPL(bind(&Map::value_type::first, boost::lambda::_1)) bind_type;
      typedef typename view_traits<transform<Map, bind_type> >::result_type result_type;
    };
  }

  template<class Map>
  typename detail::view_traits<detail::keys<Map> >::result_type 
  keys(Map& map) {
    return transform(map, boost::lambda::bind(&Map::value_type::first, boost::lambda::_1));
  }

  template<class Map>
  struct Keys {
    typedef typename detail::view_traits<detail::keys<Map> >::result_type type;
  };



} // namespace arx

#endif // __ARX_VIEWS_H__

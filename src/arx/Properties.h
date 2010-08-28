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
#ifndef ARX_PROPERTIES_H
#define ARX_PROPERTIES_H

#include "config.h"
#include <boost/mpl/identity.hpp>
#include <boost/proto/proto.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/include/cons.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/utility/result_of.hpp>

namespace arx {

  namespace detail {
    namespace proto = boost::proto;
    namespace fusion = boost::fusion;
    namespace mpl = boost::mpl;


    /**
     * Wrapper class for property keys.
     */
    template<class KeyTag>
    struct property_key_wrapper {};


    /**
     * Metafunction that returns property key tag given a property key.
     */
    template<class Key> 
    struct unwrap_property_key;

    template<class KeyTag> 
    struct unwrap_property_key<property_key_wrapper<KeyTag> >: 
      mpl::identity<KeyTag> 
    {};


    /**
     * Proto transform (and proto grammar) that builds a fusion cons of 
     * fusion pairs from a property expression.
     */
    struct property_to_cons_transform:
      proto::or_<
        proto::when<
          proto::comma<property_to_cons_transform, property_to_cons_transform>,
          proto::fold<proto::_, proto::_state, property_to_cons_transform>
        >,
        proto::when<
          proto::assign<
            proto::terminal<property_key_wrapper<proto::_> >,
            proto::_
          >,
          fusion::cons<
            fusion::pair<
              unwrap_property_key<proto::_value(proto::_left)>, 
              proto::_value(proto::_right)
            >, 
            proto::_state
          >(
            proto::make<
              fusion::pair<
                unwrap_property_key<proto::_value(proto::_left)>, 
                proto::_value(proto::_right)
              >(proto::_value(proto::_right))
            >,
            proto::_state
          )
        >
      >
    {};


    typedef property_to_cons_transform property_grammar;


    template<class Expr>
    struct property_expression;


    /**
     * Proto domain for property expressions.
     */
    struct property_domain: 
      proto::domain<proto::pod_generator<property_expression>, property_grammar> 
    {};


    /**
     * Expression wrapper for property expressions.
     */
    template<class Expr>
    struct property_expression
    {
      typedef property_expression<Expr> this_type;

      BOOST_PROTO_EXTENDS(Expr, this_type, property_domain);

      template<bool is_property = proto::matches<Expr, property_grammar>::value>
      struct get_impl {
        typedef typename boost::result_of<
          property_to_cons_transform(this_type, fusion::nil)
        >::type seq;

        template<class KeyTag>
        struct pred {
          template<class Pair>
          struct apply: 
            boost::is_same<KeyTag, typename fusion::result_of::first<Pair>::type>
          {};
        };

        template<class KeyTag>
        struct key_pos: 
          fusion::result_of::find_if<
            seq, 
            pred<KeyTag>
          >
        {};

        template<class KeyTag>
        struct has_key: 
          boost::mpl::not_<
            boost::is_same<
              typename key_pos<KeyTag>::type, 
              typename fusion::result_of::end<seq>::type
            >
          > 
        {};

        template<class KeyTag, class DefaultValue, bool contains = has_key<KeyTag>::value>
        struct getter {
          typedef 
            const typename fusion::result_of::second<
              typename boost::remove_reference<
                typename fusion::result_of::deref<typename key_pos<KeyTag>::type>::type
              >::type
            >::type &
          result_type;

          result_type operator() (const this_type &expr, const DefaultValue &) const {
            return (*fusion::find_if<pred<KeyTag> >(property_to_cons_transform()(expr, fusion::nil()))).second;
          }
        };

        template<class KeyTag, class DefaultValue>
        struct getter<KeyTag, DefaultValue, false> {
          typedef DefaultValue result_type;

          result_type operator() (const this_type &, const DefaultValue &defaultValue) const {
            return defaultValue;
          }
        };

      };

      template<>
      struct get_impl<false> {
        template<class KeyTag, class DefaultValue>
        struct getter {
          typedef void result_type;
        };
      };

      /**
       * @tparam KeyTag                Key tag to get property value for.
       * @param defaultValue           Default value that is returned in case
       *                               this property expression contains no
       *                               value for the given tag.
       */
      template<class KeyTag, class DefaultValue>
      typename get_impl<>::template getter<KeyTag, DefaultValue>::result_type 
      get(const DefaultValue &defaultValue) const {
        return get_impl<>::template getter<KeyTag, DefaultValue>()(*this, defaultValue);
      }

    };

  } // namespace detail


/**
 * Macro that defines a variable that serves as a terminal in property 
 * expressions.
 *
 * Value of this variable can be extracted from property expression using 
 * the given key tag.
 *
 * @param key_tag                      Tag of the key.
 * @param key_name                     Name of the key variable.
 */
#define ARX_DEFINE_PROPERTY_KEY(key_tag, key_name)                              \
  namespace {                                                                   \
    arx::xml::detail::property_expression<                                      \
      boost::proto::terminal<                                                   \
        arx::xml::detail::property_key_wrapper<key_tag>                         \
      >::type                                                                   \
    > key_name = {{{}}};                                                        \
  }

} // namespace arx

#endif // ARX_PROPERTIES_H

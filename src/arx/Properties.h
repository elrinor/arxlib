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
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>
#include <boost/proto/proto.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/include/cons.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <arx/TypeTraits.h>

namespace arx {

  namespace properties_detail {
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
     * Property key tag for empty property map.
     */
    struct no_properties_tag {};


    /**
     * Proto transform (and proto grammar) that builds a fusion cons of 
     * fusion pairs from a property expression.
     */
    struct property_to_cons_transform:
      proto::or_<
        proto::when<
          proto::terminal<property_key_wrapper<no_properties_tag> >,
          proto::_state
        >,
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


    /**
     * Proto transform that returns a key tag given a key terminal.
     */
    struct property_get_key_tag_transform:
      proto::when<
        proto::terminal<property_key_wrapper<proto::_> >,
        unwrap_property_key<proto::_value>()
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

      template<class KeyTag, class DefaultValue>
      struct get_by_tag_impl {
        /**
         * Fusion sequence corresponding to the current expression.
         */
        typedef 
          typename boost::result_of<
            property_to_cons_transform(this_type, fusion::nil)
          >::type 
        seq;

        /**
         * Predicate for searching the given key tag in the sequence.
         */
        struct pred {
          template<class Pair>
          struct apply: 
            boost::is_same<KeyTag, typename fusion::result_of::first<Pair>::type>
          {};
        };

        /**
         * Metafuction that returns a fusion iterator which determines 
         * position of the given key in the sequence.
         */
        struct key_pos: 
          fusion::result_of::find_if<seq, pred>
        {};

        /**
         * Metafunction that returns true if the sequence contains the 
         * given key.
         */
        struct has_key: 
          boost::mpl::not_<
            boost::is_same<
              typename key_pos::type, 
              typename fusion::result_of::end<seq>::type
            >
          > 
        {};

        /**
         * Implementation of the getter for the case when the sequence contains
         * the given key.
         */
        template<bool contains = has_key::value>
        struct getter_impl {
          typedef 
            const typename fusion::result_of::second<
              typename boost::remove_reference<
                typename fusion::result_of::deref<typename key_pos::type>::type
              >::type
            >::type &
          result_type;

          result_type operator()(const this_type &expr, const DefaultValue &) const {
            return (*fusion::find_if<pred>(property_to_cons_transform()(expr, fusion::nil()))).second;
          }
        };

        /**
         * Implementation of the getter for the case when the sequence does
         * not contain the given key.
         */
        template<>
        struct getter_impl<false> {
          typedef DefaultValue result_type;

          result_type operator()(const this_type &, const DefaultValue &defaultValue) const {
            return defaultValue;
          }
        };

        /**
         * Getter implementation.
         */
        struct getter: getter_impl<> {};

        /**
         * Getter return type.
         */
        typedef typename getter::result_type type;
      };

      /**
       * Metafuction that returns true if get operation is applicable to this
       * expression.
       */
      struct is_gettable: 
        proto::matches<Expr, property_grammar> 
      {};

      /**
       * Metafunction that returns result type of get-by-tag operation for the
       * given parameter types.
       */
      template<class KeyTag, class DefaultValue>
      struct get_by_tag_result {
        typedef 
          typename mpl::if_<
            is_gettable,
            get_by_tag_impl<KeyTag, DefaultValue>,
            mpl::identity<void>
          >::type::type
        type;
      };

      /**
       * Metafuction that returns true if the given type is a valid key type
       * (e.g. generated via ARX_DEFINE_PROPERTY_KEY).
       */
      template<class Key>
      struct is_key:
        mpl::and_<
          proto::is_expr<Key>, 
          proto::matches<Key, property_get_key_tag_transform> 
        >
      {};

      struct not_a_key {};

      /**
       * Metafuction that returns key tag for the given key type, or not_a_key
       * in case the given type is not a key type.
       */
      template<class Key>
      struct key_tag {
        typedef 
          typename mpl::if_<
            is_key<Key>,
            boost::result_of<property_get_key_tag_transform(Key)>,
            mpl::identity<not_a_key>
          >::type::type 
        type;
      };

      /**
       * Metafuction that returns result of get-by-key operation for the given
       * parameter types.
       */
      template<class Key, class DefaultValue>
      struct get_by_key_result {
        typedef 
          typename get_by_tag_result<typename key_tag<Key>::type, DefaultValue>::type 
        type;
      };

      /**
       * @tparam KeyTag                Key tag to get property value for.
       * @param defaultValue           Default value that is returned in case
       *                               this property expression contains no
       *                               value for the given tag.
       */
      template<class KeyTag, class DefaultValue>
      typename boost::enable_if<
        is_gettable,
        get_by_tag_result<KeyTag, DefaultValue>
      >::type::type
      get(const DefaultValue &defaultValue) const {
        return get_by_tag_impl<KeyTag, DefaultValue>::getter()(*this, defaultValue);
      }

      template<class Key, class DefaultValue>
      typename boost::enable_if<
        mpl::and_<is_gettable, is_key<Key> >, 
        get_by_key_result<Key, DefaultValue>
      >::type::type
      get(const Key &, const DefaultValue &defaultValue) const {
        return get<typename key_tag<Key>::type>(defaultValue);
      }

    };


    template<class T>
    struct is_property_expression_impl: 
      mpl::false_ 
    {};

    template<class Expr>
    struct is_property_expression_impl<property_expression<Expr> >: 
      mpl::true_ 
    {};


    /**
     * Metafunction that returns true for property expressions.
     */
    template<class T>
    struct is_property_expression: 
      is_property_expression_impl<typename remove_cv_reference<T>::type>
    {};

  } // namespace properties_detail


/**
 * Macro that defines a variable that serves as a terminal in property 
 * expressions.
 *
 * Value of this variable can be extracted from property expression using 
 * the given key tag.
 *
 * @param KEY_TAG                      Tag of the key.
 * @param KEY_NAME                     Name of the key variable.
 */
#define ARX_DEFINE_PROPERTY_KEY(KEY_TAG, KEY_NAME)                              \
  namespace {                                                                   \
    arx::properties_detail::property_expression<                                \
      boost::proto::terminal<                                                   \
        arx::properties_detail::property_key_wrapper<KEY_TAG>                   \
      >::type                                                                   \
    > KEY_NAME = {{{}}};                                                        \
  }

  ARX_DEFINE_PROPERTY_KEY(properties_detail::no_properties_tag, no_properties);


  using properties_detail::is_property_expression;

} // namespace arx

#endif // ARX_PROPERTIES_H

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
#ifndef ARX_XML_BINDING_H
#define ARX_XML_BINDING_H

#include "config.h"
#include <cassert>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/proto/proto.hpp>
#include <arx/Preprocessor.h>
#include <arx/Properties.h>
#include <arx/TypeTraits.h>
#include "Path.h"
#include "Error.h"
#include "ErrorLocation.h"
#include "UserData.h"
#include "NodeInspector.h"
#include "Checkers.h"

/* TODO: Inspector, Walker and Processor as parameters to (de)serialization
 * functions, stored in ctx. */

/* TODO: registration for Type & NodeType & Tag (optional) (not only for Type). 
 * Tag is there to allow multiple (de)serialization routines for the same type.
 * Tag is given as a parameter to (de)serialization routines and to 
 * ARX_XML_MEMBER. */

/* TODO: pass actual value to error reporting routine. */

namespace arx { namespace xml {
  namespace binding_detail {
    namespace proto = boost::proto;
    namespace mpl = boost::mpl;

    /**
     * Metafunction that return type of the data member given a data member 
     * pointer type.
     */
    template<class T>
    struct member_type;

    template<class Class, class MemberPointer>
    struct member_type<MemberPointer Class::*>: 
      mpl::identity<MemberPointer>
    {};


    /**
     * Metafunction that returns return type of the given getter member 
     * function pointer type.
     */
    template<class T>
    struct getter_result;

    template<class Class, class Result>
    struct getter_result<Result (Class::*)() const>:
      remove_cv_reference<Result>
    {};


    /**
     * Metafunction that returns parameter type of the given setter member 
     * function pointer type.
     */
    template<class T>
    struct setter_param;

    template<class Class, class Result, class Param>
    struct setter_param<Result (Class::*)(Param)>:
      remove_cv_reference<Param>
    {};


    /**
     * Wrapper for a message handler that checks whether there were any 
     * fatal errors and translates error messages.
     */
    template<class MessageHandler, class Params>
    struct MessageTranslator {
    public:
      explicit MessageTranslator(MessageHandler &handler, const Params &params, bool &success):
        handler(handler), params(params), success(success) {}

      template<class ErrorData>
      void operator()(ErrorSeverity severity, const ErrorData& errorData, const ErrorLocation &location) {
        assert(severity == ERROR || severity == WARNING);

        if(severity == ERROR)
          success = false;

        handler(severity, errorData, params.get<user_data_tag>(no_user_data()), location);
      }

      template<class ErrorData, class Node>
      void operator()(ErrorSeverity severity, const ErrorData& errorData, const Node &node) {
        operator()(severity, errorData, node_inspector<Node>::type().location(node));
      }

      MessageHandler &handler;
      const Params &params;
      bool &success;
    };

    template<class MessageHandler, class Params>
    MessageTranslator<MessageHandler, Params> create_translator(MessageHandler &handler, const Params &params, bool &success) {
      return MessageTranslator<MessageHandler, Params>(handler, params, success);
    }


    /**
     * Delegation wrapper.
     */
    template<class T>
    struct delegate_wrapper {};

    template<class T>
    delegate_wrapper<T> delegate(const T & = T()) {
      return delegate_wrapper<T>();
    }


    /**
     * Wrapper for xml bindings.
     */
    template<class T>
    struct binding_wrapper {
      T value;
    };


    /**
     * Proto grammar for xml bindings.
     */
    struct binding_expression_grammar:
      proto::or_<
        proto::comma<binding_expression_grammar, binding_expression_grammar>,
        proto::terminal<binding_wrapper<proto::_> >,
        proto::if_else_<proto::_, binding_expression_grammar, binding_expression_grammar>
      >
    {};


    template<class Expr>
    struct binding_expression;


    /**
     * Proto generator for xml binding expressions.
     */
    typedef 
      proto::pod_generator<binding_expression>
    binding_generator;


    /**
     * Proto domain for xml binding expressions.
     */
    struct binding_domain: 
      proto::domain<binding_generator, binding_expression_grammar> 
    {};


    /**
     * Base class for all xml bindings.
     */
    template<class Path, class Params>
    struct binding_base {
      binding_base(const Path &path, const Params &params):
        path(proto::deep_copy(path)), params(proto::deep_copy(params)) {}

      typename proto::result_of::deep_copy<Path>::type path;
      typename proto::result_of::deep_copy<Params>::type params;
    };


    /**
     * No-op binding.
     */
    struct noop_binding {};


    /**
     * Shortcut to empty parameters expression.
     */
    typedef decltype(no_properties) NoParams;


    /**
     * Functional binding.
     */
    template<class Path, class Serializer, class Deserializer, class Params>
    struct functional_binding: binding_base<Path, Params> {
      functional_binding(const Path &path, const Serializer &serializer, const Deserializer &deserializer, const Params &params):
        binding_base<Path, Params>(path, params), serializer(serializer), deserializer(deserializer) {}

      Serializer serializer;
      Deserializer deserializer;

      typedef 
        binding_expression<typename proto::terminal<binding_wrapper<functional_binding> >::type>
      expr_type;
    };

    template<class Path, class Serializer, class Deserializer, class Params>
    typename functional_binding<Path, Serializer, Deserializer, Params>::expr_type
    functional(const Path &path, const Serializer &serializer, const Deserializer &deserializer, const Params &params) {
      typedef functional_binding<Path, Serializer, Deserializer, Params> binding_type;
      typename binding_type::expr_type result = {{{binding_type(path, serializer, deserializer, params)}}};
      return result;
    }

    template<class Path, class Serializer, class Deserializer>
    auto functional(const Path &path, const Serializer &serializer, const Deserializer &deserializer) -> 
      decltype(functional(path, serializer, deserializer, no_properties)) 
    {
      return functional(path, serializer, deserializer, no_properties);
    }


    /**
     * Member binding
     */
    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Checker, class Params>
    struct member_binding: binding_base<Path, Params> {
      member_binding(const Path &path, const Checker &checker, const Params &params):
        binding_base<Path, Params>(path, params), checker(checker) {}

      typedef 
        binding_expression<typename proto::terminal<binding_wrapper<member_binding> >::type>
      expr_type;

      Checker checker;
    };

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Checker, class Params, class Enable = void>
    struct member_result {};

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Checker, class Params>
    struct member_result<
      MemberPointer, pointer, Delegate, Path, Checker, Params,
      typename boost::enable_if<
        mpl::and_<
          is_path_expression<Path>,
          is_property_expression<Params>,
          mpl::not_<is_property_expression<Checker> >
        >
      >::type
    >:
      mpl::identity<
        typename member_binding<MemberPointer, pointer, Delegate, Path, Checker, Params>::expr_type
      >
    {};

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Checker, class Params>
    typename member_result<MemberPointer, pointer, Delegate, Path, Checker, Params>::type
    member_impl(const Path &path, const Checker &checker, const Params &params) {
      typedef member_binding<MemberPointer, pointer, Delegate, Path, Checker, Params> binding_type;
      typename binding_type::expr_type result = {{{binding_type(path, checker, params)}}};
      return result;
    }

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Checker, class Params>
    typename member_result<MemberPointer, pointer, Delegate, Path, Checker, Params>::type
    member(const Path &path, const Checker &checker, const Params &params, const delegate_wrapper<Delegate> & = delegate_wrapper<Delegate>()) {
      return member_impl<MemberPointer, pointer, Delegate>(path, checker, params);
    }

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Checker>
    typename member_result<MemberPointer, pointer, Delegate, Path, Checker, NoParams>::type
    member(const Path &path, const Checker &checker, const delegate_wrapper<Delegate> & = delegate_wrapper<Delegate>()) {
      return member_impl<MemberPointer, pointer, Delegate>(path, checker, no_properties);
    }

    template<class MemberPointer, MemberPointer pointer, class Path, class Checker, class Params>
    typename member_result<MemberPointer, pointer, typename member_type<MemberPointer>::type, Path, Checker, Params>::type
    member(const Path &path, const Checker &checker, const Params &params) {
      return member_impl<MemberPointer, pointer, typename member_type<MemberPointer>::type>(path, checker, params);
    }

    template<class MemberPointer, MemberPointer pointer, class Path, class Checker>
    typename member_result<MemberPointer, pointer, typename member_type<MemberPointer>::type, Path, Checker, NoParams>::type
    member(const Path &path, const Checker &checker) {
      return member_impl<MemberPointer, pointer, typename member_type<MemberPointer>::type>(path, checker, no_properties);
    }

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Params>
    typename member_result<MemberPointer, pointer, Delegate, Path, NullChecker, Params>::type
    member(const Path &path, const Params &params, const delegate_wrapper<Delegate> & = delegate_wrapper<Delegate>()) {
      return member_impl<MemberPointer, pointer, Delegate>(path, NullChecker(), params);
    }

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path>
    typename member_result<MemberPointer, pointer, Delegate, Path, NullChecker, NoParams>::type
    member(const Path &path, const delegate_wrapper<Delegate> & = delegate_wrapper<Delegate>()) {
      return member_impl<MemberPointer, pointer, Delegate>(path, NullChecker(), no_properties);
    }

    template<class MemberPointer, MemberPointer pointer, class Path, class Params>
    typename member_result<MemberPointer, pointer, typename member_type<MemberPointer>::type, Path, NullChecker, Params>::type
    member(const Path &path, const Params &params) {
      return member_impl<MemberPointer, pointer, typename member_type<MemberPointer>::type>(path, NullChecker(), params);
    }

    template<class MemberPointer, MemberPointer pointer, class Path>
    typename member_result<MemberPointer, pointer, typename member_type<MemberPointer>::type, Path, NullChecker, NoParams>::type
    member(const Path &path) {
      return member_impl<MemberPointer, pointer, typename member_type<MemberPointer>::type>(path, NullChecker(), no_properties);
    }


    /**
     * Accessor binding.
     */
    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Checker, class Params>
    struct accessor_binding: binding_base<Path, Params> {
    accessor_binding(const Path &path, const Checker &checker, const Params &params):
      binding_base<Path, Params>(path, params), checker(checker) {}

      typedef 
        binding_expression<typename proto::terminal<binding_wrapper<accessor_binding> >::type>
      expr_type;

      Checker checker;
    };

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Checker, class Params, class Enable = void>
    struct accessor_result {};

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Checker, class Params>
    struct accessor_result<
      Getter, getter, Setter, setter, Delegate, Path, Checker, Params,
      typename boost::enable_if<
        mpl::and_<
          is_path_expression<Path>,
          is_property_expression<Params>,
          mpl::not_<is_property_expression<Checker> >
        >
      >::type
    >:
      mpl::identity<
        typename accessor_binding<Getter, getter, Setter, setter, Delegate, Path, Checker, Params>::expr_type
      >
    {};

    template<class Getter, class Setter>
    struct accessors_valid: 
      boost::is_same<
        typename getter_result<Getter>::type,
        typename setter_param<Setter>::type
      >
    {};

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Checker, class Params>
    typename accessor_result<Getter, getter, Setter, setter, Delegate, Path, Checker, Params>::type
      accessor_impl(const Path &path, const Checker &checker, const Params &params) {
      typedef accessor_binding<Getter, getter, Setter, setter, Delegate, Path, Checker, Params> binding_type;
      typename binding_type::expr_type result = {{{binding_type(path, checker, params)}}};
      return result;
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Checker, class Params>
    typename accessor_result<Getter, getter, Setter, setter, Delegate, Path, Checker, Params>::type
    accessor(const Path &path, const Checker &checker, const Params &params, const delegate_wrapper<Delegate> & = delegate_wrapper<Delegate>()) {
      return accessor_impl<Getter, getter, Setter, setter, Delegate>(path, checker, params);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Checker>
    typename accessor_result<Getter, getter, Setter, setter, Delegate, Path, Checker, NoParams>::type
    accessor(const Path &path, const Checker &checker, const delegate_wrapper<Delegate> & = delegate_wrapper<Delegate>()) {
      return accessor_impl<Getter, getter, Setter, setter, Delegate>(path, checker, no_properties);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Path, class Checker, class Params>
    typename accessor_result<Getter, getter, Setter, setter, typename getter_result<Getter>::type, Path, Checker, Params>::type
    accessor(const Path &path, const Checker &checker, const Params &params, const typename boost::enable_if<accessors_valid<Getter, Setter> >::type * = NULL) {
      return accessor_impl<Getter, getter, Setter, setter, typename getter_result<Getter>::type>(path, checker, params);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Path, class Checker>
    typename accessor_result<Getter, getter, Setter, setter, typename getter_result<Getter>::type, Path, Checker, NoParams>::type
    accessor(const Path &path, const Checker &checker, const typename boost::enable_if<accessors_valid<Getter, Setter> >::type * = NULL) {
      return accessor_impl<Getter, getter, Setter, setter, typename getter_result<Getter>::type>(path, checker, no_properties);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Params>
    typename accessor_result<Getter, getter, Setter, setter, Delegate, Path, NullChecker, Params>::type
    accessor(const Path &path, const Params &params, const delegate_wrapper<Delegate> & = delegate_wrapper<Delegate>()) {
      return accessor_impl<Getter, getter, Setter, setter, Delegate>(path, NullChecker(), params);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path>
    typename accessor_result<Getter, getter, Setter, setter, Delegate, Path, NullChecker, NoParams>::type
    accessor(const Path &path, const delegate_wrapper<Delegate> & = delegate_wrapper<Delegate>()) {
      return accessor_impl<Getter, getter, Setter, setter, Delegate>(path, NullChecker(), no_properties);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Path, class Params>
    typename accessor_result<Getter, getter, Setter, setter, typename getter_result<Getter>::type, Path, NullChecker, Params>::type
    accessor(const Path &path, const Params &params, const typename boost::enable_if<accessors_valid<Getter, Setter> >::type * = NULL) {
      return accessor_impl<Getter, getter, Setter, setter, typename getter_result<Getter>::type>(path, NullChecker(), params);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Path>
    typename accessor_result<Getter, getter, Setter, setter, typename getter_result<Getter>::type, Path, NullChecker, NoParams>::type
    accessor(const Path &path, const typename boost::enable_if<accessors_valid<Getter, Setter> >::type * = NULL) {
      return accessor_impl<Getter, getter, Setter, setter, typename getter_result<Getter>::type>(path, NullChecker(), no_properties);
    }


    /**
     * Fixup binding.
     */
    template<class Fixer>
    struct fixup_binding {
      fixup_binding(const Fixer &fixer):
        fixer(fixer) {}

      Fixer fixer;

      typedef 
        binding_expression<typename proto::terminal<binding_wrapper<fixup_binding> >::type>
      expr_type;
    };

    template<class Fixer>
    typename fixup_binding<Fixer>::expr_type
    fixup(const Fixer &fixer) {
      typedef fixup_binding<Fixer> binding_type;
      typename binding_type::expr_type result = {{{binding_type(fixer)}}};
      return result;
    }


    /**
     * Serialization context.
     */
    template<class T, class MessageHandler, class Params, class Node>
    struct serialization_context: 
      proto::callable_context<const serialization_context<T, MessageHandler, Params, Node> >
    {
      serialization_context(const T &source, MessageHandler &handler, const Params &params, Node *target): 
        source(source), handler(handler), params(params), target(target), success(true) {}

      typedef void result_type;

      void operator()(proto::tag::terminal, const binding_wrapper<noop_binding> &) const {
        return;
      }

      template<class L, class R>
      void operator()(proto::tag::comma, const L &l, const R &r) const {
        proto::eval(l, *this);
        proto::eval(r, *this);
      }

      template<class Path, class Serializer, class Deserializer, class AdditionalParams>
      void operator()(proto::tag::terminal, const binding_wrapper<functional_binding<Path, Serializer, Deserializer, AdditionalParams> > &binding) const {
        auto child = binding.value.path.create(*target);
        auto newParams = (binding.value.params, params);
        auto translator = create_translator(handler, newParams, success);
        binding.value.serializer(source, translator, newParams, &child);

        /* Serialization cannot fail. If it fails then it's a design flaw. */
        assert(success);
      }

      template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Checker, class AdditionalParams>
      void operator()(proto::tag::terminal, const binding_wrapper<member_binding<MemberPointer, pointer, Delegate, Path, Checker, AdditionalParams> > &binding) const {
        auto child = binding.value.path.create(*target);
        Delegate tmp = static_cast<Delegate>(source.*pointer);
        serialize_impl(tmp, handler, (binding.value.params, params), &child);
      }

      template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Checker, class AdditionalParams>
      void operator()(proto::tag::terminal, const binding_wrapper<accessor_binding<Getter, getter, Setter, setter, Delegate, Path, Checker, AdditionalParams> > &binding) const {
        auto child = binding.value.path.create(*target);
        Delegate tmp = static_cast<Delegate>((source.*getter)());
        serialize_impl(tmp, handler, (binding.value.params, params), &child);
      }

      template<class Fixer>
      void operator()(proto::tag::terminal, const binding_wrapper<fixup_binding<Fixer> > &) const {
        /* Just do nothing. */
      }

      template<class Pred, class L, class R>
      void operator()(proto::tag::if_else_, const Pred &pred, const L &l, const R &r) const {
        if(proto::value(pred)(source))
          proto::eval(l, *this);
        else 
          proto::eval(r, *this);
      }

      const T &source;
      MessageHandler &handler;
      const Params &params;
      Node *target;
      mutable bool success;
    };


    /**
     * Deserialization context.
     */
    template<class Node, class MessageHandler, class Params, class T>
    struct deserialization_context:
      proto::callable_context<const deserialization_context<Node, MessageHandler, Params, T> >
    {
      deserialization_context(const Node &source, MessageHandler &handler, const Params &params, T *target): 
        source(source), handler(handler), params(params), target(target), success(true) {}

      typedef bool result_type;

      bool operator()(proto::tag::terminal, const binding_wrapper<noop_binding> &) const {
        return true;
      }

      template<class L, class R>
      bool operator()(proto::tag::comma, const L &l, const R &r) const {
        /* Bitwise "and" is important here. We want to collect errors from both
         * children. */
        return proto::eval(l, *this) & proto::eval(r, *this);
      }

      template<class Path, class Serializer, class Deserializer, class AdditionalParams>
      bool operator()(proto::tag::terminal, const binding_wrapper<functional_binding<Path, Serializer, Deserializer, AdditionalParams> > &binding) const {
        auto child = binding.value.path.traverse(source);
        auto newParams = (binding.value.params, params);
        auto translator = create_translator(handler, newParams, success);
        if(!check_not_null(child, translator, binding.value.path))
          return false;
        binding.value.deserializer(child, translator, newParams, target);
        return success; /* Note that this is not generally correct as success may already be false. */
      }

      template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Checker, class AdditionalParams>
      bool operator()(proto::tag::terminal, const binding_wrapper<member_binding<MemberPointer, pointer, Delegate, Path, Checker, AdditionalParams> > &binding) const {
        typedef typename member_type<MemberPointer>::type Actual;
        auto child = binding.value.path.traverse(source);
        auto newParams = (binding.value.params, params);
        auto translator = create_translator(handler, newParams, success);
        if(!check_not_null(child, translator, binding.value.path))
          return false;
        Delegate tmp;
        if(deserialize_impl(child, handler, newParams, &tmp)) {
          Actual actualTmp = static_cast<Actual>(tmp);
          if(!binding.value.checker(actualTmp)) {
            typename node_inspector<Node>::type inspector;
            translator(
              ERROR, 
              create_invalid_value_for_type<Actual>(inspector.value(child)), 
              inspector.location(child)
            );
            return false;
          } else {
            (target->*pointer) = actualTmp;
            return true;
          }
        } else {
          return false;
        }
      }

      template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Checker, class AdditionalParams>
      bool operator()(proto::tag::terminal, const binding_wrapper<accessor_binding<Getter, getter, Setter, setter, Delegate, Path, Checker, AdditionalParams> > &binding) const {
        typedef typename setter_param<Setter>::type Actual;
        auto child = binding.value.path.traverse(source);
        auto newParams = (binding.value.params, params);
        auto translator = create_translator(handler, newParams, success);
        if(!check_not_null(child, translator, binding.value.path))
          return false;
        Delegate tmp;
        if(deserialize_impl(child, handler, newParams, &tmp)) {
          Actual actualTmp = static_cast<Actual>(tmp);
          if(!binding.value.checker(actualTmp)) {
            typename node_inspector<Node>::type inspector;
            translator(
              ERROR, 
              create_invalid_value_for_type<Actual>(inspector.value(child)), 
              inspector.location(child)
            );
            return false;
          } else {
            (target->*setter)(actualTmp);
            return true;
          }
        } else {
          return false;
        }
      }

      template<class Fixer>
      bool operator()(proto::tag::terminal, const binding_wrapper<fixup_binding<Fixer> > &binding) const {
        if(success) {
          /* Fixer is called only if deserialization up to this point was 
           * successful. */
          binding.value.fixer(*target);
        }
        return true;
      }

      template<class Pred, class L, class R>
      bool operator()(proto::tag::if_else_, const Pred &pred, const L &l, const R &r) const {
        return proto::value(pred)(*target) ? proto::eval(l, *this) : proto::eval(r, *this);
      }

      template<class Path, class Translator>
      bool check_not_null(const Node &node, Translator &translator, const Path &path) const {
        if(typename node_walker<Node>::type().is_null(node)) {
          translator(
            ERROR, 
            create_node_not_found(
              path.to_string_process(
                typename node_string_processor<Node>::type()
              )
            ), 
            typename node_inspector<Node>::type().location(node)
          );
          return false;
        } else {
          return true;
        }
      }

      const Node &source;
      MessageHandler &handler;
      const Params &params;
      T *target;
      mutable bool success;
    };


    /**
     * Expression wrapper for binding expressions.
     */
    template<class Expr>
    struct binding_expression {
      typedef binding_expression<Expr> this_type;

      BOOST_PROTO_EXTENDS(Expr, this_type, binding_domain);

      template<class T, class MessageHandler, class Params, class Node>
      void serialize(const T &source, MessageHandler &handler, const Params &params, Node *target) const {
        serialization_context<T, MessageHandler, Params, Node> ctx(source, handler, params, target);
        proto::eval(*this, ctx);
      }

      template<class Node, class T, class MessageHandler, class Params>
      bool deserialize(const Node &source, MessageHandler &handler, const Params &params, T *target) const {
        deserialization_context<Node, MessageHandler, Params, T> ctx(source, handler, params, target);
        return proto::eval(*this, ctx);
      }

    };


    template<class T>
    struct is_binding_expression_impl: 
      mpl::false_ 
    {};

    template<class Expr>
    struct is_binding_expression_impl<binding_expression<Expr> >: 
      mpl::true_ 
    {};


    /**
     * Metafunction that returns true for binding expressions.
     */
    template<class T>
    struct is_binding_expression: 
      is_binding_expression_impl<typename remove_cv_reference<T>::type>
    {};


    /**
     * Internal deserialization routine. Differs from public one in that
     * it deserializes directly into its target parameter, without creating
     * any temporaries.
     */
    template<class Node, class T, class MessageHandler, class Params>
    bool deserialize_impl(const Node &source, MessageHandler &handler, const Params &params, T *target) {
      return binding(static_cast<T *>(NULL)).deserialize(source, handler, params, target);
    }


    /**
     * Internal serialization routine. No difference from the public one,
     * added just for interface consistency with deserialization.
     */
    template<class T, class MessageHandler, class Params, class Node>
    void serialize_impl(const T &source, MessageHandler &handler, const Params &params, Node *target) {
      binding(static_cast<T *>(NULL)).serialize(source, handler, params, target);
    }


    /**
     * Noop binding literal.
     */
    namespace {
      binding_expression<proto::terminal<binding_wrapper<noop_binding> >::type> noop = {{{{}}}};
    }

  } // namespace binding_detail

  using binding_detail::noop;
  using binding_detail::delegate;
  using binding_detail::functional;
  using binding_detail::member;
  using binding_detail::accessor;
  using binding_detail::fixup;
  using binding_detail::is_binding_expression;
  using boost::proto::if_else;

  /**
   * Message handler that simply ignores all incoming messages.
   */
  struct NullMessageHandler {
    template<class ErrorData, class UserData>
    void operator()(ErrorSeverity, const ErrorData &, const UserData &, const ErrorLocation &) {
      return;
    }
  };


  /* Serialization. */

  template<class T, class Node>
  void serialize(
    const T &source, 
    Node *target
  ) {
    NullMessageHandler handler;
    serialize(source, handler, no_properties, target);
  }

  template<class T, class Params, class Node>
  typename boost::enable_if<is_property_expression<Params>, void>::type
  serialize(
    const T &source, 
    Params &params, 
    Node *target
  ) {
    NullMessageHandler handler;
    serialize(source, handler, params, target);
  }

  template<class T, class MessageHandler, class Node>
  typename boost::disable_if<is_property_expression<MessageHandler>, void>::type
  serialize(
    const T &source, 
    MessageHandler &handler, 
    Node *target
  ) {
    serialize(source, handler, no_properties, target);
  }

  template<class T, class MessageHandler, class TranslatorParams, class Params, class Node>
  typename boost::enable_if<is_property_expression<Params>, void>::type
  serialize(
    const T &source, 
    binding_detail::MessageTranslator<MessageHandler, TranslatorParams> &translator, 
    const Params &params, 
    Node *target
  ) {
    serialize(source, translator.handler, params, target);
  }

  template<class T, class MessageHandler, class Params, class Node>
  typename boost::enable_if<is_property_expression<Params>, void>::type
  serialize(
    const T &source, 
    MessageHandler &handler, 
    const Params &params, 
    Node *target
  ) {
    binding_detail::serialize_impl(source, handler, params, target);
  }


  /* Deserialization. */

  template<class Node, class T>
  bool deserialize(
    const Node &source, 
    T *target
  ) {
    NullMessageHandler handler;
    return deserialize(source, handler, no_properties, target);
  }

  template<class Node, class Params, class T>
  typename boost::enable_if<is_property_expression<Params>, bool>::type
  deserialize(
    const Node &source, 
    Params &params, 
    T *target
  ) {
    NullMessageHandler handler;
    return deserialize(source, handler, params, target);
  }

  template<class Node, class MessageHandler, class T>
  typename boost::disable_if<is_property_expression<MessageHandler>, bool>::type
  deserialize(
    const Node &source, 
    MessageHandler &handler, 
    T *target
  ) {
    return deserialize(source, handler, no_properties, target);
  }

  template<class Node, class MessageHandler, class TranslatorParams, class Params, class T>
  typename boost::enable_if<is_property_expression<Params>, bool>::type
  deserialize(
    const Node &source, 
    binding_detail::MessageTranslator<MessageHandler, TranslatorParams> &translator, 
    const Params &params, 
    T *target
  ) {
    return deserialize(source, translator.handler, params, target);
  }

  template<class Node, class MessageHandler, class Params, class T>
  typename boost::enable_if<is_property_expression<Params>, bool>::type
  deserialize(const Node &source, MessageHandler &handler, const Params &params, T *target) {
    T tmp;
    if(binding_detail::deserialize_impl(source, handler, params, &tmp)) {
      *target = tmp;
      return true;
    } else {
      return false;
    }
  }


#define ARX_XML_DEFINE_BINDING_I(                                               \
  NS_START,                                                                     \
  NS_END,                                                                       \
  NS_PREFIX,                                                                    \
  NAME,                                                                         \
  NAME_TPL_HEAD,                                                                \
  NAME_TPL_SPEC_HEAD,                                                           \
  NAME_TPL_SPEC,                                                                \
  NAME_TPL_SPEC_ACC,                                                            \
  HOLDER,                                                                       \
  HOLDER_TPL_HEAD,                                                              \
  HOLDER_TPL_SPEC_HEAD,                                                         \
  HOLDER_TPL_SPEC,                                                              \
  HOLDER_TPL_SPEC_ACC,                                                          \
  TYPE,                                                                         \
  TYPE_TPL,                                                                     \
  ... /* BINDING */                                                             \
)                                                                               \
  ARX_STRIP(NS_START)                                                           \
    ARX_STRIP(NAME_TPL_HEAD)                                                    \
    class NAME;                                                                 \
                                                                                \
    ARX_STRIP(NAME_TPL_SPEC_HEAD)                                               \
    class NAME ARX_STRIP(NAME_TPL_SPEC) {                                       \
    public:                                                                     \
      typedef decltype(boost::proto::deep_copy((__VA_ARGS__))) result_type;     \
      result_type operator()() const {                                          \
        return boost::proto::deep_copy((__VA_ARGS__));                          \
      }                                                                         \
    };                                                                          \
  ARX_STRIP(NS_END)                                                             \
                                                                                \
  namespace xml_binding_definition_detail {                                     \
    ARX_STRIP(HOLDER_TPL_HEAD)                                                  \
    struct HOLDER;                                                              \
                                                                                \
    ARX_STRIP(HOLDER_TPL_SPEC_HEAD)                                             \
    struct HOLDER ARX_STRIP(HOLDER_TPL_SPEC) {                                  \
      static const ARX_STRIP(NS_PREFIX) NAME ARX_STRIP(NAME_TPL_SPEC_ACC)::result_type \
        binding;                                                                \
    };                                                                          \
                                                                                \
    ARX_STRIP(HOLDER_TPL_SPEC_HEAD)                                             \
    const typename ARX_STRIP(NS_PREFIX) NAME ARX_STRIP(NAME_TPL_SPEC_ACC)::result_type \
      HOLDER ARX_STRIP(HOLDER_TPL_SPEC)::binding =                              \
      ARX_STRIP(NS_PREFIX) NAME ARX_STRIP(NAME_TPL_SPEC_ACC)()();               \
  }                                                                             \
                                                                                \
  ARX_STRIP(TYPE_TPL)                                                           \
  inline const ARX_STRIP(NS_PREFIX) NAME ARX_STRIP(NAME_TPL_SPEC_ACC)::result_type & \
  binding(const TYPE *) {                                                       \
    return xml_binding_definition_detail::HOLDER ARX_STRIP(HOLDER_TPL_SPEC_ACC)::binding; \
  }


#define ARX_XML_DEFINE_NAMED_TPL_BINDING(NAME, TYPE, TYPE_TPL, ... /* BINDING */) \
  ARX_XML_DEFINE_BINDING_I(                                                     \
    (),                                                                         \
    (),                                                                         \
    (),                                                                         \
    NAME,                                                                       \
    (),                                                                         \
    (),                                                                         \
    (),                                                                         \
    (),                                                                         \
    ARX_CAT_3(NAME, _holder_, __LINE__),                                        \
    (template<class Dummy, class Dummy2>),                                      \
    (template<class Dummy>),                                                    \
    (<Dummy, void>),                                                            \
    (<void, void>),                                                             \
    TYPE,                                                                       \
    TYPE_TPL,                                                                   \
    __VA_ARGS__                                                                 \
  )


/**
 * This macro defines named xml binding for the given TYPE.
 *
 * Binding construction code is placed into the class with the given NAME,
 * thus making it possible to befriend this class from the client code.
 * This allows to access private members of the client class from the 
 * binding code.
 */
#define ARX_XML_DEFINE_NAMED_BINDING(NAME, TYPE, ... /* BINDING */)             \
  ARX_XML_DEFINE_NAMED_TPL_BINDING(NAME, TYPE, (), __VA_ARGS__)
  

/**
 * This macro defines an xml binding for the given TYPE.
 */
#define ARX_XML_DEFINE_BINDING(TYPE, ... /* BINDING */)                         \
  ARX_XML_DEFINE_BINDING_I(                                                     \
    (namespace xml_binding_definition_detail {),                                \
    (}),                                                                        \
    (xml_binding_definition_detail::),                                          \
    xml_binding_definition,                                                     \
    (template<class T>),                                                        \
    (template<>),                                                               \
    (<TYPE>),                                                                   \
    (<TYPE>),                                                                   \
    xml_binding_holder,                                                         \
    (template<class T, class Dummy>),                                           \
    (template<class Dummy>),                                                    \
    (<TYPE, Dummy>),                                                            \
    (<TYPE, void>),                                                             \
    TYPE,                                                                       \
    (),                                                                         \
    __VA_ARGS__                                                                 \
  )


/**
 * This macro constructs a member xml binding.
 *
 * Actual signature is as follows:
 * <tt>ARX_XML_MEMBER(member_pointer, path, [checker], [params], [delegate])</tt>.
 * Parameters in brackets are optional.
 * 
 * This macro is a shortcut to arx::xml::member function and it takes the same 
 * parameters except for the fact that there is no need to supply the 
 * type of member pointer.
 */
#define ARX_XML_MEMBER(MEMBER_POINTER, PATH, ...)                               \
  arx::xml::member<decltype(MEMBER_POINTER), MEMBER_POINTER>(PATH, ##__VA_ARGS__)


/**
 * This macro constructs an accessor xml binding.
 * 
 * Actual signature is as follows:
 * <tt>ARX_XML_ACCESSOR(getter_pointer, setter_pointer, path, [checker], [params], [delegate])</tt>.
 * Parameters in brackets are optional.
 *
 * This macro is a shortcut to arx::xml::accessor function and it takes the same
 * parameters except for the fact that there is no need to supply the types
 * of getter and setter pointers.
 */
#define ARX_XML_ACCESSOR(GETTER_POINTER, SETTER_POINTER, PATH, ...)             \
  arx::xml::accessor<                                                           \
    decltype(GETTER_POINTER),                                                   \
    GETTER_POINTER,                                                             \
    decltype(SETTER_POINTER),                                                   \
    SETTER_POINTER                                                              \
  >(PATH, ##__VA_ARGS__)


/**
 * This macro constructs a functional xml binding.
 *
 * Actual signature is as follows:
 * <tt>ARX_XML_FUNCTIONAL(path, serializer, deserializer, [params])</tt>
 * Parameters in brackets are optional.
 *
 * This macro is a shortcut to arx::xml::accessor function and it takes the same
 * parameters.
 */
#define ARX_XML_FUNCTIONAL(PATH, SERIALIZER, DESERIALIZER, ...)                 \
  arx::xml::functional(PATH, SERIALIZER, DESERIALIZER, ##__VA_ARGS__)


/**
 * This macro constructs a fixup xml binding.
 */
#define ARX_XML_FIXUP(FIXER)                                                    \
  arx::xml::fixup(FIXER)


/**
 * This macro constructs a conditional xml binding.
 */
#define ARX_XML_IF(CONDITION, THEN)                                             \
  arx::xml::if_else(CONDITION, THEN, arx::xml::noop)


/**
 * This macro constructs a conditional xml binding.
 */
#define ARX_XML_IF_ELSE(CONDITION, THEN, ELSE)                                  \
  arx::xml::if_else(CONDITION, THEN, ELSE)

}} // namespace arx::xml

#endif // ARX_XML_BINDING_H

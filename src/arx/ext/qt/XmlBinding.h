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
#ifndef ARX_EXT_QT_XML_BINDING_H
#define ARX_EXT_QT_XML_BINDING_H

#include "config.h"
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/proto/proto.hpp>
#include <QtGlobal> /* For QtMsgType. */
#include <QSourceLocation>
#include <QDomNode>
#include <arx/Preprocessor.h>
#include <arx/Properties.h>
#include "XmlPath.h"
#include "XmlQDom.h"
#include "XmlUserData.h"

namespace arx { namespace xml {
  /**
    * Message handler that simply ignores all incoming messages.
    */
  struct NullMessageHandler {
    template<class ErrorData, class UserData>
    void operator()(QtMsgType /*severity*/, const ErrorData&, const UserData &, const QSourceLocation &) {
      return;
    }
  };

  template<class T>
  bool serialize(const T &source, QDomNode *target) {
    NullMessageHandler handler;
    return serialize(source, handler, no_properties, target);
  }

  template<class T, class MessageHandler>
  bool serialize(const T &source, MessageHandler &handler, QDomNode *target) {
    return serialize(source, handler, no_properties, target);
  }

  template<class T, class MessageHandler, class Params>
  bool serialize(const T &source, MessageHandler &handler, const Params &params, QDomNode *target) {
    return binding(static_cast<T *>(NULL)).serialize(source, handler, params, target);
  }

  template<class T>
  bool deserialize(QDomNode &source, T *target) {
    NullMessageHandler handler;
    return deserialize(source, handler, no_properties, target);
  }

  template<class T, class MessageHandler>
  bool deserialize(QDomNode &source, MessageHandler &handler, T *target) {
    return deserialize(source, handler, no_properties, target);
  }

  template<class T, class MessageHandler, class Params>
  bool deserialize(QDomNode &source, MessageHandler &handler, const Params &params, T *target) {
    return binding(static_cast<T *>(NULL)).deserialize(source, handler, params, target);
  }


  namespace xml_binding_detail {
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
     * Metafunction that removes const, volatile and reference modifiers of the
     * given type.
     */
    template<class T>
    struct remove_cvr: 
      boost::remove_cv<typename boost::remove_reference<T>::type> 
    {};


    /**
     * Metafunction that returns return type of the given getter member 
     * function pointer type.
     */
    template<class T>
    struct getter_result;

    template<class Class, class Result>
    struct getter_result<Result (Class::*)() const>:
      remove_cvr<Result>
    {};


    /**
     * Metafunction that returns parameter type of the given setter member 
     * function pointer type.
     */
    template<class T>
    struct setter_param;

    template<class Class, class Result, class Param>
    struct setter_param<Result (Class::*)(Param)>:
      remove_cvr<Param>
    {};


    /**
     * Wrapper for a message handler that checks whether there were any 
     * fatal errors and translates error messages.
     */
    template<class MessageHandler, class Params>
    struct MessageTranslator {
    public:
      explicit MessageTranslator(MessageHandler &handler, const Params &params):
        handler(handler), params(params), success(true) {}

      template<class ErrorData>
      void operator()(QtMsgType severity, const ErrorData& errorData, const QSourceLocation &sourceLocation) {
        assert(severity == QtFatalMsg || severity == QtWarningMsg);

        if(severity == QtFatalMsg)
          success = false;

        handler(severity, errorData, params.get<user_data_tag>(no_user_data()), sourceLocation);
      }

      MessageHandler &handler;
      const Params &params;
      bool success;
    };

    template<class MessageHandler, class Params>
    MessageTranslator<MessageHandler, Params> create_translator(MessageHandler &handler, const Params &params) {
      return MessageTranslator<MessageHandler, Params>(handler, params);
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
     * Functional binding.
     */
    template<class Path, class Serializer, class Deserializer, class Params>
    struct functional_binding: binding_base<Path, Params> {
      functional_binding(const Path &path, const Serializer &serializer, const Deserializer &deserializer, const Params &params):
        binding_base(path, params), serializer(serializer), deserializer(deserializer) {}

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
      binding_type::expr_type result = {{{binding_type(path, serializer, deserializer, params)}}};
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
    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Params>
    struct member_binding: binding_base<Path, Params> {
      member_binding(const Path &path, const Params &params):
        binding_base(path, params) {}

      typedef 
        binding_expression<typename proto::terminal<binding_wrapper<member_binding> >::type>
      expr_type;
    };

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Params>
    typename member_binding<MemberPointer, pointer, Delegate, Path, Params>::expr_type
    member_impl(const Path &path, const Params &params) {
      typedef member_binding<MemberPointer, pointer, Delegate, Path, Params> binding_type;
      binding_type::expr_type result = {{{binding_type(path, params)}}};
      return result;
    }

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Params>
    auto member(const Path &path, const Params &params) ->
      decltype(member_impl<MemberPointer, pointer, Delegate>(path, params)) 
    {
      return member_impl<MemberPointer, pointer, Delegate>(path, params);
    }

    template<class MemberPointer, MemberPointer pointer, class Delegate, class Path>
    auto member(const Path &path) ->
      decltype(member_impl<MemberPointer, pointer, Delegate>(path, no_properties))
    {
      return member_impl<MemberPointer, pointer, Delegate>(path, no_properties);
    }

    template<class MemberPointer, MemberPointer pointer, class Path, class Params>
    auto member(const Path &path, const Params &params) ->
      decltype(member_impl<MemberPointer, pointer, typename member_type<MemberPointer>::type>(path, params))
    {
      return member_impl<MemberPointer, pointer, typename member_type<MemberPointer>::type>(path, params);
    }

    template<class MemberPointer, MemberPointer pointer, class Path>
    auto member(const Path &path) ->
      decltype(member_impl<MemberPointer, pointer, typename member_type<MemberPointer>::type>(path, no_properties))
    {
      return member_impl<MemberPointer, pointer, typename member_type<MemberPointer>::type>(path, no_properties);
    }


    /**
     * Accessor binding.
     */
    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Params>
    struct accessor_binding: binding_base<Path, Params> {
      accessor_binding(const Path &path, const Params &params):
        binding_base(path, params) {}

      typedef 
        binding_expression<typename proto::terminal<binding_wrapper<accessor_binding> >::type>
      expr_type;
    };
    
    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Params>
    typename accessor_binding<Getter, getter, Setter, setter, Delegate, Path, Params>::expr_type
    accessor_impl(const Path &path, const Params &params) {
      typedef accessor_binding<Getter, getter, Setter, setter, Delegate, Path, Params> binding_type;
      binding_type::expr_type result = {{{binding_type(path, params)}}};
      return result;
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Params>
    auto accessor(const Path &path, const Params &params) ->
      decltype(accessor_impl<Getter, getter, Setter, setter, Delegate>(path, params)) 
    {
      return accessor_impl<Getter, getter, Setter, setter, Delegate>(path, params);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Params>
    auto accessor(const Path &path) ->
      decltype(accessor_impl<Getter, getter, Setter, setter, Delegate>(path, no_properties))
    {
      return accessor_impl<Getter, getter, Setter, setter, Delegate>(path, no_properties);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Path, class Params>
    auto accessor(const Path &path, const Params &params) ->
      typename boost::enable_if<
        boost::is_same<
          typename getter_result<Getter>::type,
          typename setter_param<Setter>::type
        >, 
        decltype(accessor_impl<Getter, getter, Setter, setter, typename getter_result<Getter>::type>(path, params))
      >::type
    {
      return accessor_impl<Getter, getter, Setter, setter, typename getter_result<Getter>::type>(path, params);
    }

    template<class Getter, Getter getter, class Setter, Setter setter, class Path>
    auto accessor(const Path &path) ->
      typename boost::enable_if<
        boost::is_same<
          typename getter_result<Getter>::type,
          typename setter_param<Setter>::type
        >,
        decltype(accessor_impl<Getter, getter, Setter, setter, typename getter_result<Getter>::type>(path, no_properties))
      >::type
    {
      return accessor_impl<Getter, getter, Setter, setter, typename getter_result<Getter>::type>(path, no_properties);
    }


    /**
     * Common context.
     */
    template<class Derived>
    struct common_context:
      proto::callable_context<Derived>
    {
      typedef bool result_type;

      bool operator()(proto::tag::terminal, const binding_wrapper<noop_binding> &) const {
        return true;
      }

      template<class L, class R>
      bool operator()(proto::tag::comma, const L &l, const R &r) const {
        /* Bitwise "and" is important here. We want to collect errors from both
         * children. */
        return proto::eval(l, derived()) & proto::eval(r, derived());
      }

      const Derived &derived() const {
        return *static_cast<const Derived *>(this);
      }

    };


    /**
     * Serialization context.
     */
    template<class T, class MessageHandler, class Params>
    struct serialization_context: 
      common_context<const serialization_context<T, MessageHandler, Params> >
    {
      typedef 
        common_context<const serialization_context<T, MessageHandler, Params> >
      base_type;

      using base_type::operator();

      serialization_context(const T &source, MessageHandler &handler, const Params &params, QDomNode *target): 
        source(source), handler(handler), params(params), target(target) {}

      template<class Path, class Serializer, class Deserializer, class Params>
      bool operator()(proto::tag::terminal, const binding_wrapper<functional_binding<Path, Serializer, Deserializer, Params> > &binding) const {
        auto newParams = (binding.value.params, params);
        auto translator = create_translator(handler, newParams);
        QDomNode child = binding.value.path.create(*target);
        binding.value.serializer(source, translator, newParams, &child);
        return translator.success;
      }

      template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Params>
      bool operator()(proto::tag::terminal, const binding_wrapper<member_binding<MemberPointer, pointer, Delegate, Path, Params> > &binding) const {
        QDomNode child = binding.value.path.create(*target);
        Delegate tmp = static_cast<Delegate>(source.*pointer);
        return serialize(tmp, handler, (binding.value.params, params), &child);
      }

      template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Params>
      bool operator()(proto::tag::terminal, const binding_wrapper<accessor_binding<Getter, getter, Setter, setter, Delegate, Path, Params> > &binding) const {
        QDomNode child = binding.value.path.create(*target);
        Delegate tmp = static_cast<Delegate>((source.*getter)());
        return serialize(tmp, handler, (binding.value.params, params), &child);
      }

      template<class Pred, class L, class R>
      bool operator()(proto::tag::if_else_, const Pred &pred, const L &l, const R &r) const {
        return proto::value(pred)(source) ? proto::eval(l, *this) : proto::eval(r, *this);
      }

      const T &source;
      MessageHandler &handler;
      const Params &params;
      QDomNode *target;
    };


    /**
     * Deserialization context.
     */
    template<class T, class MessageHandler, class Params>
    struct deserialization_context:
      common_context<const deserialization_context<T, MessageHandler, Params> >
    {
      typedef 
        common_context<const deserialization_context<T, MessageHandler, Params> >
      base_type;

      using base_type::operator();

      deserialization_context(QDomNode &source, MessageHandler &handler, const Params &params, T *target): 
        source(source), handler(handler), params(params), target(target) {}

      template<class Path, class Serializer, class Deserializer, class Params>
      bool operator()(proto::tag::terminal, const binding_wrapper<functional_binding<Path, Serializer, Deserializer, Params> > &binding) const {
        auto newParams = (binding.value.params, params);
        auto translator = create_translator(handler, newParams);
        binding.value.deserializer(binding.value.path.traverse(source), translator, newParams, target);
        return translator.success;
      }

      template<class MemberPointer, MemberPointer pointer, class Delegate, class Path, class Params>
      bool operator()(proto::tag::terminal, const binding_wrapper<member_binding<MemberPointer, pointer, Delegate, Path, Params> > &binding) const {
        Delegate tmp;
        if(deserialize(binding.value.path.traverse(source), handler, (binding.value.params, params), &tmp)) {
          (target->*pointer) = static_cast<member_type<MemberPointer>::type>(tmp);
          return true;
        } else {
          return false;
        }
      }

      template<class Getter, Getter getter, class Setter, Setter setter, class Delegate, class Path, class Params>
      bool operator()(proto::tag::terminal, const binding_wrapper<accessor_binding<Getter, getter, Setter, setter, Delegate, Path, Params> > &binding) const {
        Delegate tmp;
        if(deserialize(binding.value.path.traverse(source), handler, (binding.value.params, params), &tmp)) {
          (target->*setter)(static_cast<setter_param<Setter>::type>(tmp));
          return true;
        } else {
          return false;
        }
      }

      template<class Pred, class L, class R>
      bool operator()(proto::tag::if_else_, const Pred &pred, const L &l, const R &r) const {
        return proto::value(pred)(*target) ? proto::eval(l, *this) : proto::eval(r, *this);
      }

      QDomNode &source;
      MessageHandler &handler;
      const Params &params;
      T *target;
    };


    /**
     * Expression wrapper for binding expressions.
     */
    template<class Expr>
    struct binding_expression {
      typedef binding_expression<Expr> this_type;

      BOOST_PROTO_EXTENDS(Expr, this_type, binding_domain);

      template<class T, class MessageHandler, class Params>
      bool serialize(const T &source, MessageHandler &handler, const Params &params, QDomNode *target) const {
        serialization_context<T, MessageHandler, Params> ctx(source, handler, params, target);
        return proto::eval(*this, ctx);
      }

      template<class T, class MessageHandler, class Params>
      bool deserialize(QDomNode &source, MessageHandler &handler, const Params &params, T *target) const {
        deserialization_context<T, MessageHandler, Params> ctx(source, handler, params, target);
        return proto::eval(*this, ctx);
      }

    };


    /**
     * Noop binding literal.
     */
    namespace {
      binding_expression<proto::terminal<binding_wrapper<noop_binding> >::type> noop = {{{}}};
    }

  } // namespace xml_binding_detail

  using xml_binding_detail::noop;
  using xml_binding_detail::functional;
  using xml_binding_detail::member;
  using xml_binding_detail::accessor;
  using boost::proto::if_else;


#define ARX_DEFINE_XML_BINDING_I(                                               \
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
  BINDING                                                                       \
)                                                                               \
  ARX_STRIP(NS_START)                                                           \
    ARX_STRIP(NAME_TPL_HEAD)                                                    \
    class NAME;                                                                 \
                                                                                \
    ARX_STRIP(NAME_TPL_SPEC_HEAD)                                               \
    class NAME ARX_STRIP(NAME_TPL_SPEC) {                                       \
    public:                                                                     \
      typedef decltype(boost::proto::deep_copy(BINDING)) result_type;           \
      result_type operator()() const {                                          \
        return boost::proto::deep_copy(BINDING);                                \
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
  const ARX_STRIP(NS_PREFIX) NAME ARX_STRIP(NAME_TPL_SPEC_ACC)::result_type &   \
  binding(const TYPE *) {                                                       \
    return xml_binding_definition_detail::HOLDER ARX_STRIP(HOLDER_TPL_SPEC_ACC)::binding; \
  }


#define ARX_DEFINE_NAMED_XML_BINDING_TPL(NAME, TYPE, TYPE_TPL, BINDING)         \
  ARX_DEFINE_XML_BINDING_I(                                                     \
    (),                                                                         \
    (),                                                                         \
    (),                                                                         \
    NAME,                                                                       \
    (),                                                                         \
    (),                                                                         \
    (),                                                                         \
    (),                                                                         \
    ARX_CAT_3(NAME, _holder_, __LINE__),                                        \
    (),                                                                         \
    (),                                                                         \
    (),                                                                         \
    (),                                                                         \
    TYPE,                                                                       \
    TYPE_TPL,                                                                   \
    BINDING                                                                     \
  )

#define ARX_DEFINE_NAMED_XML_BINDING(NAME, TYPE, BINDING)                       \
  ARX_DEFINE_NAMED_XML_BINDING_TPL(NAME, TYPE, (), BINDING)
  
#define ARX_DEFINE_XML_BINDING(TYPE, BINDING)                                   \
  ARX_DEFINE_XML_BINDING_I(                                                     \
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
    BINDING                                                                     \
  )

#define ARX_XML_MEMBER(MEMBER)                                                  \
  decltype(MEMBER), MEMBER


}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_BINDING_H

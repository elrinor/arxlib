#ifndef ARX_EXT_QT_XML_PATH_H
#define ARX_EXT_QT_XML_PATH_H

#include "config.h"
#include <boost/proto/proto.hpp>
#include "DefaultNodeWalker.h"

namespace arx { namespace xml {
  namespace detail {
    namespace proto = boost::proto;
    namespace fusion = boost::fusion;
    namespace mpl = boost::mpl;

    /**
     * Tag for the starting element of a path.
     */
    struct path_start {};


    struct path_element_grammar:
      proto::terminal<proto::_>
    {};

    struct path_head_grammar:
      proto::or_<
        proto::terminal<path_start>,
        path_element_grammar,
        proto::divides<path_head_grammar, path_element_grammar>
      >
    {};

    /**
     * Proto grammar for xml path expression.
     */
    struct path_grammar:
      proto::or_<
        proto::terminal<path_start>,
        path_element_grammar,
        proto::divides<path_head_grammar, path_element_grammar>,
        proto::multiplies<path_head_grammar, path_element_grammar>
      >
    {};


    /**
     * Transform that returns true if the given xml path expression defines an
     * attribute access.
     */
    struct path_is_attribute_transform:
      proto::or_<
        proto::when<
          proto::multiplies<proto::_, proto::_>,
          mpl::true_()
        >,
        proto::when<
          proto::terminal<proto::_>,
          mpl::false_()
        >,
        proto::when<
          proto::nary_expr<proto::_, proto::vararg<proto::_> >,
          proto::fold<
            proto::_, 
            mpl::false_(), 
            mpl::or_<path_is_attribute_transform, proto::_state>()
          >
        >
      >
    {};


    template<class Expr>
    struct path_expression;


    /**
     * Proto domain for xml path expressions.
     */
    struct path_domain: 
      proto::domain<proto::generator<path_expression>, path_grammar> 
    {};


    /**
     * Traversal context.
     */
    template<class Node, class NodeWalker>
    struct traversal_context: 
      proto::callable_context<const traversal_context<Node, NodeWalker> >
    {
      traversal_context(const Node &startNode, const NodeWalker &nodeWalker): 
        startNode(startNode), nodeWalker(nodeWalker) {}

      typedef Node result_type;

      Node operator()(proto::tag::terminal, const path_start &) const {
        return startNode;
      }

      template<class L, class R>
      Node operator()(proto::tag::divides, const L &l, const R &r) const {
        Node node = proto::eval(l, *this);

        if(nodeWalker.is_null(node))
          return node;

        return nodeWalker.element(node, proto::eval(r, *this));
      }

      template<class L, class R>
      Node operator()(proto::tag::multiplies, const L &l, const R &r) const {
        Node node = proto::eval(l, *this);

        if(nodeWalker.is_null(node))
          return node;

        return nodeWalker.attribute(node, proto::eval(r, *this));
      }

      const Node &startNode;
      const NodeWalker &nodeWalker;
    };


    /**
     * Creation context.
     */
    template<class Node, class NodeWalker, bool atRoot>
    struct creation_context:
      proto::callable_context<const creation_context<Node, NodeWalker, atRoot> >
    {
      creation_context(Node &startNode, const NodeWalker &nodeWalker): 
        startNode(startNode), nodeWalker(nodeWalker) {}

      typedef Node result_type;

      Node operator()(proto::tag::terminal, const path_start &) const {
        return startNode;
      }

      template<class L, class R>
      Node operator()(proto::tag::divides, const L &l, const R &r) const {
        if(atRoot) {
          /* At root node elements must always be created. */
          creation_context<Node, NodeWalker, false> ctx(startNode, nodeWalker);
          return nodeWalker.create_element(proto::eval(l, ctx), proto::eval(r, ctx));
        } else {
          auto name = proto::eval(r, *this);
          Node node = proto::eval(l, *this);

          /* At non-root nodes elements may be reused. */
          Node element = nodeWalker.element(node, name);
          if(nodeWalker.is_null(element))
            element = nodeWalker.create_element(node, name);
          return element;
        }
      }

      template<class L, class R>
      Node operator()(proto::tag::multiplies, const L &l, const R &r) const {
        static_assert(atRoot, "Attribute access must be a root node of path expression tree.");

        creation_context<Node, NodeWalker, false> ctx(startNode, nodeWalker);
        return nodeWalker.create_attribute(proto::eval(l, ctx), proto::eval(r, ctx));
      }

      Node &startNode;
      const NodeWalker &nodeWalker;
    };


    /**
     * Expression wrapper for property expressions.
     */
    template<class Expr>
    struct path_expression: 
      proto::extends<Expr, path_expression<Expr>, path_domain>
    {
      typedef path_expression<Expr> this_type;

      typedef
        proto::extends<Expr, this_type, path_domain>
      base_type;

      path_expression(const Expr &expr = Expr()): 
        base_type(expr)
      {}

      enum {
        IS_ATTRIBUTE = boost::result_of<path_is_attribute_transform(Expr)>::type::value,
        IS_ELEMENT = !IS_ATTRIBUTE
      };

      bool isAttribute() const {
        return IS_ATTRIBUTE;
      }

      bool isElement() const {
        return IS_ELEMENT;
      }

      template<class Node, class NodeWalker>
      Node traverse(const Node &startNode, const NodeWalker &nodeWalker) const {
        traversal_context<Node, NodeWalker> ctx(startNode, nodeWalker);
        return proto::eval(*this, ctx);
      }

      template<class Node>
      Node traverse(const Node &startNode) const {
        DefaultNodeWalker nodeWalker;
        return traverse(startNode, nodeWalker);
      }

      template<class Node, class NodeWalker>
      Node create(Node &startNode, const NodeWalker &nodeWalker) const {
        creation_context<Node, NodeWalker, true> ctx(startNode, nodeWalker);
        return proto::eval(*this, ctx);
      }

      template<class Node>
      Node create(Node &startNode) const {
        DefaultNodeWalker nodeWalker;
        return create(startNode, nodeWalker);
      }

    };

  } // namespace detail


  /**
   * Starting terminal for xml path expressions.
   */
  detail::path_expression<boost::proto::terminal<detail::path_start>::type> self;

}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_PATH_H

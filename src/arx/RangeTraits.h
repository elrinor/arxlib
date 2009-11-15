#ifndef __ARX_RANGETRAITS_H__
#define __ARX_RANGETRAITS_H__

#include "config.h"
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits.hpp>  /* for boost::is_convertible */
#include "HasTypedXxx.h"

namespace arx {
#if 0
// -------------------------------------------------------------------------- //
// Interface category
// -------------------------------------------------------------------------- //
  /** Tag for collections. Collections require the following interface:
   * <code>
   * boost::range_iterator<T>::type;
   * boost::range_const_iterator<T>::type;
   * boost::range_iterator<T>::type boost::begin(T&);
   * boost::range_iterator<T>::type boost::end(T&);
   * boost::range_const_iterator<T>::type boost::begin(const T&);
   * boost::range_const_iterator<T>::type boost::end(const T&);
   * </code> 
   */
  struct collection_tag {};

  /** Tag for finite collections. Finite collections additionally require: 
   * <code>
   * typedef size_type;
   *
   * size_type size();
   * </code>
   *
   * Optional requirements:
   * <code>
   * iterator insert(iterator, const value_type&);
   * iterator erase(iterator where);
   * iterator erase(iterator first, iterator last);
   * </code>
   *
   * Additional interface:
   * <code>
   * typedef reverse_iterator;
   * typedef const_reverse_iterator;
   *
   * reverse_iterator rbegin();
   * const_reverse_iterator rbegin() const;
   * reverse_iterator rend();
   * const_reverse_iterator rend() const;
   * size_type max_size();
   * void clear();
   * bool empty();
   * </code> 
   */ 
  struct finite_collection_tag: public collection_tag {};

  /** Tag for sequences. Sequences are required to expose at least one
   * function from additional interface.
   * 
   * Additional interface:
   * <code>
   * reference front();
   * void push_front(const value_type&);
   * void pop_front();
   * reference back();
   * void push_back(const value_type&);
   * void pop_back();
   * </code>
   */ 
  struct sequence_tag: public finite_collection_tag {};

  /** Tag for random access sequences. Random access sequences additionally require random access iterators.
   * Random access sequences additionally expose:
   *
   * <code>
   * reference at(size_type);
   * reference operator[] (size_type);
   * size_type capacity() const;
   * void reserve(size_type);
   * void resize(size_type);
   * void resize(size_type, const value_type&);
   * </code>
   */ 
  struct random_access_sequence_tag: public sequence_tag {};

  /** Tag for unordered maps. Unordered maps additionally require:
   *
   * <code>
   * typedef key_type;
   *
   * std::pair<iterator, iterator> equal_range(const key_type&);
   * iterator find(const key_type&);
   * </code>
   */ 
  struct unordered_map_tag: public finite_collection_tag {};

  /** Tag for ordered maps. Ordered maps additionally require: 
   *
   * <code>
   * iterator lower_bound(const key_type&);
   * iterator upper_bound(const key_type&);
   * </code>
   */ 
  struct ordered_map_tag: public finite_collection_tag {};

  namespace detail {
    namespace interface_category_impl {
      using namespace boost::mpl;

      BOOST_MPL_HAS_XXX_TRAIT_DEF(interface_category);



      BOOST_MPL_HAS_XXX_TRAIT_DEF(type);

      BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_nested_iterator_category, iterator::iterator_category, false);
      BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_nested_const_iterator_category, const_iterator::iterator_category, false);
      ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(has_cbegin, &T::begin, typename T::const_iterator (T::*) () const);
      ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(has_cend,   &T::end,   typename T::const_iterator (T::*) () const);

      struct cannot_deduce_interface_category;
/*
      template<class T>
      struct is_collection: public
        and_<
          and_<
            has_const_iterator<boost::range_const_iterator>,
            has_const_pointer<T>,
            has_const_reference<T>
          >,
          and_<
            has_iterator<T>,
            has_pointer<T>,
            has_reference<T>
          >,
          and_<
            has_difference_type<T>,
            has_size_type<T>,
            has_value_type<T>
          >,
          and_<
            has_cbegin<T>,
            has_cend<T>,
            has_nested_iterator_category<T>,
            has_nested_const_iterator_category<T>
          >
        > {};

      /*template<class T>
      struct is_*/
/*
      template<class T>
      struct interface_category: public
        if_<
          is_collection<T>,
          collection_tag,
          void
        > {};
*/
    } // namespace interface_category_impl
  } // namespace detail

  /** Metafunction that returns interface category of T. */
/*  template<class T>
  struct interface_category: public detail::interface_category_impl::interface_category<T> {};*/
#endif

// -------------------------------------------------------------------------- //
// Structural mutability
// -------------------------------------------------------------------------- //
#if 0
  /** Tag for structurally immutable collections. */
  struct structurally_immutable_tag {};

  /** Tag for structurally mutable collections. */
  struct structurally_mutable_tag {};

  /** Metafunction that returns structural mutability category of T. */
  template<class T>
  struct structural_mutability_category {
    typedef typename T::structural_mutability_category type;
  };
#endif


// -------------------------------------------------------------------------- //
// Itemwise mutability
// -------------------------------------------------------------------------- //
#if 0
  /** Tag for itemwise immutable collections. */
  struct itemwise_immutable_tag {};

  /** Tag for itemwise mutable collections. */
  struct itemwise_mutable_tag {};

  /** Metafunction that returns itemwise mutability category of T. */
  template<class T>
  struct itemwise_mutability_category {
    typedef typename T::itemwise_mutability_category type;
  };
#endif

// -------------------------------------------------------------------------- //
// Proxy
// -------------------------------------------------------------------------- //
#if 0
  /** Tag for proxy collections.
   * Proxy collection does not own the underlying data, therefore its constness does not
   * affect the mutability. */
  struct proxy_tag {};

  /** Tag for non-proxy collections. */
  struct non_proxy_tag {};

  /** Metafunction that returns proxy category of T. */
  template<class T>
  struct proxy_category {
    typedef typename T::proxy_category type;
  };
#endif

  /** Tag for proxy types. */
  struct proxy_tag {};

  /** Tag for non-proxy types. */
  struct non_proxy_tag {};

  namespace detail {
    namespace proxy_category_ns {
      using namespace boost::mpl;

      BOOST_MPL_HAS_XXX_TRAIT_DEF(proxy_category);

      template<class T, class Tag> 
      struct is_proxy_convertible: public boost::is_convertible<typename T::proxy_category, Tag> {};

      template<class T>
      struct proxy_category_impl: public 
        if_<
          not_<has_proxy_category<T> >, 
          non_proxy_tag,
          if_<
            is_proxy_convertible<T, proxy_tag>,
            proxy_tag,
            non_proxy_tag
          >
        > {};
    } // namespace proxy_category_ns
  } // namespace detail

  /** Metafunction that returns proxy category of T. */
  template<class T>
  proxy_category: public detail::proxy_category_ns::proxy_category_impl<T> {};

  /** Metafunction that returns whether T is a proxy type. 
   * Proxy does not own the underlying data, therefore it can be copied around freely.*/
  template<class T>
  is_proxy: public boost::is_same<proxy_category<T>::type, proxy_tag> {};


#if 0
// -------------------------------------------------------------------------- //
// 
// -------------------------------------------------------------------------- //
  /** Default view_category for views. */
  struct normal_view_tag {};

  namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(view_category);

    template<class T, bool has_category>
    struct view_category_impl: public boost::mpl::identity<typename T::view_category> {};

    template<class T>
    struct view_category_impl<T, false>: public boost::mpl::identity<void> {};
  } // namespace detail

  /** Metafunction that returns view category of T, or void in case T doesn't define a view category. */
  template<class T>
  struct view_category: public detail::view_category_impl<T, detail::has_view_category<T>::value>::type {};

  /** Metafunction that returns true if T is a normal view. */
  template<class T>
  struct is_normal_view: public boost::is_convertible<typename view_category<T>::type, normal_view_tag> {};

  /** Metafunction that returns how the given range should be stored. */
  template<class T>
  struct stored_range: public boost::mpl::if_<is_normal_view<T>, T, T&> {};
#endif



} // namespace arx


#endif // __ARX_RANGETRAITS_H__

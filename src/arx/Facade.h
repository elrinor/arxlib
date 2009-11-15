#ifndef __ARX_FACADE_H__
#define __ARX_FACADE_H__

#include "config.h"
#include <cassert>
#include <exception>                   /* for std::out_or_range. */
#include <boost/range.hpp>
#include <boost/iterator.hpp>          /* for new iterator categories. */
#include <boost/type_traits.hpp>       /* for boost::is_convertible. */
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/preprocessor/cat.hpp>

#include <vector>

namespace arx {
  namespace detail {
    namespace facades {
#define ARX_FACADE(facade_name)                                                 \
      template<class ActualBase>                                                \
      struct BOOST_PP_CAT(facade_name, _impl);                                  \
                                                                                \
      struct facade_name {                                                      \
        template<class ActualBase>                                              \
        struct apply {                                                          \
          typedef BOOST_PP_CAT(facade_name, _impl) type;                        \
        };                                                                      \
      };                                                                        \
                                                                                \
      template<class ActualBase>                                                \
      struct BOOST_PP_CAT(facade_name, _impl): public ActualBase

      /** If for facades. */
      template<class Cond, class ThenFacade, class ElseFacade, class BaseFacade>
      struct If {
        template<class ActualBase>
        struct apply {
          typedef typename boost::mpl::if_<Cond, ThenFacade, ElseFacade>::type::apply<typename BaseFacade::apply<ActualBase>::type>::type type;
        };
      };

      /** Base for all facades. */
      template<class Derived, class Collection>
      struct Base {
        typedef Derived derived_type;
        typedef Collection collection_type;

        collection_type& collection() {
          return derived().crtpCollection();
        }

        const collection_type& collection() const {
          return derived().crtpCollection();
        }

        derived_type& derived() {
          return static_cast<derived_type&>(*this);
        }

        const derived_type& derived() const {
          return static_cast<const derived_type&>(*this);
        }
      };

      /** Skip facade - adds nothing. */
      ARX_FACADE(Skip) {};

      /** Generic facade for collections. */
      ARX_FACADE(Collection) {
        typedef typename boost::range_iterator<collection_type>::type         iterator;
        typedef typename boost::range_const_iterator<collection_type>::type   const_iterator;

        typedef typename boost::iterator_reference<iterator>::type            reference;
        typedef typename boost::iterator_pointer<iterator>::type              pointer;
        typedef typename boost::iterator_reference<const_iterator>::type      const_reference;
        typedef typename boost::iterator_pointer<const_iterator>::type        const_pointer;

        typedef typename boost::iterator_value<iterator>::type                value_type;
        typedef typename boost::iterator_difference<iterator>::type           difference_type;

        typedef typename boost::reverse_iterator<iterator>                    reverse_iterator;
        typedef typename boost::reverse_iterator<const_iterator>              const_reverse_iterator;

        iterator begin() {
          return boost::begin(collection());
        }

        const_iterator begin() const {
          return boost::begin(collection());
        }

        iterator end() {
          return boost::end(collection());
        }

        const_iterator end() const {
          return boost::end(collection());
        }

        reverse_iterator rbegin() {
          return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const {
          return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
          return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
          return const_reverse_iterator(begin());
        }

        iterator erase(iterator where) {
          return collection().erase(where);
        }

        iterator erase(iterator first, iterator last) {
          return collection().erase(first, last);
        }

        iterator insert(iterator where, const value_type& value) {
          return collection().insert(where, value);
        }

        void swap(derived_type& other) {
          collection().swap(other);
        }

        friend bool operator!= (const derived_type& l, const derived_type& r) {
          return !(l == r);
        }

        friend bool operator> (const derived_type& l, const derived_type& r) {
          return r < l;
        }

        friend bool operator<= (const derived_type& l, const derived_type& r) {
          return !(r > l);
        }

        friend bool operator>= (const derived_type& l, const derived_type& r) {
          return !(r < l);
        }
      };

      /** Facade for allocator-bearing collections. */
      ARX_FACADE(Allocator) {
        typedef typename collection_type::allocator_type allocator_type;

        allocator_type get_allocator() const {
          return collection().get_allocator();
        }
      };

      /** Facade for sized collections. */
      ARX_FACADE(Size) {
        typedef typename collection_type::size_type size_type;

        size_type size() const {
          return collection().size();
        }

        size_type max_size() const {
          return collection().max_size();
        }
      };

      /** Facade for random-access collections without size. */
      ARX_FACADE(SizeRandomAccess) {
        typedef std::size_t size_type;

        size_type size() const {
          return static_cast<size_type>(derived().end() - derived().begin());
        }
      };

      /** Facade for collections that define empty(). */
      ARX_FACADE(Empty) {
        bool empty() const {
          return collection().empty();
        }
      };

      /** Facade for collections that do not define empty() but have size. */
      ARX_FACADE(EmptySize) {
        bool empty() const {
          return derived().size() == 0;
        }
      };

      /** Facade for collections that define operator==. */
      ARX_FACADE(Equal) {
        friend bool operator== (const derived_type& l, const derived_type& r) {
          return l.collection() == r.collection();
        }
      };

      /** Facade for collections that do not define operator==. */
      ARX_FACADE(EqualNew) {
        bool operator== (const derived_type& other) const {
          return derived().size() == other.size() && std::equal(derived().begin(), derived().end(), other.begin());
        }
      };

      /** Facade for collections that define operator<. */
      ARX_FACADE(Less) {
        friend bool operator< (const derived_type& l, const derived_type& r) {
          return l.collection() < r.collection();
        }
      };

      /** Facade for collections that do not define operator<. */
      ARX_FACADE(LessNew) {
        bool operator< (const derived_type& other) const {
          return std::lexicographical_compare(derived().begin(), derived().end(), other.begin(), other.end());
        }
      };

      ARX_FACADE(Sequence) {
        reference front() {
          return *derived().begin();
        }

        const_reference front() const {
          return *derived().begin();
        }

        reference back() {
          iterator atEnd = derived().end();
          atEnd--;
          return *atEnd;
        }

        const_reference back() const {
          const_iterator atEnd = derived().end();
          atEnd--;
          return *atEnd;
        }

        void push_back(const value_type& value) {
          derived().insert(derived.end(), value);
        }

        void push_back() {
          derived().push_back(value_type());
        }

        void pop_back() {
          iterator atEnd = derived().end();
          atEnd--;
          derived().erase(atEnd);
        }

        void push_front(const value_type& value) {
          derived().insert(derived.begin(), value);
        }

        void push_front() {
          derived().push_front(value_type());
        }

        void pop_front() {
          derived().erase(derived().begin());
        }
      };

      ARX_FACADE(RandomAccessSequence) {
        reference at(size_type pos) {
          ARX_ASSERT_OR_THROW(pos >= 0 && pos < derived().size(), xRan());

          return container().at(pos);
        }

        const_reference at(size_type pos) const {
          ARX_ASSERT_OR_THROW(pos >= 0 && pos < derived().size(), xRan());

          return container().at(pos);
        }

        size_type capacity() const {
          return collection().capacity();
        }

        void reserve(size_type count) {
          collection().reserve(count);
        }

        void resize(size_type newSize) {
          collection().resize(newSize);
        }

        void resize(size_type newSize, const value_type& value) {
          collection().resize(newSize, value);
        }

        reference operator[] (size_type pos) {
          assert(pos >= 0 && pos < derived().size());

          return *(derived().begin() + pos);
        }

        const_reference operator[] (size_type pos) const {
          assert(pos >= 0 && pos < derived().size());

          return *(derived().begin() + pos);
        }

      private:
        void xRan() const {
          ARX_THROW(std::out_of_range("invalid subscript"));
        }
      };



#undef ARX_FACADE

      BOOST_MPL_HAS_XXX_TRAIT_DEF(allocator_type);
      BOOST_MPL_HAS_XXX_TRAIT_DEF(size_type);
      BOOST_MPL_HAS_XXX_TRAIT_DEF(key_type);
      ARX_HAS_TYPED_MEMBER_XXX_TRAIT_DEF(size, typename T::size_type (T::*) () const);
      ARX_HAS_TYPED_MEMBER_XXX_TRAIT_DEF(empty, bool (T::*) () const);
      ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(has_friend_equal, operator==, bool (*) (const T&, const T&));
      ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(has_friend_less,  operator<,  bool (*) (const T&, const T&));
      ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(has_member_equal, &T::operator==, bool (T::*) (const T&) const);
      ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(has_member_less,  &T::operator<,  bool (T::*) (const T&) const);
      ARX_HAS_TYPED_MEMBER_XXX_TRAIT_DEF(front, typename T::const_reference (T::*) () const);


      

      using namespace boost;
      using namespace boost::mpl;

      template<class Derived, class C, 
        class has_base_size = and_<has_size<C>, has_size_type<C> >,
        class is_random_access = is_convertible<typename iterator_traversal<typename range_iterator<C>::type>::type, random_access_traversal_tag>,
        class defines_size = or_<has_base_size, is_random_access>,
        class has_equal = or_<has_friend_equal<C>, has_member_equal<C> >,
        class has_less = or_<has_friend_less<C>, has_member_less<C> >
      >
      struct Facade: public
        If<has_allocator_type<C>, Allocator, Skip,
        If<has_empty, Empty, If<defines_size, EmptySize, Skip, Skip>,
        If<has_base_size, Size, If<defines_size, SizeRandomAccess, Skip, Skip>, 
        If<has_equal, Equal, EqualNew,
        If<has_less, Less, LessNew,
        If<or_<is_random_access, has_front<C> >, Sequence, Skip,
        If<is_random_access, RandomAccessSequence, Skip,
        If<has_key_type<C>, >
        > > > > > > >::apply<Base<Derived, C> >::type
      
    }



    template<class P, class T>
    struct FacadeImpl: public 
      CollectionFacade<
      If<has_allocator_type, AllocatorFacade, Skip, 

      > >
      

  }

  template<class Collection> class Facade: 
    public detail::FacadeImpl<detail::FacadePackage<Facade<Collection>, Collection>, Collection>


} // namespace arx

#endif // __ARX_FACADE_H__
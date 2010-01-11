#ifndef __ARX_UTILITY_H__
#define __ARX_UTILITY_H__

#include "config.h"
#include <boost/static_assert.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <functional>  /* for std::less. */
#include <utility>     /* for std::pair. */
#include <exception>   /* for std::runtime_error. */

#if 0
// -------------------------------------------------------------------------- //
// std::tr1::hash<std::pair<*> >
// -------------------------------------------------------------------------- //
namespace std {
  namespace tr1 {
    template<class T1, class T2>
    class hash<std::pair<T1, T2> >: public std::unary_function<std::pair<T1, T2>, size_t> {
    public:
      result_type operator()(const argument_type& pair) const {
        return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
      }
    };

  } // namespace tr1
} // namespace std
#endif // currently disabled

// -------------------------------------------------------------------------- //
// Some useful defines
// -------------------------------------------------------------------------- //
#ifdef ARX_MSVC
#  define FORCEINLINE __forceinline
# else
#  define FORCEINLINE inline
#endif

#define STATIC_ASSERT BOOST_STATIC_ASSERT

#define Unreachable() {assert(!"Unreachable code executed."); ARX_THROW(::std::runtime_error(__FILE__ ":" BOOST_PP_STRINGIZE(__LINE__) ": Unreachable code executed."));} 

namespace arx {
// -------------------------------------------------------------------------- //
// Some useful functions
// -------------------------------------------------------------------------- //
  /**
   * Square template
   */
  template<class T> 
  inline T sqr(T x) {
    return x * x;
  }


// -------------------------------------------------------------------------- //
// UnorderedPair
// -------------------------------------------------------------------------- //
  /**
   * UnorderedPair stores an unordered pair of two values of the same type.
   * Derives from Cmp for empty base optimization.
   */
  template<class T, class Cmp = std::less<T> > class UnorderedPair: private Cmp {
  public:
    T first;
    T second;

    UnorderedPair() {}

    UnorderedPair(const T& a, const T& b, const Cmp& c = Cmp()): 
      Cmp(c), first(Cmp::operator()(a, b) ? a : b), second(Cmp::operator()(a, b) ? b : a) {}

    bool operator<(const UnorderedPair& other) const {
      return Cmp::operator()(first, other.first) ||
        (!Cmp::operator()(other.first, first) && Cmp::operator()(second, other.second));
    }

    bool operator>(const UnorderedPair& other) const {
      return other < *this;
    }

    bool operator>=(const UnorderedPair& other) const {
      return !(*this < other);
    }

    bool operator<=(const UnorderedPair& other) const {
      return !(*this > other);
    }

    bool operator==(const UnorderedPair& other) const {
      return *this >= other && *this <= other;
    }

    bool operator!=(const UnorderedPair& other) const {
      return !(*this == other);
    }
  };

  template<class T, class Cmp> 
  UnorderedPair<T, Cmp> make_upair(const T& a, const T& b, const Cmp& c) {
    return UnorderedPair<T, Cmp>(a, b, c);
  }

  template<class T> 
  UnorderedPair<T> make_upair(const T& a, const T& b) {
    return UnorderedPair<T>(a, b);
  }

} // namespace arx

#endif // __ARX_UTILITY_H__

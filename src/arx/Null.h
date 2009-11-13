#ifndef __ARX_NULL_H__
#define __ARX_NULL_H__

#include <boost/noncopyable.hpp>
#include <boost/utility.hpp>            /* for boost::addressof */

namespace arx {
  namespace null_adl_protected {
    class null_t: public boost::noncopyable {
      bool operator== (const null_t&) {
        return true;
      }

      bool operator!= (const null_t&) {
        return false;
      }

      bool operator== (int value) {
        return value == 0;
      }

      bool operator!= (int value) {
        return value != 0;
      }
    };
  } // namespace null_adl_protected

  typedef null_adl_protected::null_t null_t;

  /* Declaring it static is OK and it solves the problem with multiple 
   * definitions when linking. */
  static null_t static_null;

} // namespace arx

/**
 * Typed null value, for use in expressions only.
 */
#define null (::arx::static_null)

/**
 * Null-comparable interface. For use in class definitions.
 *
 * @param CLASS class name.
 * @param NULL_CHECK_EXPR expression that checks whether this object is null.
 */
#define ARX_NULL_COMPARABLE(CLASS, NULL_CHECK_EXPR)                             \
  bool operator== (const arx::null_t&) const {                                  \
    return NULL_CHECK_EXPR;                                                     \
  }                                                                             \
                                                                                \
  bool operator!= (const arx::null_t& r) const {                                \
    return !operator== (r);                                                     \
  }                                                                             \
                                                                                \
  friend bool operator== (const arx::null_t& l, const CLASS& r) {               \
    return r == l;                                                              \
  }                                                                             \
                                                                                \
  friend bool operator!= (const arx::null_t& l, const CLASS& r) {               \
    return !(l == r);                                                           \
  }                                                                             \


/* TODO: think NULL_ASSIGNABLE / NULL_CONSTRUCTIBLE */

#endif // __ARX_NULL_H__

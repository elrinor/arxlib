#ifndef __ARX_UTILITY_H__
#define __ARX_UTILITY_H__

#include <utility> /* for std::pair */

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


#endif // __ARX_UTILITY_H__

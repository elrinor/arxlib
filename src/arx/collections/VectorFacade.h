#ifndef __ARX_VECTORFACADE_H__
#define __ARX_VECTORFACADE_H__

#include "config.h"
#include "RangeFacade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// VectorFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container> 
  class VectorFacade: public ContainerProxy<Derived, Container> {
  public:
    void assign(size_type count, const value_type& value) {
      container().assign(count, value);
    }

    template<class InputIterator>
    void assign(InputIterator first, InputIterator last) {
      container().assign(first, last);
    }

    reference at(size_type pos) {
      return container().at(pos);
    }

    const_reference at(size_type pos) const {
      return container().at(pos);
    }

    reference back() {
      return container().back();
    }

    const_reference back() const {
      return container().back();
    }

    size_type capacity() const {
      return container().capacity();
    }

    reference front() {
      return container().front();
    }

    const_reference front() const {
      return container().front();
    }

    void insert(iterator where, size_type count, const value_type& value) {
      return container().insert(where, count, value);
    }

    template<class InputIterator>
    void insert(iterator where, InputIterator first, InputIterator last) {
      return container().insert(where, first, last);
    }

    void pop_back() {
      container().pop_back();
    }

    void push_back(const value_type& value) { /* Differs from stl - reference used. */
      container().push_back(value);
    }

    void reserve(size_type count) {
      container().reserve(count);
    }

    void resize(size_type newSize) {
      container().resize(newSize);
    }

    void resize(size_type newSize, const value_type& value) {
      container().resize(newSize, value);
    }

    reference operator[] (size_type pos) {
      return container().operator[] (pos);
    }

    const_reference operator[] (size_type pos) const {
      return container().operator[] (pos);
    }

    /* Additional functions. */

    void push_back() {
      push_back(value_type());
    }

  protected:
    /*
    container_type& crtpContainer();
    const container_type& crtpContainer() const;
    */

  private:
    container_type& container() {
      return derived().crtpContainer();
    }

    const container_type& container() const {
      return derived().crtpContainer();
    }

    derived_type& derived() {
      return static_cast<derived_type&>(*this);
    }

    const derived_type& derived() const {
      return static_cast<const derived_type&>(*this);
    }
  }

} // namespace arx

#endif // __ARX_VECTORFACADE_H__

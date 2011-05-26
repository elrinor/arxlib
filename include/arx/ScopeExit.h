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
#ifndef ARX_SCOPE_EXIT_H
#define ARX_SCOPE_EXIT_H

#include <arx/config.h>
#include <utility> /* For std::move. */
#include <boost/preprocessor/cat.hpp>

namespace arx { namespace detail {
// -------------------------------------------------------------------------- //
// ScopeExit
// -------------------------------------------------------------------------- //
  template<class Functor>
  class ScopeExit {
  public:
    ScopeExit(ScopeExit &&other): mFunctor(std::move(other.mFunctor)) {}

    ScopeExit(Functor &&functor): mFunctor(std::move(functor)) {}

    ~ScopeExit() {
      mFunctor();
    }

  private:
    Functor mFunctor;
  };

// -------------------------------------------------------------------------- //
// ScopeExitIntroducer
// -------------------------------------------------------------------------- //
  class ScopeExitIntroducer {
  public:
    template<class Functor>
    ScopeExit<Functor> operator<<(Functor &&functor) const {
      return ScopeExit<Functor>(std::move(functor));
    }
  };

}} // namespace arx::detail


#define ARX_SCOPE_EXIT(... /* LAMBDA_CAPTURE */)                                \
  auto BOOST_PP_CAT(scopeExitTemporary_, __LINE__) =                            \
    ::arx::detail::ScopeExitIntroducer() << [__VA_ARGS__]()

#endif // ARX_SCOPE_EXIT_H

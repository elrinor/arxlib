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
#ifndef ARX_XML_ERROR_LOCATION_H
#define ARX_XML_ERROR_LOCATION_H

#include "config.h"

namespace arx { namespace xml {
// -------------------------------------------------------------------------- //
// ErrorLocation
// -------------------------------------------------------------------------- //
  class ErrorLocation {
  public:
    ErrorLocation(int line, int column): mLine(line), mColumn(column) {}

    int line() const {
      return mLine;
    }

    int column() const {
      return mColumn;
    }

  private:
    int mLine, mColumn;
  };
  
}} // namespace arx::xml

#endif // ARX_XML_ERROR_LOCATION_H

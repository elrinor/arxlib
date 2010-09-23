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
#ifndef ARX_EXT_QT_XML_USER_DATA_H
#define ARX_EXT_QT_XML_USER_DATA_H

#include "config.h"
#include <arx/Properties.h>

namespace arx { namespace xml {

  struct user_data_tag {};

  ARX_DEFINE_PROPERTY_KEY(user_data_tag, user_data);

  struct no_user_data {};

}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_USER_DATA_H

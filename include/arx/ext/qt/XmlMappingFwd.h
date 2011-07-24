/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2011 Alexander Fokin <apfokin@gmail.com>
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
#ifndef ARX_EXT_QT_XML_MAPPING_FWD_H
#define ARX_EXT_QT_XML_MAPPING_FWD_H

#include <arx/config.h>

namespace arx { namespace xml {
  class Serializer;
  class Deserializer;
}} // namespace arx::xml

#define ARX_XML_DECLARE_MAPPING_FUNCTIONS(... /* TYPE */)                       \
  void xml_serialize(arx::xml::Serializer &serializer, const __VA_ARGS__ &value); \
  void xml_deserialize(arx::xml::Deserializer &deserializer, __VA_ARGS__ &value); \

#endif // ARX_EXT_QT_XML_MAPPING_FWD_H

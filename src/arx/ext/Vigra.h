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
#ifndef ARX_EXT_VIGRA_H
#define ARX_EXT_VIGRA_H

#include "config.h"
#include "vigra/Accessors.h"
#include "vigra/Binarization.h"
#include "vigra/Colors.h"
#include "vigra/Convert.h"
#include "vigra/Converter.h"
#include "vigra/Drawing.h"
#include "vigra/MetaFunctions.h"
#include "vigra/RGBAValue.h"
#include "vigra/StdImage.h"
#include "vigra/Validation.h"

// -------------------------------------------------------------------------- //
// Inter-lib defines
// -------------------------------------------------------------------------- //
#ifndef ARX_USE_VIGRA
#  define ARX_USE_VIGRA
#endif

#ifdef ARX_USE_MAGICK
#  include "VigraMagick.h"
#endif

#ifdef ARX_USE_OPENCV
#  include "VigraOpenCV.h"
#endif

#ifdef ARX_USE_QT
#  include "VigraQt.h"
#endif

#endif // ARX_EXT_VIGRA_H

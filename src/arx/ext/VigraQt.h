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
#ifndef ARX_EXT_VIGRA_QT_H
#define ARX_EXT_VIGRA_QT_H

#include "config.h"
#include <cassert>
#include <cstring> /* for memcpy() */
#include <string>
#include <QImage>
#include "Vigra.h"
#include "Qt.h"

namespace arx {
// -------------------------------------------------------------------------- //
// Image conversion
// -------------------------------------------------------------------------- //
  template<class PixelType, class Alloc>
  void convert(const vigra::BasicImage<PixelType, Alloc>& vigraImage, QImage& qImage) {
    vigra::BBGRAImage placeholder;
    const vigra::BBGRAImage& vigraBgraImage = convert_nocopy(vigraImage, placeholder);

    /* Convert to QImage. */
    qImage = QImage(vigraBgraImage.width(), vigraBgraImage.height(), QImage::Format_ARGB32);

    /* g++ -Wall issues warning on the next line if we don't use static_cast. */
    assert(static_cast<int>(qImage.byteCount()) == static_cast<int>(vigraBgraImage.width() * vigraBgraImage.height() * sizeof(vigra::BRGBAImage::value_type))); 
    memcpy(qImage.bits(), vigraBgraImage.data(), qImage.byteCount());
  }

  template<class PixelType, class Alloc>
  void convert(const QImage& qImage, vigra::BasicImage<PixelType, Alloc>& vigraImage) {
    /* Convert to BGRA */
    QImage qBgraImage = qImage.convertToFormat(QImage::Format_ARGB32);

    /* Convert to vigra equivalent. */
    vigra::BBGRAImage vigraBgraImage(qBgraImage.width(), qBgraImage.height(), reinterpret_cast<const vigra::BBGRAImage::value_type *>(qBgraImage.bits()));
    /* TODO: this copy is redundant ^. */

    /* Convert to vigra. */
    convert(vigraBgraImage, vigraImage);
  }


#ifdef ARX_USE_QT_IMAGE_IO
// -------------------------------------------------------------------------- //
// Image import & export
// -------------------------------------------------------------------------- //
  namespace detail {
    QString toQString(const std::string& s) {
      return QString::fromStdString(s);
    }

    QString toQString(const std::wstring& s) {
      return QString::fromStdWString(s);
    }

  } // namespace detail

  template<class PixelType, class Alloc, class StdString>
  void importImage(vigra::BasicImage<PixelType, Alloc>& vigraImage, const StdString& fileName) {
    QImage qImage;
    qImage.load(detail::toQString(fileName));
    convert(qImage, vigraImage);
  }

  template<class PixelType, class Alloc, class StdString>
  void exportImage(const vigra::BasicImage<PixelType, Alloc>& vigraImage, const StdString& fileName) {
    QImage qImage;
    convert(vigraImage, qImage);
    qImage.save(detail::toQString(fileName));
  }
#endif // ARX_USE_QT_IMAGE_IO

} // namespace arx


// -------------------------------------------------------------------------- //
// Export defined symbols to enable ADL
// -------------------------------------------------------------------------- //
namespace vigra {
  using arx::convert;

#ifdef ARX_USE_QT_IMAGE_IO
  using arx::importImage;
  using arx::exportImage;
#endif // ARX_USE_QT_IMAGE_IO
}

#endif // ARX_EXT_VIGRA_QT_H

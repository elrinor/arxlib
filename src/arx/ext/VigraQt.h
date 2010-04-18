#ifndef __ARX_EXT_VIGRA_QT_H__
#define __ARX_EXT_VIGRA_QT_H__

#include "config.h"
#include <cassert>
#include <cstring> /* for memcpy() */
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
    const vigra::BBGRAImage& vigraRgbaImage = convert_nocopy(vigraImage, placeholder);

    /* Convert to QImage. */
    qImage = QImage(vigraRgbaImage.width(), vigraRgbaImage.height(), QImage::Format_RGB32);

    /* g++ -Wall issues warning on the next line if we don't use static_cast. */
    assert(static_cast<int>(qImage.byteCount()) == static_cast<int>(vigraRgbaImage.width() * vigraRgbaImage.height() * sizeof(vigra::BRGBAImage::value_type))); 
    memcpy(qImage.bits(), vigraRgbaImage.data(), qImage.byteCount());
  }

} // namespace arx


// -------------------------------------------------------------------------- //
// Export defined symbols to enable ADL
// -------------------------------------------------------------------------- //
namespace vigra {
  using arx::convert;
}

#endif // __ARX_EXT_VIGRA_QT_H__

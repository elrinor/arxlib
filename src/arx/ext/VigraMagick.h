#ifndef __ARX_EXT_VIGRA_MAGICK_H__
#define __ARX_EXT_VIGRA_MAGICK_H__

#include "config.h"
#include <boost/mpl/integral_c.hpp>
#include <cassert>
#include <fstream>
#include "Vigra.h"
#include "Magick.h"

namespace arx {
  namespace detail {
// -------------------------------------------------------------------------- //
// magick_storage_type
// -------------------------------------------------------------------------- //
    template<class T> struct magick_storage_type;
    template<> struct magick_storage_type<unsigned char>:  boost::mpl::integral_c<Magick::StorageType, Magick::CharPixel> {};
    template<> struct magick_storage_type<double>:         boost::mpl::integral_c<Magick::StorageType, Magick::DoublePixel> {};
    template<> struct magick_storage_type<float>:          boost::mpl::integral_c<Magick::StorageType, Magick::FloatPixel> {};
    template<> struct magick_storage_type<unsigned int>:   boost::mpl::integral_c<Magick::StorageType, Magick::IntegerPixel> {};
    // template<> struct magick_storage_type<unsigned long>:  boost::mpl::integral_c<Magick::StorageType, Magick::LongPixel> {}; /* MSVC chokes here for no apparent reason. */
    template<> struct magick_storage_type<unsigned short>: boost::mpl::integral_c<Magick::StorageType, Magick::ShortPixel> {};
    
    template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex> 
    struct magick_storage_type<vigra::RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >: magick_storage_type<ChannelType> {};

    template<class ChannelType> 
    struct magick_storage_type<vigra::TinyVector<ChannelType, 4> >: magick_storage_type<ChannelType> {};


// -------------------------------------------------------------------------- //
// magick_order_map
// -------------------------------------------------------------------------- //
    template<class ChannelType> struct magick_order_map                                         { static const char* value() { return "I"; } };
    template<class ChannelType> struct magick_order_map<vigra::RGBValue<ChannelType, 0, 1, 2> > { static const char* value() { return "RGB"; } };
    template<class ChannelType> struct magick_order_map<vigra::RGBValue<ChannelType, 2, 1, 0> > { static const char* value() { return "BGR"; } };
    template<class ChannelType> struct magick_order_map<vigra::TinyVector<ChannelType, 4> >     { static const char* value() { return "RGBA"; } };

  } // namespace detail


// -------------------------------------------------------------------------- //
// Image conversion
// -------------------------------------------------------------------------- //
  template<class PixelType>
  void convert(const vigra::BasicImage<PixelType>& vigraImage, Magick::Image& magickImage) {
    if(vigraImage.width() == 0 || vigraImage.height() == 0) {
      magickImage = Magick::Image();
    } else {
      magickImage.read(
        vigraImage.width(), 
        vigraImage.height(), 
        detail::magick_order_map<PixelType>::value(), 
        detail::magick_storage_type<PixelType>::value, 
        vigraImage.data()
      );
    }
  }

  template<class PixelType>
  void convert(Magick::Image& magickImage, vigra::BasicImage<PixelType>& vigraImage) {
    if(magickImage.columns() == 0 || magickImage.rows() == 0) {
      vigraImage = vigra::BasicImage<PixelType>();
    } else {
      vigraImage.resize(magickImage.columns(), magickImage.rows());
      magickImage.write(
        0,
        0,
        magickImage.columns(),
        magickImage.rows(),
        detail::magick_order_map<PixelType>::value(),
        detail::magick_storage_type<PixelType>::value,
        const_cast<vigra::BasicImage<PixelType>::pointer>(vigraImage.data()) /* We cheat a bit here, but I see no other way that would not degrade performance. */
      );
    }
  }


  namespace detail {
// -------------------------------------------------------------------------- //
// Image import
// -------------------------------------------------------------------------- //
    void importMagickImage(Magick::Image& magickImage, const std::string& fileName) {
      try {
        magickImage.read(fileName);
      } catch (Magick::Warning&) {} /* Ignore warnings. */
    }

#ifdef ARX_WIN32
    void importMagickImage(Magick::Image& magickImage, const std::wstring& fileName) {
      std::ifstream file(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
      file.seekg(0, std::ios_base::end);
      std::ifstream::pos_type fileSize = file.tellg();
      if(fileSize == std::ifstream::pos_type(-1))
        return;

      char* data = new char[fileSize];
      file.seekg(0);
      file.read(data, fileSize);
      Magick::Blob magickBlob(data, fileSize);
      delete[] data;

      try {
        magickImage.read(magickBlob);
      } catch (Magick::Warning&) {} /* Ignore warnings. */
    }
#endif // ARX_WIN32

  } // namespace detail

  template<class PixelType, class StdString>
  void importImage(vigra::BasicImage<PixelType>& vigraImage, const StdString& fileName) {
    Magick::Image magickImage;
    detail::importMagickImage(magickImage, fileName);
    convert(magickImage, vigraImage);
  }


  namespace detail {
// -------------------------------------------------------------------------- //
// Image export
// -------------------------------------------------------------------------- //
    void exportMagickImage(Magick::Image& magickImage, const std::string& fileName) {
      magickImage.write(fileName);
    }

#ifdef ARX_WIN32
    void exportMagickImage(Magick::Image& magickImage, const std::wstring& fileName) {
      Magick::Blob blob;

      std::string shortFileName;
      shortFileName.reserve(fileName.size());
      for(unsigned i = 0; i < fileName.size(); i++)
        shortFileName += static_cast<char>(fileName[i]);
      magickImage.fileName(shortFileName);

      Magick::Blob magickBlob;
      magickImage.write(&magickBlob);

      std::ofstream file(fileName.c_str(), std::ios_base::out | std::ios_base::binary);
      file.write(static_cast<const char*>(magickBlob.data()), magickBlob.length());
    }
#endif // ARX_WIN32

  } // namespace detail

  template<class PixelType, class StdString>
  void exportImage(const vigra::BasicImage<PixelType>& vigraImage, const StdString& fileName) {
    Magick::Image magickImage;
    convert(vigraImage, magickImage);
    detail::exportMagickImage(magickImage, fileName);
  }


// -------------------------------------------------------------------------- //
// Quantization
// -------------------------------------------------------------------------- //
  template<class PixelType>
  void quantize(vigra::BasicImage<PixelType>& vigraImage, unsigned colors) {
    Magick::Image magickImage;
    convert(vigraImage, magickImage);

    magickImage.quantizeColors(colors);
    magickImage.quantizeColorSpace(Magick::RGBColorspace);
    magickImage.quantizeDither(false);
    magickImage.quantize();

    convert(magickImage, vigraImage);
  }

} // namespace arx


// -------------------------------------------------------------------------- //
// Export defined symbols to enable ADL
// -------------------------------------------------------------------------- //
namespace vigra {
  using arx::convert;
  using arx::importImage;
  using arx::exportImage;
  using arx::quantize;
}

namespace Magick {
  using arx::convert;
}


#endif // __ARX_EXT_VIGRA_MAGICK_H__



#ifndef __ARX_EXT_VIGRA_MAGICK_H__
#define __ARX_EXT_VIGRA_MAGICK_H__

#include "config.h"
#include <fstream>
#include "Vigra.h"
#include "Magick.h"

namespace vigra {
  namespace detail {
    void readMagickImage(Magick::Image& magickImage, const std::string& fileName) {
      try {
        magickImage.read(fileName);
      } catch (Magick::Warning&) {} /* Ignore warnings. */
    }

    void writeMagickImage(Magick::Image& magickImage, const std::string& fileName) {
      magickImage.write(fileName);
    }

#ifdef ARX_WIN32
    void readMagickImage(Magick::Image& magickImage, const std::wstring& fileName) {
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

    void writeMagickImage(Magick::Image& magickImage, const std::wstring& fileName) {
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
#endif // _WIN32

// -------------------------------------------------------------------------- //
// ImageImporter
// -------------------------------------------------------------------------- //
    template<class PixelType>
    class ImageImporter {
    public:
      template<class StdString>
      void operator() (BasicImage<PixelType>& vigraImage, const StdString& fileName) {
        BRGBAImage vigraRgbaImage;
        ImageImporter<BRGBAImage::value_type>()(vigraRgbaImage, fileName);

        vigraImage.resize(vigraRgbaImage.width(), vigraRgbaImage.height());
        copyImage(srcImageRange(vigraRgbaImage, RGBAToGrayAccessor<BRGBAImage::value_type>()), destImage(vigraImage));
      }
    };

    template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex>
    class ImageImporter<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> > {
    public:
      template<class StdString>
      void operator() (BasicImage<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >& vigraImage, const StdString& fileName) {
        BRGBAImage vigraRgbaImage;
        ImageImporter<BRGBAImage::value_type>()(vigraRgbaImage, fileName);

        vigraImage.resize(vigraRgbaImage.width(), vigraRgbaImage.height());
        copyImage(srcImageRange(vigraRgbaImage, RGBAToRGBAccessor<BRGBAImage::value_type>()), destImage(vigraImage));
      }
    };

    template<>
    class ImageImporter<BRGBAImage::value_type> {
    public:
      template<class StdString>
      void operator() (BRGBAImage& vigraImage, const StdString& fileName) {
        STATIC_ASSERT((sizeof(Magick::PixelPacket) == sizeof(BRGBAImage::value_type))); /* TODO. */

        Magick::Image magickImage;

        detail::readMagickImage(magickImage, fileName);

        Magick::Pixels view(magickImage);
        Magick::PixelPacket* pixels = view.get(0, 0, magickImage.columns(), magickImage.rows());

        BRGBAImage vigraRgbaImage(magickImage.columns(), magickImage.rows(), reinterpret_cast<BRGBAImage::value_type*>(pixels));
        vigraImage.swap(vigraRgbaImage);
      }
    };


// -------------------------------------------------------------------------- //
// ImageExporter
// -------------------------------------------------------------------------- //
    template<class PixelType>
    class ImageExporter {
    public:
      template<class StdString>
      void operator() (BasicImage<PixelType>& vigraImage, const StdString& fileName) {
        BRGBAImage vigraRgbaImage;
        vigraRgbaImage.resize(vigraImage.width(), vigraImage.height());
        copyImage(srcImageRange(vigraImage, GrayToRGBAAccessor<PixelType>()), destImage(vigraRgbaImage));
        ImageExporter<BRGBAImage::value_type>()(vigraRgbaImage, fileName);
      }
    };

    template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex>
    class ImageExporter<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> > {
    public:
      template<class StdString>
      void operator() (BasicImage<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >& vigraImage, const StdString& fileName) {
        BRGBAImage vigraRgbaImage;
        vigraRgbaImage.resize(vigraImage.width(), vigraImage.height());
        copyImage(srcImageRange(vigraImage, RGBToRGBAAccessor<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >()), destImage(vigraRgbaImage));
        ImageExporter<BRGBAImage::value_type>()(vigraRgbaImage, fileName);
      }
    };

    template<>
    class ImageExporter<BRGBAImage::value_type> {
    public:
      template<class StdString>
      void operator() (BRGBAImage& vigraImage, const StdString& fileName) {
        STATIC_ASSERT((sizeof(Magick::PixelPacket) == sizeof(BRGBAImage::value_type))); /* TODO. */

        Magick::Image magickImage(Magick::Geometry(vigraImage.width(), vigraImage.height()), Magick::Color(0, 0, 0, 0));

        Magick::Pixels view(magickImage);
        Magick::PixelPacket* pixels = view.get(0, 0, vigraImage.width(), vigraImage.height());

        memcpy(pixels, vigraImage.data(), sizeof(BRGBAImage::value_type) * vigraImage.width() * vigraImage.height());

        magickImage.syncPixels();

        detail::writeMagickImage(magickImage, fileName);
      }

    };

  } // namespace detail

  template<class PixelType, class StdString>
  void exportImage(BasicImage<PixelType>& vigraImage, const StdString& fileName) {
    detail::ImageExporter<PixelType>()(vigraImage, fileName);
  }

  template<class PixelType, class StdString>
  void importImage(BasicImage<PixelType>& vigraImage, const StdString& fileName) {
    detail::ImageImporter<PixelType>()(vigraImage, fileName);
  }

} // namespace vigra

#endif // __ARX_EXT_VIGRA_MAGICK_H__

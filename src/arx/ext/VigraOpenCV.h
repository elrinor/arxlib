#ifndef __ARX_EXT_VIGRA_OPENCV_H__
#define __ARX_EXT_VIGRA_OPENCV_H__

#include "config.h"
#include <cassert>
#include <boost/mpl/int.hpp>
#include <arx/Utility.h>
#include "Vigra.h"
#include "OpenCV.h"

namespace arx {
  namespace detail {
// -------------------------------------------------------------------------- //
// opencv_data_type
// -------------------------------------------------------------------------- //
    template<class PixelType> struct opencv_data_type: public boost::mpl::int_<cv::DataDepth<PixelType>::value> {
      STATIC_ASSERT((value != -1));
    };

    template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex> 
    struct opencv_data_type<vigra::RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >: opencv_data_type<ChannelType> {};

    template<class ChannelType> 
    struct opencv_data_type<vigra::TinyVector<ChannelType, 4> >: opencv_data_type<ChannelType> {};


// -------------------------------------------------------------------------- //
// opencv_channels
// -------------------------------------------------------------------------- //
    template<class PixelType> struct opencv_channels: boost::mpl::int_<1> {};

    template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex> 
    struct opencv_channels<vigra::RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >: boost::mpl::int_<3> {};

    template<class ChannelType> 
    struct opencv_data_type<vigra::TinyVector<ChannelType, 4> >: boost::mpl::int_<4> {};
  
  } // namespace detail


// -------------------------------------------------------------------------- //
// Image conversion
// -------------------------------------------------------------------------- //
  template<class PixelType>
  void convert(const vigra::BasicImage<PixelType>& vigraImage, cv::Mat& cvImage) {
    if(vigraImage.width() == 0 || vigraImage.width() == 0) {
      cvImage = cv::Mat();
    } else {
    /* Hmm... cv::Mat constructor only takes memory allocated via cv::fastMalloc(). 
     * Who was the genius behind this? I wonder. */
      int size = sizeof(PixelType) * vigraImage.width() * vigraImage.height();
      void* pixels = cv::fastMalloc(size);
      memcpy(pixels, vigraImage.data(), size);

      cvImage = cv::Mat(
        vigraImage.width(), 
        vigraImage.height(), 
        CV_MAKETYPE(detail::opencv_data_type<PixelType>::value, detail::opencv_channels<PixelType>::value), 
        pixels
      );
    }
  }

  template<class PixelType>
  void convert(const cv::Mat& cvImage, vigra::BasicImage<PixelType>& vigraImage) {
    if(cvImage.rows == 0 || cvImage.cols == 0) { 
      vigraImage = vigra::BasicImage<PixelType>();
    } else {
      int sourceChannels = cvImage.channels();
      int targetChannels = detail::opencv_channels<PixelType>::value;
      int targetType = CV_MAKETYPE(detail::opencv_data_type<PixelType>::value, targetChannels);

      /* Convert channels. */
      cv::Mat cvTmpImage;
      if(sourceChannels == targetChannels) {
        cvTmpImage = cvImage;
      } else {
        /* OpenCV has no support for direct conversion between channel types... Again I ask who was the genius behind this. */
        switch(sourceChannels) {
        case 1: 
          switch(targetChannels) {
          case 3: cv::cvtColor(cvImage, cvTmpImage, CV_GRAY2BGR, targetChannels); break;
          case 4: cv::cvtColor(cvImage, cvTmpImage, CV_GRAY2BGRA, targetChannels); break;
          default: Unreachable();
          }
        case 3:
          switch(targetChannels) {
          case 1: cv::cvtColor(cvImage, cvTmpImage, CV_RGB2GRAY, targetChannels); break;
          case 4: cv::cvtColor(cvImage, cvTmpImage, CV_BGR2BGRA, targetChannels); break;
          default: Unreachable();
          }
        case 4:
          switch(targetChannels) {
          case 1: cv::cvtColor(cvImage, cvTmpImage, CV_RGBA2GRAY, targetChannels); break;
          case 3: cv::cvtColor(cvImage, cvTmpImage, CV_BGRA2BGR, targetChannels); break;
          default: Unreachable();
          }
        default: Unreachable();
        }
      }

      assert(cvTmpImage.channels() == targetChannels);

      /* Convert data type. */
      cv::Mat cvTargetImage;
      cvImage.convertTo(cvTargetImage, targetType); /* TODO: do we need scaling here? */

      /* Copy back. */
      assert(cvTargetImage.flags & cv::Mat::CONTINUOUS_FLAG);
      vigraImage = vigra::BasicImage<PixelType>(
        cvTargetImage.cols, 
        cvTargetImage.rows, 
        reinterpret_cast<vigra::BasicImage<PixelType>::const_pointer>(cvTargetImage.data)
      );
    }
  }


// -------------------------------------------------------------------------- //
// Segmentation
// -------------------------------------------------------------------------- //
  template<class PixelType, class DestPixelType>
  void pyramidSegment(const BasicImage<PixelType>& vigraImage, BasicImage<DestPixelType>& vigraDestImage, int level, double threshold1, double threshold2) {
    cv::Mat cvImage;
    convert(vigraImage, cvImage);

    cv::Mat cvDestImage = cvImage.clone();
    IplImage iplImage = cvImage, iplDestImage = cvDestImage;
    CvMemStorage* storage = cvCreateMemStorage(1024);
    CvSeq* comp = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);
    cvPyrSegmentation(&iplImage, &iplDestImage, storage, &comp, level, threshold1, threshold2);

    convert(cvDestImage, vigraDestImage);
  }

} // namespace arx

// -------------------------------------------------------------------------- //
// Export defined symbols to enable ADL
// -------------------------------------------------------------------------- //
namespace vigra {
  using arx::convert;
}

namespace cv {
  using arx::convert;
}

#endif // __ARX_EXT_VIGRA_OPENCV_H__

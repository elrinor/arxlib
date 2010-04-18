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
    template<class PixelType> struct opencv_data_type: boost::mpl::int_<cv::DataDepth<typename vigra::channel_type<PixelType>::type>::value> {
      STATIC_ASSERT((value != -1));
    };

  } // namespace detail


// -------------------------------------------------------------------------- //
// Image conversion
// -------------------------------------------------------------------------- //
  template<class PixelType, class Alloc>
  void convert(const vigra::BasicImage<PixelType, Alloc>& vigraImage, cv::Mat& cvImage) {
    if(vigraImage.width() == 0 || vigraImage.width() == 0) {
      cvImage = cv::Mat();
    } else {
    /* Hmm... cv::Mat constructor only takes memory allocated via cv::fastMalloc(). 
     * Who was the genius behind this? I wonder. */
      int size = sizeof(PixelType) * vigraImage.width() * vigraImage.height();
      void* pixels = cv::fastMalloc(size);
      memcpy(pixels, vigraImage.data(), size);

      cvImage = cv::Mat(
        vigraImage.height(), 
        vigraImage.width(), 
        CV_MAKETYPE(detail::opencv_data_type<PixelType>::value, vigra::channels<PixelType>::value), 
        pixels
      );
    }
  }

  template<class PixelType, class Alloc>
  void convert(const cv::Mat& cvImage, vigra::BasicImage<PixelType, Alloc>& vigraImage) {
    if(cvImage.rows == 0 || cvImage.cols == 0) { 
      vigraImage = vigra::BasicImage<PixelType, Alloc>();
    } else {
      int sourceChannels = cvImage.channels();
      int targetChannels = vigra::channels<PixelType>::value;
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
      vigraImage = vigra::BasicImage<PixelType, Alloc>(
        cvTargetImage.cols, 
        cvTargetImage.rows, 
        reinterpret_cast<vigra::BasicImage<PixelType, Alloc>::const_pointer>(cvTargetImage.data)
      );
    }
  }


// -------------------------------------------------------------------------- //
// Clusterization
// -------------------------------------------------------------------------- //
  /**
   * @param vigraImage                 source image.
   * @param vigraDestImage             destination image.
   * @param level                      maximal level of the pyramid for segmentation, source image size must be a multiple of level.
   * @param linkThreshold              threshold for establishing links.
   * @param clusterThreshold           threshold for the segments clustering.
   */
  template<class SrcPixelType, class SrcAlloc, class DstPixelType, class DstAlloc>
  void pyramidClusterize(const vigra::BasicImage<SrcPixelType, SrcAlloc>& vigraImage, vigra::BasicImage<DstPixelType, DstAlloc>& vigraDestImage, int level, double linkThreshold, double clusterThreshold) {
    cv::Mat cvImage;
    convert(vigraImage, cvImage);

    cv::Mat cvDestImage = cvImage.clone();
    IplImage iplImage = cvImage, iplDestImage = cvDestImage;
    CvMemStorage* storage = cvCreateMemStorage(1024);
    CvSeq* comp = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvPoint), storage);

    ARX_OPENCV_CALL(cvPyrSegmentation(&iplImage, &iplDestImage, storage, &comp, level, linkThreshold, clusterThreshold));

    convert(cvDestImage, vigraDestImage);
  }

} // namespace arx


// -------------------------------------------------------------------------- //
// Export defined symbols to enable ADL
// -------------------------------------------------------------------------- //
namespace vigra {
  using arx::convert;
  using arx::pyramidClusterize;
}

namespace cv {
  using arx::convert;
}

#endif // __ARX_EXT_VIGRA_OPENCV_H__

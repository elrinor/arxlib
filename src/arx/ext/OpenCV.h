#ifndef ARX_EXT_OPENCV_H
#define ARX_EXT_OPENCV_H

#include "config.h"
#include <exception>
#include <boost/lexical_cast.hpp>
#include <opencv/cv.h>

namespace arx {
// -------------------------------------------------------------------------- //
// opencv_exception
// -------------------------------------------------------------------------- //
  class opencv_exception: public std::runtime_error {
  public:
    opencv_exception(const cv::Exception& exception): mException(exception), 
      std::runtime_error(
        exception.err + " " +
        "(error code " + boost::lexical_cast<std::string>(exception.code) + ") in " + 
        (exception.func.empty() ? "" : exception.func + ", ") +
        "file " + exception.file + ", "
        "line " + boost::lexical_cast<std::string>(exception.line)
      ) {}

    int code() const {
      return mException.code;
    }

    const std::string& function() const {
      return mException.func;
    }

    const std::string& file() const {
      return mException.file;
    }

    int line() const {
      return mException.line;
    }

    const std::string& message() const {
      return mException.err;
    }

  private:
    cv::Exception mException;
  };


  /** @def ARX_OPENCV_CALL
   * 
   * Wraps an OpenCV call translating OpenCV exceptions into std-compatible
   * arx::opencv_exception.
   *
   * @param CALL_EXPRESSION            OpenCV function call expression. */
#define ARX_OPENCV_CALL(CALL_EXPRESSION)                                        \
  ARX_TRY {                                                                     \
    CALL_EXPRESSION;                                                            \
  } ARX_CATCH(cv::Exception& e) {                                               \
    ARX_THROW(arx::opencv_exception(e));                                        \
  }

} // namespace arx

// -------------------------------------------------------------------------- //
// Inter-lib defines
// -------------------------------------------------------------------------- //
#ifndef ARX_USE_OPENCV
#  define ARX_USE_OPENCV
#endif

#ifdef ARX_USE_VIGRA
#  include "VigraOpenCV.h"
#endif

#endif // ARX_EXT_OPENCV_H

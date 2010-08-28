#include "XmlDataBinder.h"

namespace arx {
// -------------------------------------------------------------------------- //
// XmlDataBinder::Impl
// -------------------------------------------------------------------------- //
  class XmlDataBinder::Impl: public XmlDataBinder {
  public:


  };


// -------------------------------------------------------------------------- //
// XmlDataBinder
// -------------------------------------------------------------------------- //
  XmlDataBinder::Impl *XmlDataBinder::sInstance = NULL;

  XmlDataBinder *XmlDataBinder::instance() {
    /* This method is supposed to be accessed during initialization at 
     * least once => no need for synchronization. */

    if(sInstance == NULL)
      sInstance = new Impl();
    return sInstance;
  }

  void XmlDataBinder::destroyInstance() {
    if(sInstance != NULL) {
      delete sInstance;
      sInstance = NULL;
    }
  }


  namespace {
    ARX_STATIC_BLOCK(XML_DATA_BINDER_CPP) {
      /* Construct instance at initialization time. */
      XmlDataBinder::instance();
    }
  
  } // namespace `anonymous-namespace`

} // namespace arx

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
#ifndef ARX_EXT_QT_FORM_GENERATOR_H
#define ARX_EXT_QT_FORM_GENERATOR_H

#include "config.h"
#include <algorithm> /* for std::swap(). */
#include "FormPrinter.h"

namespace arx {
// -------------------------------------------------------------------------- //
// FormGenerator
// -------------------------------------------------------------------------- //
  /* TODO: use virtual functions to simplify things. */
  template<class Derived>
  class FormGenerator: public FormPrintWorker {
  public:
    FormGenerator(int numPages, QPrinter::Orientation desiredOrientation, int desiredWidth, int desiredHeight): 
      FormPrintWorker(numPages), mDesiredOrientation(desiredOrientation), mDesiredWidth(desiredWidth), mDesiredHeight(desiredHeight) {
      assert(mDesiredHeight > 0 && mDesiredWidth > 0);
    }

    virtual void operator() (QPrinter& printer, QPainter& painter) {
      /* Compute actual sizes. */
      int actualWidth = mDesiredWidth, actualHeight = mDesiredHeight;
      if(printer.orientation() != mDesiredOrientation)
        std::swap(actualWidth, actualHeight);

      /* Get paper size & compute margins. */
      QRectF pageRect = printer.pageRect(QPrinter::Millimeter);
      qreal hMargin = (pageRect.width()  * 1000.0 - actualWidth ) / 2;
      qreal vMargin = (pageRect.height() * 1000.0 - actualHeight) / 2;

      /* Set up transformation. */
      painter.scale(printer.resolution() / 25400.0, printer.resolution() / 25400.0); /* Convert to microns. */
      painter.translate(hMargin, vMargin); /* Add margins. */
      if(printer.orientation() != mDesiredOrientation) { /* Rotate if needed. */
        painter.translate(0, actualHeight);
        painter.setTransform(QTransform(0, -1, 1, 0, 0, 0), true);
      }

      /* Draw! */
      for(int i = 0; i < numPages(); i++) {
        if(i != 0)
          printer.newPage();

        derived().drawPage(painter, i);

        emit advanced(1);
      }
    }

  protected:
    void drawPage(QPainter&, int);

    int width() const {
      return mDesiredWidth;
    }

    int height() const {
      return mDesiredHeight;
    }

  private:
    Derived& derived() {
      return *static_cast<Derived*>(this);
    }

    const Derived& derived() const {
      return *static_cast<Derived*>(this);
    }

    QPrinter::Orientation mDesiredOrientation;
    int mDesiredWidth, mDesiredHeight;
  };

} // namespace arx

#endif // ARX_EXT_QT_FORM_GENERATOR_H

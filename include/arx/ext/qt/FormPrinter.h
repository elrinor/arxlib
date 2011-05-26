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
#ifndef ARX_EXT_QT_FORM_PRINTER_H
#define ARX_EXT_QT_FORM_PRINTER_H

#include <arx/config.h>
#include <cassert>
#include <QObject>
#include <QPrinter>
#include <QPainter>

namespace arx {
// -------------------------------------------------------------------------- //
// FormPrintWorker
// -------------------------------------------------------------------------- //
  class FormPrintWorker: public QObject {
    Q_OBJECT;
  public:
    FormPrintWorker(int numPages): mNumPages(numPages), mFormPrinter(NULL) {}

    virtual void operator() (QPrinter&, QPainter&) = 0;

    int numPages() const {
      return mNumPages;
    }

  protected:
    int totalPages() const;

    int currentPage() const;

  Q_SIGNALS:
    void advanced(int amount);

  private:
    friend class FormPrinter;

    int mNumPages;
    FormPrinter* mFormPrinter;
  };


// -------------------------------------------------------------------------- //
// FormPrinter
// -------------------------------------------------------------------------- //
  class FormPrinter: public QObject {
    Q_OBJECT;
  public:
    FormPrinter(QPrinter& printer): mPrinter(printer), mTotalPages(0), mCurrentPage(0) {}

    FormPrinter& operator() (FormPrintWorker* worker) {
      assert(worker->mFormPrinter == NULL);
      
      worker->mFormPrinter = this;
      connect(worker, SIGNAL(advanced(int)), this, SLOT(advance(int)));
      worker->setParent(this);
      mWorkers.append(worker);
      mTotalPages += worker->numPages();

      return *this;
    }

    typedef void result_type;

    void operator() () {
      /* Init. */
      mCurrentPage = 0;
      Q_EMIT started(mTotalPages);

      /* Set up painter. */
      QPainter painter;
      painter.begin(&mPrinter);

      /* Draw! */
      for(int i = 0; i < mWorkers.size(); i++) {
        if(i != 0)
          mPrinter.newPage();
        painter.resetTransform();
        mWorkers[i]->operator() (mPrinter, painter);
      }

      /* Clean up. */
      painter.end();
      Q_EMIT finished();
    }

    int totalPages() const {
      return mTotalPages;
    }

    int currentPage() const {
      return mCurrentPage;
    }

  Q_SIGNALS:
    void started(int maximum);
    void progressed(int current);
    void finished();

  private Q_SLOTS:
    void advance(int amount) {
      mCurrentPage += amount;
      Q_EMIT progressed(mCurrentPage);
    }

  private:
    QList<FormPrintWorker*> mWorkers;
    QPrinter& mPrinter;
    int mTotalPages, mCurrentPage;
    volatile bool mIsCancelled;
  };


// -------------------------------------------------------------------------- //
// FormPrintWorker
// -------------------------------------------------------------------------- //
  inline int FormPrintWorker::totalPages() const {
    assert(mFormPrinter != NULL);

    return mFormPrinter->totalPages();
  }

  inline int FormPrintWorker::currentPage() const {
    assert(mFormPrinter != NULL);

    return mFormPrinter->currentPage();
  }

} // namespace arx

#endif // ARX_EXT_QT_FORM_PRINTER_H

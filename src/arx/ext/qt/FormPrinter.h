#ifndef __ARX_EXT_QT_FORM_PRINTER_H__
#define __ARX_EXT_QT_FORM_PRINTER_H__

#include "config.h"
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
    FormPrintWorker(int numPages): mNumPages(numPages) {}

    virtual void operator() (QPrinter&, QPainter&) = 0;

    int numPages() const {
      return mNumPages;
    }

  Q_SIGNALS:
    void advanced(int amount);

  private:
    int mNumPages;
  };


// -------------------------------------------------------------------------- //
// FormPrinter
// -------------------------------------------------------------------------- //
  class FormPrinter: public QObject {
    Q_OBJECT;
  public:
    FormPrinter(QPrinter& printer): mPrinter(printer), mTotalPages(0), mCurrentPages(0) {}

    FormPrinter& operator() (FormPrintWorker* worker) {
      connect(worker, SIGNAL(advanced(int)), this, SLOT(advance(int)));
      worker->setParent(this);
      mWorkers.append(worker);
      mTotalPages += worker->numPages();

      return *this;
    }

    typedef void result_type;

    void operator() () {
      /* Init. */
      mCurrentPages = 0;
      Q_EMIT started(mTotalPages);

      /* Set up painter. */
      QPainter painter;
      painter.begin(&mPrinter);

      /* Draw! */
      for(int i = 0; i < mWorkers.size(); i++ ) {
        if(i != 0)
          mPrinter.newPage();
        painter.resetTransform();
        mWorkers[i]->operator() (mPrinter, painter);
      }

      /* Clean up. */
      painter.end();
      Q_EMIT finished();
    }

  Q_SIGNALS:
    void started(int maximum);
    void progressed(int current);
    void finished();

  private Q_SLOTS:
    void advance(int amount) {
      mTotalPages += amount;
      Q_EMIT progressed(mCurrentPages);
    }

  private:
    QList<FormPrintWorker*> mWorkers;
    QPrinter& mPrinter;
    int mTotalPages, mCurrentPages;
    volatile bool mIsCancelled;
  };

} // namespace arx

#endif // __ARX_EXT_QT_FORM_PRINTER_H__

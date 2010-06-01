#ifndef ARX_EXT_QT_FORM_PRINTER_H
#define ARX_EXT_QT_FORM_PRINTER_H

#include "config.h"
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

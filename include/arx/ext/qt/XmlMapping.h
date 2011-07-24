/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2011 Alexander Fokin <apfokin@gmail.com>
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
#ifndef ARX_EXT_QT_XML_MAPPING_H
#define ARX_EXT_QT_XML_MAPPING_H

#include <arx/config.h>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QCoreApplication> /* For Q_DECLARE_TR_FUNCTIONS. */
#include "XmlMappingFwd.h"

namespace arx { namespace xml {
  namespace detail {
    template<class Derived>
    class Mapper {
    public:
      template<class T>
      void mapElement(const QString &name, T &&value) {
        derived().startElement(name);
        derived().map(std::forward<T>(value));
        derived().endElement();
      }

      template<class T>
      void mapAttribute(const QString &name, T &&value) {
        derived().startAttribute(name);
        derived().map(std::forward<T>(value));
        derived().endAttribute();
      }

      template<class Function>
      void attribute(const QString &name, const Function &function) {
        derived().startAttribute(name);
        function();
        derived().endAttribute();
      }

      template<class Function>
      void element(const QString &name, const Function &function) {
        derived().startElement(name);
        function();
        derived().endElement();
      }

    private:
      Derived &derived() {
        return *static_cast<Derived *>(this);
      }
    };

  } // namespace detail


// -------------------------------------------------------------------------- //
// Serializer
// -------------------------------------------------------------------------- //
  class Serializer: public detail::Mapper<Serializer> {
  public:
    Serializer(QXmlStreamWriter &writer): 
      mAfterText(false), 
      mWriter(writer) 
    {}

    void startElement(const QString &name) {
#ifndef NDEBUG
      if(mAfterText) {
        qWarning("Serializer::startElement: interlaced text and xml elements are not supported");
        mAfterText = false;
      }
#endif

      mWriter.writeStartElement(name);
    }

    void endElement() {
      mWriter.writeEndElement();
      mAfterText = false;
    }

    void startAttribute(const QString &name) {
#ifndef NDEBUG
      if(mAfterText) {
        qWarning("Serializer::startAttribute: interlaced text and xml attributes are not supported");
        mAfterText = false;
      }
#endif

      mAttribute = name;
    }

    void endAttribute() {
      mAttribute = QString();
      mAfterText = false;
    }

    void text(const QString &text) {
#ifndef NDEBUG
      if(mAfterText) {
        qWarning("Serializer::text: text is already set");
        return;
      }
#endif

      if(!mAttribute.isNull()) {
        mWriter.writeAttribute(mAttribute, text);
      } else {
        mWriter.writeCharacters(text);
      }

      mAfterText = true;
    }

    QString text() {
      qWarning("Serializer::text(): is expected never to be called");
      
      return QString();
    }

    template<class T>
    void map(const T &value) {
      serialize(*this, value);
    }

    void handleError(const QString &) const {
      qWarning("Serializer::handleError: is expected never to be called");
    }

    bool hasErrors() const {
      return false;
    }

    bool isSerializing() const {
      return true;
    }

    bool isDeserializing() const {
      return false;
    }

  private:
    bool mAfterText;
    QString mAttribute;
    QXmlStreamWriter &mWriter;
  };


// -------------------------------------------------------------------------- //
// ErrorHandler
// -------------------------------------------------------------------------- //
  class ErrorHandler {
  public:
    virtual void handleError(const QXmlStreamReader &reader, const QString &message) = 0;
  };

  class DefaultErrorHandler: public ErrorHandler {
  public:
    virtual void handleError(const QXmlStreamReader &, const QString &) {}
  };


// -------------------------------------------------------------------------- //
// Deserializer
// -------------------------------------------------------------------------- //
  class Deserializer: public detail::Mapper<Deserializer> {
    Q_DECLARE_TR_FUNCTIONS(Deserializer);
  public:
    Deserializer(QXmlStreamReader &reader, ErrorHandler &errorHandler): 
      mHasErrors(false), 
      mSkipToken(false),
      mReader(reader),
      mErrorHandler(errorHandler) 
    {}

    QStringRef startElement() {
      if(!expect(QXmlStreamReader::StartElement, QT_TR_NOOP("Expected element start, got \"%1\"")))
        return QStringRef();

      /* Skip start element token. */
      mSkipToken = true;

      return mReader.name();
    }

    void startElement(const QString &name) {
      if(startElement() != name)
        handleError(tr("Expected \"%1\" xml element, got \"%2\"").arg(name).arg(mReader.name().toString()));
    }

    void endElement() {
      mText = QString();

      if(!expect(QXmlStreamReader::EndElement, QT_TR_NOOP("Expected element end, got \"%1\"")))
        return;

      /* Skip end element token. */
      readNext(); 
    }

    bool hasElement() {
      skipWhitespace();

      return mReader.tokenType() == QXmlStreamReader::StartElement;
    }

    void startAttribute(const QString &name) {
      mAttribute = name;
    }

    void endAttribute() {
      mAttribute = QString();
    }

    void text(QString &text) {
#ifndef NDEBUG
      if(mReader.tokenType() != QXmlStreamReader::StartElement)
        qWarning("Deserializer::text: must be at element start to request attribute text");
#endif

      if(!mAttribute.isNull()) {
        QStringRef result = mReader.attributes().value(mAttribute);
        if(result.isNull())
          handleError(tr("Attribute \"%1\" not found").arg(mAttribute));
        text = result.toString();
      } else {
        if(mText.isNull())
          mText = readElementText();
        text = mText;
      }
    }

    QString text() {
      QString result;
      text(result);
      return result;
    }

    template<class T>
    void map(T &value) {
      deserialize(*this, value);
    }

    template<class T>
    void map(const T &) {
      qWarning("Deserializer::map(const T &): is expected never to be called");
    }

    bool hasErrors() const {
      return mHasErrors;
    }

    void handleError(const QString &message) {
      mHasErrors = true;

      mErrorHandler.handleError(mReader, message);
    }

    bool isSerializing() const {
      return false;
    }

    bool isDeserializing() const {
      return true;
    }

  protected:
    void handleReaderError() {
      switch(mReader.error()) {
      case QXmlStreamReader::NotWellFormedError:
        handleError(tr("Xml document is not well-formatted"));
      case QXmlStreamReader::PrematureEndOfDocumentError:
        handleError(tr("Xml document ended prematurely"));
      default:
        handleError(mReader.errorString());
      }
    }

    bool atWhiteSpace() {
      switch(tokenType()) {
      case QXmlStreamReader::NoToken:
      case QXmlStreamReader::StartDocument:
      case QXmlStreamReader::EndDocument:
      case QXmlStreamReader::Comment:
      case QXmlStreamReader::ProcessingInstruction:
        return true;
      case QXmlStreamReader::Characters:
        return mReader.isWhitespace();
      default:
        return false;
      }
    }

    void skipWhitespace() {
      while(atWhiteSpace())
        readNext();
    }

    bool expect(QXmlStreamReader::TokenType token, const char *errorString) {
      while(true) {
        skipWhitespace();

        if(tokenType() == token)
          return true;

        if(mReader.hasError()) {
          handleReaderError();
          return false;
        } 

        handleError(tr(errorString).arg(mReader.tokenString()));
        readNext();
      }
    }

    QString readElementText() {
      QString result;

      while(true) {
        switch (tokenType()) {
        case QXmlStreamReader::Characters:
        case QXmlStreamReader::EntityReference:
          result += mReader.text();
          break;
        case QXmlStreamReader::EndElement:
          return result;
        case QXmlStreamReader::ProcessingInstruction:
        case QXmlStreamReader::Comment:
          break;
        case QXmlStreamReader::StartElement:
          result += readElementText();

          /* Skip element end. */
          readNext(); 
          break;
        default:
          handleReaderError();
          return result;
        }

        readNext();
      }
    }

    QXmlStreamReader::TokenType tokenType() {
      if(mSkipToken) {
        mReader.readNext();
        mSkipToken = false;
      }

      return mReader.tokenType();
    }

    void readNext() {
      if(mSkipToken) {
        mReader.readNext();
        mSkipToken = false;
      }

      mReader.readNext();
    }

  private:
    bool mHasErrors;
    bool mSkipToken;
    QString mText;
    QString mAttribute;
    QXmlStreamReader &mReader;
    ErrorHandler &mErrorHandler;
  };

// -------------------------------------------------------------------------- //
// Interface functions
// -------------------------------------------------------------------------- //
  template<class T>
  void serialize(Serializer &serializer, const T &value) {
    xml_serialize(serializer, value);
  }

  template<class T>
  void serialize(QXmlStreamWriter &writer, const T &value) {
    Serializer serializer(writer);
    xml_serialize(serializer, value);
  }

  template<class T>
  bool deserialize(Deserializer &deserializer, T &value) {
    xml_deserialize(deserializer, value);
    return !deserializer.hasErrors();
  }

  template<class T>
  bool deserialize(QXmlStreamReader &reader, T &value) {
    DefaultErrorHandler errorHandler;
    Deserializer deserializer(reader, errorHandler);
    xml_deserialize(deserializer, value);
    return !deserializer.hasErrors();
  }

  template<class T>
  bool deserialize(QXmlStreamReader &reader, ErrorHandler &errorHandler, T &value) {
    Deserializer deserializer(reader, errorHandler);
    xml_deserialize(deserializer, value);
    return !deserializer.hasErrors();
  }

}} // namespace arx::xml

#define ARX_XML_DEFINE_MAPPING_FUNCTIONS(MAPPING_FUNCTION, ... /* TYPE */)      \
  void xml_serialize(arx::xml::Serializer &serializer, const __VA_ARGS__ &value) { \
    MAPPING_FUNCTION(serializer, const_cast<__VA_ARGS__ &>(value));             \
  }                                                                             \
                                                                                \
  void xml_deserialize(arx::xml::Deserializer &deserializer, __VA_ARGS__ &value) { \
    MAPPING_FUNCTION(deserializer, value);                                      \
  }                                                                             \

#endif // ARX_EXT_QT_XML_MAPPING_H

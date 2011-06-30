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

    template<class T>
    void map(const T &value) {
      serialize(*this, value);
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
      mReader(reader),
      mErrorHandler(errorHandler) 
    {}

    void startElement(const QString &name) {
      if(!expect(QXmlStreamReader::StartElement, QT_TR_NOOP("Expected element start, got \"%1\"")))
        return;

      if(mReader.name() != name)
        handleError(mReader, tr("Expected \"%1\" xml element, got \"%2\"").arg(name).arg(mReader.name().toString()));

      /* Skip start element token. */
      mReader.readNext();
    }

    void endElement() {
      mText = QString();

      if(!expect(QXmlStreamReader::EndElement, QT_TR_NOOP("Expected element end, got \"%1\"")))
        return;

      /* Skip end element token. */
      mReader.readNext(); 
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
      if(!mAttribute.isNull()) {
        QStringRef result = mReader.attributes().value(mAttribute);
        if(result.isNull())
          handleError(mReader, tr("Attribute \"%1\" not found").arg(mAttribute));
        text = result.toString();
      } else {
        if(mText.isNull())
          mText = readElementText();
        text = mText;
      }
    }

    template<class T>
    void map(T &value) {
      deserialize(*this, value);
    }

    bool hasErrors() const {
      return mHasErrors;
    }

  protected:
    void handleError(const QXmlStreamReader &reader, const QString &message) {
      mHasErrors = true;

      mErrorHandler.handleError(reader, message);
    }

    void handleReaderError() {
      switch(mReader.error()) {
      case QXmlStreamReader::NotWellFormedError:
        handleError(mReader, tr("Xml document is not well-formatted"));
      case QXmlStreamReader::PrematureEndOfDocumentError:
        handleError(mReader, tr("Xml document ended prematurely"));
      default:
        handleError(mReader, mReader.errorString());
      }
    }

    bool atWhiteSpace() {
      switch(mReader.tokenType()) {
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
        mReader.readNext();
    }

    bool expect(QXmlStreamReader::TokenType tokenType, const char *errorString) {
      while(true) {
        skipWhitespace();

        if(mReader.tokenType() == tokenType)
          return true;

        if(mReader.hasError()) {
          handleReaderError();
          return false;
        } 

        handleError(mReader, tr(errorString).arg(mReader.tokenString()));
        mReader.readNext();
      }
    }
    
    QString readElementText() {
      QString result;

      while(true) {
        switch (mReader.tokenType()) {
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
          break;
        default:
          handleError(mReader, tr("Expected element end, got \"%1\"").arg(mReader.tokenString()));
          return result;
        }

        mReader.readNext();
      }
    }

  private:
    bool mHasErrors;
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
    return xml_deserialize(deserializer, value);
  }

  template<class T>
  bool deserialize(QXmlStreamReader &reader, T &value) {
    DefaultErrorHandler errorHandler;
    Deserializer deserializer(reader, errorHandler);
    return xml_deserialize(deserializer, value);
  }

  template<class T>
  bool deserialize(QXmlStreamReader &reader, ErrorHandler &errorHandler, T &value) {
    Deserializer deserializer(reader, errorHandler);
    return xml_deserialize(deserializer, value);
  }

}} // namespace arx::xml

#define ARX_DECLARE_XML_MAPPING_FUNCTIONS(... /* TYPE */)                       \
  void xml_serialize(arx::xml::Serializer &serializer, const __VA_ARGS__ &value); \
  bool xml_deserialize(arx::xml::Deserializer &deserializer, __VA_ARGS__ &value); \

#define ARX_DEFINE_XML_MAPPING_FUNCTIONS(MAPPING_FUNCTION, ... /* TYPE */)      \
  void xml_serialize(arx::xml::Serializer &serializer, const __VA_ARGS__ &value) { \
    MAPPING_FUNCTION(serializer, const_cast<__VA_ARGS__ &>(value));             \
  }                                                                             \
                                                                                \
  bool xml_deserialize(arx::xml::Deserializer &deserializer, __VA_ARGS__ &value) { \
    MAPPING_FUNCTION(deserializer, value);                                      \
    return deserializer.hasErrors();                                            \
  }                                                                             \

#endif // ARX_EXT_QT_XML_MAPPING_H

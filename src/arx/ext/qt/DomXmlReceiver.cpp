#include <arx/ext/qt/DomXmlReceiver.h>
#include <cassert>
#include <QDomNode>
#include <QXmlQuery>

namespace arx {
  DomXmlReceiver::DomXmlReceiver(const QXmlQuery &query, QDomNode *outNode): mQuery(query) {
    mNodeStack.push_back(*outNode);
  }

  QDomNode DomXmlReceiver::appendChild(QDomNode &node) {
    return mNodeStack.back().appendChild(node);
  }

  QDomDocument DomXmlReceiver::document() {
    assert(mNodeStack.size() > 0);

    return mNodeStack.front().ownerDocument(); /* TODO: what is the return value of ownerDocument() for QDomDocument? */
  }


  void DomXmlReceiver::atomicValue(const QVariant &value) {
    /* Process atomic values as text nodes. */
    
    appendChild(document().createTextNode(value.toString()));
  }

  void DomXmlReceiver::attribute(const QXmlName &name, const QStringRef &value) {
    QDomAttr attribute = document().createAttribute(name.localName(mQuery.namePool()));
    attribute.setPrefix(name.prefix(mQuery.namePool()));
    attribute.setValue(value.toString());

    mNodeStack.back().toElement().setAttributeNode(attribute);
  }

  void DomXmlReceiver::characters(const QStringRef &value) {
    appendChild(document().createTextNode(value.toString()));
  }

  void DomXmlReceiver::comment(const QString &value) {
    appendChild(document().createComment(value));
  }

  void DomXmlReceiver::endDocument() {
    /* Skip. */
  }

  void DomXmlReceiver::endElement() {
    mNodeStack.pop_back();
  }

  void DomXmlReceiver::endOfSequence() {
    /* Skip. */
  }

  void DomXmlReceiver::namespaceBinding(const QXmlName &name) {
    QDomAttr attribute = document().createAttribute("xmlns");
    attribute.setPrefix(name.prefix(mQuery.namePool()));
    attribute.setValue(name.namespaceUri(mQuery.namePool()));

    mNodeStack.back().toElement().setAttributeNode(attribute);
  }

  void DomXmlReceiver::processingInstruction(const QXmlName &target, const QString &value) {
    appendChild(document().createProcessingInstruction(target.localName(mQuery.namePool()), value));
  }

  void DomXmlReceiver::startDocument() {
    /* Skip. */
  }

  void DomXmlReceiver::startElement(const QXmlName &name) {
    QDomElement element = appendChild(document().createElementNS(name.namespaceUri(mQuery.namePool()), name.localName(mQuery.namePool()))).toElement();
    element.setPrefix(name.prefix(mQuery.namePool()));

    mNodeStack.push_back(element);
  }

  void DomXmlReceiver::startOfSequence() {
    /* Skip. */
  }

} // namespace arx

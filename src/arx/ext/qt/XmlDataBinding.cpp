#include "XmlDataBinding.h"
#include <QString>
#include <QList>
#include <arx/Foreach.h>

namespace arx {
  namespace detail {
// -------------------------------------------------------------------------- //
// XmlDataBindingAction
// -------------------------------------------------------------------------- //
    class XmlDataBindingAction: public boost::noncopyable {
    public:
      virtual ~XmlDataBindingAction() {}

      virtual void serialize(const XmlDataBindingContext &ctx, const void *value, QDomNode *target) const = 0;

      virtual void deserialize(const XmlDataBindingContext &ctx, QDomNode &source, void *value) const = 0;
    };


// -------------------------------------------------------------------------- //
// XmlDataBindingSingleAction
// -------------------------------------------------------------------------- //
    class XmlDataBindingSingleAction: public XmlDataBindingAction {
    public:
      typedef XmlDataBinding::Setter Setter;
      typedef XmlDataBinding::Getter Getter;

      XmlDataBindingSingleAction(const QString &path, const Getter &getter, const Setter &setter):
        mPath(path), mSetter(setter), mGetter(getter) {}

      virtual void serialize(const XmlDataBindingContext &ctx, const void *value, QDomNode *target) const {
        mGetter(ctx, value, target);
      }
      
      virtual void deserialize(const XmlDataBindingContext &ctx, QDomNode &source, void *value) const {
        mSetter(ctx, source, value);
      }

    private:
      QString mPath;
      Setter mSetter;
      Getter mGetter;
    };


// -------------------------------------------------------------------------- //
// XmlDataBindingActionList
// -------------------------------------------------------------------------- //
    class XmlDataBindingActionList: public XmlDataBindingAction {
    public:
      virtual void serialize(const XmlDataBindingContext &ctx, const void *value, QDomNode *target) const {
        foreach(XmlDataBindingAction *action, mActions)
          action->serialize(ctx, value, target);
      }

      virtual void deserialize(const XmlDataBindingContext &ctx, QDomNode &source, void *value) const {
        foreach(XmlDataBindingAction *action, mActions)
          action->deserialize(ctx, source, value);
      }

      void add(XmlDataBindingAction *action) {
        mActions.push_back(action);
      }

      virtual ~XmlDataBindingActionList() {
        foreach(XmlDataBindingAction *action, mActions)
          delete action;
      }
      
    private:
      QList<XmlDataBindingAction *> mActions;
    };


// -------------------------------------------------------------------------- //
// XmlDataBindingIfAction
// -------------------------------------------------------------------------- //



// -------------------------------------------------------------------------- //
// XmlDataBindingBlock
// -------------------------------------------------------------------------- //
    class XmlDataBindingBlock: public boost::noncopyable {
    public:
      typedef XmlDataBinding::Condition Condition;

    private:
      Condition mCondition;
      

    };

  } // namespace detail


// -------------------------------------------------------------------------- //
// XmlDataBinding
// -------------------------------------------------------------------------- //
  XmlDataBinding::XmlDataBinding(const std::type_info &typeInfo, const Checker &checker):
    mTypeInfo(typeInfo), mChecker(checker) 
  {

  }

  const std::type_info &XmlDataBinding::typeInfo() const {
    return mTypeInfo;
  }

  XmlDataBinding &XmlDataBinding::operator() (const QString &path, const Setter &setter, const Getter &getter) {

  }

  XmlDataBinding &XmlDataBinding::if_(const Condition &cond);
  XmlDataBinding &XmlDataBinding::else_();
  XmlDataBinding &XmlDataBinding::fi_();


} // namespace arx

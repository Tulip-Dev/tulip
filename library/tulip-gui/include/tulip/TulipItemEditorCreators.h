#ifndef TULIPITEMEDITORCREATORS_H
#define TULIPITEMEDITORCREATORS_H

#include <QtCore/QObject>
#include <QtGui/QItemEditorCreatorBase>
#include <QtGui/QStyleOptionViewItem>
#include <QtGui/QPainter>

#include <tulip/tulipconf.h>
#include <tulip/PropertyTypes.h>
#include <tulip/TulipMetaTypes.h>

namespace tlp {

class TLP_QT_SCOPE TulipItemEditorCreator {
public:
  virtual QWidget* createWidget(QWidget *parent) const=0;
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant &) const {
    return false;
  }
  virtual QString displayText(const QVariant &) const {
    return "";
  }

  virtual void setEditorData(QWidget* editor, const QVariant &data)=0;
  virtual QVariant editorData(QWidget* editor)=0;
};

template<typename T>
class TLP_QT_SCOPE StringDisplayEditorCreator: public TulipItemEditorCreator {
public:
  inline QString displayText(const QVariant &v) const {
    return T::toString(v.value<typename T::RealType>()).c_str();
  }
};

class TLP_QT_SCOPE ColorEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget *parent) const;
  bool paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant &v) const;
  virtual void setEditorData(QWidget* editor, const QVariant &data);
  virtual QVariant editorData(QWidget* editor);
};

}

#endif // TULIPITEMEDITORCREATORS_H

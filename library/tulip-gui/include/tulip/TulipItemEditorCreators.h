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

class TLP_QT_SCOPE TulipItemEditorCreator: public QItemEditorCreatorBase {
public:
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant &) const { return false; }
  virtual QString displayText(const QVariant &) const { return ""; }
};

template<typename T>
class TLP_QT_SCOPE StringDisplayEditorCreator: public TulipItemEditorCreator {
public:
  inline QString displayText(const QVariant &v) const { return T::toString(v.value<typename T::RealType>()).c_str(); }
};

class TLP_QT_SCOPE ColorEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *parent) const;
  QByteArray valuePropertyName() const;
  bool paint(QPainter* painter, const QStyleOptionViewItem& option, const QVariant &v) const;
};

}

#endif // TULIPITEMEDITORCREATORS_H

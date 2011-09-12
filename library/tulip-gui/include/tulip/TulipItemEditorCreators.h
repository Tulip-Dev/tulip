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
  virtual QWidget* createWidget(QWidget*) const=0;
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const {
    return false;
  }
  virtual QString displayText(const QVariant &) const {
    return "";
  }

  virtual void setEditorData(QWidget*,const QVariant&,tlp::Graph* g=NULL)=0;
  virtual QVariant editorData(QWidget*,tlp::Graph* g=NULL)=0;
};

template<typename T>
class TLP_QT_SCOPE StringDisplayEditorCreator: public TulipItemEditorCreator {
public:
  inline QString displayText(const QVariant& v) const {
    return T::toString(v.value<typename T::RealType>()).c_str();
  }
};

class TLP_QT_SCOPE ColorEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
  virtual void setEditorData(QWidget*, const QVariant&,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
};

template<typename T>
class TLP_QT_SCOPE LineEditEditorCreator: public StringDisplayEditorCreator<T> {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
};

class TLP_QT_SCOPE BooleanEditorCreator: public TulipItemEditorCreator {
public:
  virtual QWidget* createWidget(QWidget*) const;
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant &) const;
  virtual void setEditorData(QWidget*,const QVariant&,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
};

class TLP_QT_SCOPE SizeEditorCreator: public StringDisplayEditorCreator<tlp::SizeType> {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
};

template<typename PROP>
class TLP_QT_SCOPE PropertyEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
};

}

#include "cxx/TulipItemEditorCreators.cxx"

#endif // TULIPITEMEDITORCREATORS_H

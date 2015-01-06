/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN

#ifndef TULIPITEMEDITORCREATORS_H
#define TULIPITEMEDITORCREATORS_H

#include <tulip/tulipconf.h>
#include <tulip/PropertyTypes.h>

#include <QVariant>
#include <QSize>

class QWidget;
class QPainter;
class QStyleOptionViewItem;
class QModelIndex;

namespace tlp {

class PropertyInterface;

class TLP_QT_SCOPE TulipItemEditorCreator {
public:
  virtual ~TulipItemEditorCreator() {}
  virtual QWidget* createWidget(QWidget*) const=0;
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
  virtual QString displayText(const QVariant &) const {
    return "";
  }

  virtual QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

  virtual void setEditorData(QWidget*,const QVariant&,bool isMandatory,tlp::Graph* g=NULL)=0;
  virtual QVariant editorData(QWidget*,tlp::Graph* g=NULL)=0;
  virtual void setPropertyToEdit(tlp::PropertyInterface*) {}
};

template<typename T>
class TLP_QT_SCOPE StringDisplayEditorCreator: public TulipItemEditorCreator {
public:
  inline QString displayText(const QVariant& v) const {
    return QString::fromUtf8(T::toString(v.value<typename T::RealType>()).c_str());
  }
};

class TLP_QT_SCOPE ColorEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
};

template<typename T>
class LineEditEditorCreator: public StringDisplayEditorCreator<T> {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
};

template<typename T>
class MultiLinesEditEditorCreator: public StringDisplayEditorCreator<T> {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
  bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
};

class TLP_QT_SCOPE BooleanEditorCreator: public TulipItemEditorCreator {
public:
  virtual QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*,const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE CoordEditorCreator: public StringDisplayEditorCreator<tlp::PointType> {
  bool editSize;
public:
  CoordEditorCreator():editSize(false) {}
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual void setPropertyToEdit(tlp::PropertyInterface* prop);
};

template<typename PROP>
class PropertyEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE PropertyInterfaceEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE NumericPropertyEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE ColorScaleEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
};

class TLP_QT_SCOPE StringCollectionEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE TulipFileDescriptorEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &) const;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
};

class TLP_QT_SCOPE NodeShapeEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
};

class TLP_QT_SCOPE EdgeExtremityShapeEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
  virtual bool paint(QPainter*, const QStyleOptionViewItem&, const QVariant&) const;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
};

class TLP_QT_SCOPE EdgeShapeEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  virtual void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget*,tlp::Graph*);
  virtual QString displayText(const QVariant &) const;
};

template<typename ElementType>
class VectorEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget* parent) const;
  virtual void setEditorData(QWidget* editor, const QVariant& data,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget* editor,tlp::Graph*);
  virtual QString displayText(const QVariant &data) const;
};

class QVectorBoolEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget* parent) const;
  virtual void setEditorData(QWidget* editor, const QVariant& data,bool,tlp::Graph*);
  virtual QVariant editorData(QWidget* editor,tlp::Graph*);
  virtual QString displayText(const QVariant &data) const;
};

class TLP_QT_SCOPE TulipFontEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget*) const;
  void setEditorData(QWidget*, const QVariant&,bool,tlp::Graph*);
  QVariant editorData(QWidget*,tlp::Graph*);
  QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE TulipLabelPositionEditorCreator: public tlp::TulipItemEditorCreator {
  static QVector<QString> POSITION_LABEL;
public:
  QWidget* createWidget(QWidget*) const;
  void setEditorData(QWidget*w, const QVariant&var, bool, tlp::Graph*);
  QVariant editorData(QWidget*,tlp::Graph*);
  QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE GraphEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget* parent) const;
  void setEditorData(QWidget*w, const QVariant&var, bool, tlp::Graph*);
  QVariant editorData(QWidget*,tlp::Graph*);
  QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE EdgeSetEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget* parent) const;
  void setEditorData(QWidget*w, const QVariant&var, bool, tlp::Graph*);
  QVariant editorData(QWidget*,tlp::Graph*);
  QString displayText(const QVariant &) const;
};

class TLP_QT_SCOPE QStringEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget* parent) const;
  void setEditorData(QWidget*w, const QVariant&var, bool, tlp::Graph*);
  QVariant editorData(QWidget*,tlp::Graph*);
  QString displayText(const QVariant &) const;
  void setPropertyToEdit(tlp::PropertyInterface* prop);

protected:
  std::string propName;
};

class TLP_QT_SCOPE QStringListEditorCreator: public tlp::TulipItemEditorCreator {
public:
  QWidget* createWidget(QWidget* parent) const;
  void setEditorData(QWidget*w, const QVariant& var, bool, tlp::Graph*);
  QVariant editorData(QWidget*,tlp::Graph*);
  QString displayText(const QVariant &) const;
};

}


#if defined(__clang__)
template class tlp::StringDisplayEditorCreator<tlp::PointType>;
template class tlp::StringDisplayEditorCreator<tlp::SizeType>;
#endif


#include "cxx/TulipItemEditorCreators.cxx"

#endif // TULIPITEMEDITORCREATORS_H
///@endcond
